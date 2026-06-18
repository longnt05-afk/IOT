//
// Created by nguye on 18/10/2025.
//
#include "servo_module.h"
#include "config.h"       // Nạp file config để lấy chân PIN
#include <ESP32Servo.h>   // Nạp thư viện Servo

// Khởi tạo đối tượng servo

static Servo floodServo;

// Khởi tạo servo cửa chống lũ
void setupServos() {
    floodServo.attach(FLOOD_SERVO_PIN);
    floodServo.write(FLOOD_SERVO_CLOSE_ANGLE); // Đảm bảo cửa đóng khi khởi động
    Serial.println("Servo Module OK.");
}

// Đặt góc quay cho servo cửa lũ
void setFloodDoorAngle(int angle) {
    floodServo.write(angle);
}
