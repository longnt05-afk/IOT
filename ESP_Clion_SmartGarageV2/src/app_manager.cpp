// C++
#include "app_manager.h"
#include <Arduino.h>

// Nạp file cấu hình chung
#include "config.h"
#include "secrets_data.h"
// Nạp các module con
#include "lcd_module.h"
#include "dht_module.h"
#include "blynk_module.h"
#include "rfid_module.h"
#include "servo_module.h"
#include "ultrasonic_module.h"
#include "motor_module.h"
#include "buzz_module.h"
#include "Blynk/BlynkHandlers.h"

// --- Cấu hình Timer ---

// Thời gian đọc cảm biến DHT
#define DHT_READ_INTERVAL 5000
unsigned long previousMillis_DHT = 0;

// Thời gian đọc cảm biến chống ngập ngoài
#define FLOOD_READ_INTERVAL 5000
unsigned long previousMillis_Flood = 0;

// Thời gian hiển thị thông báo LCD khi quét thẻ RFID
#define LCD_MSG_DURATION 3000
unsigned long messageDisplayTime = 0;

// Thời gian đọc khoảng cách trong gara (ngập trong gara)
#define GARAGE_READ_INTERVAL 5000
unsigned long previousMillis_Garage = 0;

// --- Biến trạng thái của ứng dụng ---
// Các trạng thái hiển thị / chờ
static const int STATE_IDLE = 0;
static const int STATE_ACCESS_MSG = 1;
int currentState = STATE_IDLE;

// Trạng thái cảnh báo chống ngập
bool isFlooded = false; // ngoài gara
static float lastFloodDist = 999;
bool isGarageFlooded = false; // trong gara

// Đồng bộ trạng thái nút bấm nếu có lữo bị lệch pha khi lỡ mất điện
BLYNK_CONNECTED() {
    Serial.println("[Blynk] Da ket noi/ket noi lai. Dong bo trang thai len App...");
    syncGarageDoorState(isGarageDoorOpen());
}

// CALLBACK BLYNK: xử lý lệnh mở/đóng cửa từ app
BLYNK_WRITE(VPIN_GARAGE_DOOR) {
    // Chuẩn hóa đầu vào -> 0 hoặc 1
    int blynkValue = (param.asInt() != 0) ? 1 : 0;
    int currentStateValue = isGarageDoorOpen() ? 1 : 0;
    Serial.printf("[Blynk] Nhan V0, gia tri: %d\n", blynkValue);
    if (blynkValue == currentStateValue) {
        return;
    }
    bool prevState = isGarageDoorOpen();
    changeGarageDoorState();

    bool newState = isGarageDoorOpen();
    if (newState != prevState) {
        if (newState) {
            Serial.println("[Logic] Nhan lenh MO CUA");
            sendBlynkLogEvent("mo_cua_garage");
            displayLCD("GARAGE", "Dang mo cua...");
        } else {
            Serial.println("[Logic] Nhan lenh DONG CUA");
            sendBlynkLogEvent("dong_cua_garage");
            displayLCD("GARAGE", "Dang dong cua...");
        }

        syncGarageDoorState(newState);
    } else {
        Serial.println("[Blynk] Lenh KHONG duoc thuc hien (motor dang hoat dong)");
        syncGarageDoorState(prevState);
    }
}

// Khởi tạo toàn bộ ứng dụng
void app_setup() {
    Serial.begin(115200);
    Serial.println("\n--- Chuong trinh Smart Garage V1 ---");
    Serial.println("Dang khoi tao cac module phan cung...");

    // Khởi tạo các module phần cứng
    setupLCD();
    setupDHT();
    setupRFID();
    setupServos();
    setupUltrasonicSensors();
    setupMotors();
    setupBuzzer();
    Serial.println("Khoi tao phan cung OK.");
    Serial.println("Dang khoi tao ket noi...");

    // Khởi tạo Blynk (bao gồm WiFi)
    setupBlynk();

    Serial.println("He thong san sang.");
    displayLCD("Smart Garage V1", "Cho quet the...");
}

