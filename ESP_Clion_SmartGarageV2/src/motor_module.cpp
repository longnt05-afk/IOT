//
// Created by nguye on 19/10/2025.
//
#include "motor_module.h"
#include "config.h"
#include <Arduino.h>

// --- Biến trạng thái NỘI BỘ (private) ---
static bool s_isGarageDoorOpen = false; // Trạng thái cửa
static bool s_isMotorRunning = false;
static unsigned long s_motorStartTime = 0;

// Hàm nội bộ để dừng động cơ
static void stopGarageMotor() {
    digitalWrite(GARAGE_MOTOR_IN1, LOW);
    digitalWrite(GARAGE_MOTOR_IN2, LOW);
    analogWrite(GARAGE_MOTOR_ENA, 0);
    s_isMotorRunning = false;
    Serial.println("[Motor] Dong co da dung.");
}

// Hàm nội bộ kích hoạt động cơ MỞ CỬA
static void openGarageMotor_internal() {
    Serial.println("[Motor] Dang... MO CUA");
    digitalWrite(GARAGE_MOTOR_IN1, HIGH);
    digitalWrite(GARAGE_MOTOR_IN2, LOW);
    analogWrite(GARAGE_MOTOR_ENA, GARAGE_MOTOR_SPEED);
    s_isMotorRunning = true;
    s_motorStartTime = millis();
}

// Hàm nội bộ kích hoạt động cơ ĐÓNG CỬA
static void closeGarageMotor_internal() {
    Serial.println("[Motor] Dang... DONG CUA");
    digitalWrite(GARAGE_MOTOR_IN1, LOW);
    digitalWrite(GARAGE_MOTOR_IN2, HIGH);
    analogWrite(GARAGE_MOTOR_ENA, GARAGE_MOTOR_SPEED);
    s_isMotorRunning = true;
    s_motorStartTime = millis();
}

// --- Các hàm "CÔNG KHAI" (public) ---

void setupMotors() {
    // Motor Cửa Cuốn
    pinMode(GARAGE_MOTOR_ENA, OUTPUT);
    pinMode(GARAGE_MOTOR_IN1, OUTPUT);
    pinMode(GARAGE_MOTOR_IN2, OUTPUT);
    stopGarageMotor(); // Dừng motor cửa cuốn

    // Máy Bơm
    pinMode(PUMP_MOTOR_ENA, OUTPUT);
    digitalWrite(PUMP_MOTOR_ENA, LOW);
    Serial.println("Motor Module (Cua + Bom) OK.");
}

void motor_loop() {
    if (!s_isMotorRunning) return;
    if (millis() - s_motorStartTime >= GARAGE_MOTOR_RUN_TIME_MS) {
        stopGarageMotor();
    }
}

// Hàm Mở/Đóng cửa
void changeGarageDoorState() {
    if (s_isMotorRunning) return; // Đang bận, từ chối

    if (s_isGarageDoorOpen) {
        // Đang Mở -> Lệnh Đóng
        closeGarageMotor_internal();
        s_isGarageDoorOpen = false;
    } else {
        // Đang Đóng -> Lệnh Mở
        openGarageMotor_internal();
        s_isGarageDoorOpen = true;
    }
}

// Hàm "hỏi" trạng thái cửa
bool isGarageDoorOpen() {
    return s_isGarageDoorOpen;
}

// Bật/Tắt máy bơm
void setPumpState(bool on) {
    // Giả định D2 điều khiển 1 relay, HIGH là Bật
    digitalWrite(PUMP_MOTOR_ENA, on ? HIGH : LOW);
}



