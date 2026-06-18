//
// Created by nguye on 18/10/2025.
//
#include "blynk_module.h"
#include "secrets_data.h"
#include "config.h"
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <Arduino.h>

void setupBlynk() {
    Serial.println("Dang ket noi toi WiFi...");
    WiFi.begin(WIFI_SSID, WIFI_PASS);
    int wifiAttempts = 0;
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
        wifiAttempts++;
        if (wifiAttempts > 20) {
            Serial.println("\nKhong the ket noi WiFi!");
            return;
        }
    }
    Serial.println("\nDa ket noi WiFi!");

    Blynk.config(BLYNK_AUTH_TOKEN);
    Serial.println("Dang ket noi toi Blynk...");
    Blynk.connect(10000);
    if (Blynk.connected()) {
        Serial.println("Da ket noi Blynk!");
    } else {
        Serial.println("Khong the ket noi Blynk.");
    }
}
//duy trì kết nối giữa thiết bị và server Blynk
void runBlynk() {
    if (WiFi.status() == WL_CONNECTED) {
        if (!Blynk.connected()) {
            Serial.println("Mat ket noi Blynk. Dang thu ket noi lai...");
            Blynk.connect();
        }
        Blynk.run();
    } else {
        Serial.println("Mat ket noi WiFi. Dang thu ket noi lai...");
        WiFi.reconnect();
    }
}

void sendDHTDataToBlynk(float temp, float humi) {
    if (Blynk.connected()) {
        if (!isnan(temp)) {
            Blynk.virtualWrite(VPIN_TEMP, temp);
        }
        if (!isnan(humi)) {
            Blynk.virtualWrite(VPIN_HUMI, humi);
        }
    }
}

void sendOutsideFloodDataToBlynk(float distance, bool isDoorOpen) {
    if (Blynk.connected()) {
        if (!isnan(distance)) {
            Blynk.virtualWrite(VPIN_FLOOD_SENSOR, distance);
        }
        Blynk.virtualWrite(VPIN_FLOOD_DOOR, isDoorOpen ? 1 : 0);
    }
}

void sendFloodInGarageDataToBlynk(float distance) {
    if (Blynk.connected()) {
        if (!isnan(distance)) {
            Blynk.virtualWrite(VPIN_FLOOD_IN_GARAGE_SENSOR, distance);
        }
    }
}

void sendBlynkLogEvent(String eventCode) {
    if (Blynk.connected()) {
        Serial.printf("!!! Dang gui Log Event: %s\n", eventCode.c_str());
        Blynk.logEvent(eventCode);
    } else {
        Serial.println("LOI: Khong the gui Log Event (mat ket noi Blynk).");
    }
}
void syncGarageDoorState(bool isOpen) {
    if (Blynk.connected()) {
        // Gọi hàm virtualWrite từ BÊN TRONG module này
        Blynk.virtualWrite(VPIN_GARAGE_DOOR, isOpen ? 1 : 0);
    }
}

void sendInsideFloodDataToBlynk(float distance, bool isPumpOn) {
    if (Blynk.connected()) {
        if (!isnan(distance)) {
            Blynk.virtualWrite(VPIN_FLOOD_IN_GARAGE_SENSOR, distance); // Gửi V3
        }
        Blynk.virtualWrite(VPIN_PUMP, isPumpOn ? 1 : 0); // Gửi V4
    }
}









