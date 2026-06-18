//
// Created by nguye on 18/10/2025.
//

#ifndef ESP_CLION_CONFIG_H
#define ESP_CLION_CONFIG_H
#pragma once // Đảm bảo file này chỉ được nạp 1 lần
#include <Arduino.h>
// Màn hình LCD I2C
static const int LCD_SDA_PIN = 21;
static const int LCD_SCL_PIN = 22;
// Cảm biến DHT11
static const int DHT_PIN = 15;
#define DHT_TYPE    DHT11
// --- Cảm biến siêu âm ---
//     Ngoài cửa
static const int FLOOD_SENSOR_TRIG = 33; // Cảm biến siêu âm 1 (Chống ngập)
static const int FLOOD_SENSOR_ECHO = 32; //
//     Trong Garage
static const int GARAGE_SENSOR_TRIG = 25; // Cảm biến siêu âm 2
static const int GARAGE_SENSOR_ECHO = 26; //

// --- Cửa chống lũ ---
static const int FLOOD_SERVO_PIN = 13; // Servo 1 (Cửa chống lũ)

//     Cửa cuốn
static const int GARAGE_MOTOR_ENA = 4;
static const int GARAGE_MOTOR_IN1 = 17; // Rx2
static const int GARAGE_MOTOR_IN2 = 16; // Tx2
// --- Máy bơm nước trong gara ---
static const int PUMP_MOTOR_ENA = 2;

// --- Chân cắm RFID (SPI) ---
static const int RFID_SS_PIN = 5;
static const int RFID_SCK_PIN = 18;
static const int RFID_MOSI_PIN = 23;
static const int RFID_MISO_PIN = 19;
static const int RFID_RST_PIN = 27;

// --- Còi Buzz ---
static const int BUZZER_PIN = 14;
// --- Cấu hình chân ẢO (Virtual Pins) cho Blynk ---

#define VPIN_GARAGE_DOOR    V0 // Cua Garage (Switch)
#define VPIN_FLOOD_DOOR     V1 // Cua chong ngap (Switch)
#define VPIN_FLOOD_SENSOR   V2 // Cam bien ngap ngoai (Sensor)
#define VPIN_FLOOD_IN_GARAGE_SENSOR   V3 // Cam bien ngap trong garage (Sensor)
#define VPIN_PUMP           V4 // May bom trong ham (Switch)
#define VPIN_TEMP           V5 // Nhiet do
#define VPIN_HUMI           V6 // Do am


// --- DANH SÁCH UID GIA ĐÌNH (Hardcoded) ---

const int NUM_FAMILY_CARDS = 4;
static String FAMILY_UIDS[NUM_FAMILY_CARDS] = {
    "1D102403", // UID 1
    "E6B83C03", // UID 2
    "D355D60C", // UID 3
    //"B34BE32C" // UID 4
};

// --- Cấu hình logic chống ngập ---

static const float FLOOD_TRIGGER_DISTANCE_CM = 5.0; // (cm)
static const int FLOOD_SERVO_OPEN_ANGLE = 145; // Góc "Bật"
static const int FLOOD_SERVO_CLOSE_ANGLE = 0; // Góc "Tắt"
static const int GARAGE_MOTOR_RUN_TIME_MS = 1700; // 1.6 giây
static const int GARAGE_MOTOR_SPEED = 255; // Tốc độ (0-255)
static const float GARAGE_FLOOD_TRIGGER_CM = 5.0; // (cm)
static const int FLOOD_RECHECK = 30000; // 30s
static const int GARAGE_FLOOD_RECHECK = 10000; // 10s
#endif //ESP_CLION_CONFIG_H