// Chạy vòng lặp chính
void app_loop() {
    unsigned long currentMillis = millis();

    // 1. Luôn chạy Blynk và vòng lặp điều khiển motor
    runBlynk();
    motor_loop();

    // 2. Xử lý trạng thái hiển thị thông báo trên LCD (khi quét thẻ)
    if (currentState == STATE_ACCESS_MSG) {
        if (currentMillis - messageDisplayTime >= LCD_MSG_DURATION) {
            currentState = STATE_IDLE;
        }
    }

    // 3a. Nếu kiểm tra RFID (trạng thái chờ)
    if (currentState == STATE_IDLE) {
        String cardUID = checkNewCardUID();
        if (!cardUID.isEmpty()) {
            Serial.print("\nPhat hien the! UID: ");
            Serial.println(cardUID);

            if (isFamilyCard(cardUID)) {
                // Thẻ hợp lệ -> chấp nhận truy cập, mở/đóng cửa
                Serial.println("=> The hop le. Chao mung ve nha!");
                displayLCD("THE HOP LE", "Chao mung ve nha!");
                changeGarageDoorState();
                if (isGarageDoorOpen()) {
                    Serial.println("[Logic] Nhan lenh MO CUA");
                    sendBlynkLogEvent("mo_cua_garage");
                    displayLCD("GARAGE", "Dang mo cua...");
                } else {
                    Serial.println("[Logic] Nhan lenh DONG CUA");
                    sendBlynkLogEvent("dong_cua_garage");
                    displayLCD("GARAGE", "Dang dong cua...");
                }
                // Đồng bộ trạng thái cửa lên Blynk
                syncGarageDoorState(isGarageDoorOpen());

                currentState = STATE_ACCESS_MSG;
                messageDisplayTime = millis();
                return;
            } else {
                Serial.println("=> LOI: The la. Tu choi truy cap!");
                beepFail();
                displayLCD("THE KHONG HOP LE", "Tu choi truy cap!");
            }

            currentState = STATE_ACCESS_MSG;
            messageDisplayTime = currentMillis;
            return;
        }
    }

    // 3b. Cảm biến chống ngập bên ngoài
    unsigned long outGarageCheckInterval;
    if (isFlooded) {
        outGarageCheckInterval = FLOOD_RECHECK;
    } else {
        outGarageCheckInterval = FLOOD_READ_INTERVAL;
    }
    if (currentMillis - previousMillis_Flood >= outGarageCheckInterval) {
        previousMillis_Flood = currentMillis;

        float floodDist = getFloodDistance();
        lastFloodDist = floodDist;
        bool openDoor = false;

        if (floodDist <= FLOOD_TRIGGER_DISTANCE_CM && floodDist > 0) {
            setFloodDoorAngle(FLOOD_SERVO_OPEN_ANGLE);
            openDoor = true;
            if (!isFlooded) {
                Serial.println("!!! CANH BAO: Phat hien nuoc tran vao garage!");
                sendBlynkLogEvent("nuoc_tran_vao_garage");
                isFlooded = true;
            }
        } else {
            setFloodDoorAngle(FLOOD_SERVO_CLOSE_ANGLE);
            openDoor = false;
            if (isFlooded) {
                Serial.println("Nuoc da rut. He thong tro ve binh thuong.");
                sendBlynkLogEvent("nuoc_ben_ngoai_gara_da_rut");
                isFlooded = false;
            }
        }

        // Gửi dữ liệu lên Blynk
        sendOutsideFloodDataToBlynk(floodDist, openDoor);
        Serial.printf("[LOG]  LastOutSideFloodDist: %.1fcm\n", lastFloodDist);
    }

    // 3c. Cảm biến DHT
    if (currentMillis - previousMillis_DHT >= DHT_READ_INTERVAL) {
        previousMillis_DHT = currentMillis;
        float temp = getTemperature();
        float humi = getHumidity();
        sendDHTDataToBlynk(temp, humi);
        if (currentState == STATE_IDLE) {
            String line1 = "Temp: " + (isnan(temp) ? "LOI" : String(temp, 1)) + (char) 223 + "C";
            String line2 = "Humi: " + (isnan(humi) ? "LOI" : String(humi, 1)) + " %";
            displayLCD(line1, line2);
        }
        Serial.printf("[LOG] Temp: %.1f*C | Humi: %.1f%% \n", temp, humi);
    }


    // 3d. Kiểm tra ngập trong gara
    unsigned long inGarageCheckInterval;
    if (isGarageFlooded) {
        inGarageCheckInterval = GARAGE_FLOOD_RECHECK;
    } else {
        inGarageCheckInterval = GARAGE_READ_INTERVAL;
    }
    if (currentMillis - previousMillis_Garage >= inGarageCheckInterval) {
        previousMillis_Garage = currentMillis;

        float garageDist = getGarageDistance();
        bool isPumpOn = false;

        if (garageDist <= GARAGE_FLOOD_TRIGGER_CM && garageDist > 0) {
            isPumpOn = true;
            if (!isGarageFlooded) {
                Serial.println("!!! CANH BAO: Phat hien nuoc tran TRONG GARA!");
                sendBlynkLogEvent("gara_bi_ngap_nuoc");
                isGarageFlooded = true;
            }
        } else {
            if (isGarageFlooded) {
                Serial.println("Nuoc TRONG GARA da rut. Tat bom.");
                isGarageFlooded = false;
            }
        }

        setPumpState(isPumpOn);
        sendInsideFloodDataToBlynk(garageDist, isPumpOn);
        Serial.printf("[LOG] Garage Dist: %.1fcm | PumpOn: %d\n", garageDist, isPumpOn ? 1 : 0);
        Serial.println();
    }
}
