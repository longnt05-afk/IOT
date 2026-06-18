//
// Created by nguye on 18/10/2025.
//
#include "ultrasonic_module.h"
#include "config.h"     // Nạp config để lấy chân PIN
#include <Arduino.h>    // Nạp Arduino để dùng pinMode, pulseIn...

// Hằng số thời gian cho việc kích hoạt cảm biến siêu âm (tính bằng micro giây)
static const unsigned int ULTRASONIC_TRIGGER_PULSE_US = 10;
static const unsigned int ULTRASONIC_CLEAN_PULSE_US = 2;

// Hàm nội bộ (private) để đo khoảng cách
static float measureDistance(int trigPin, int echoPin) {
    digitalWrite(trigPin, LOW);
    delayMicroseconds(ULTRASONIC_CLEAN_PULSE_US);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(ULTRASONIC_TRIGGER_PULSE_US);
    digitalWrite(trigPin, LOW);

    long duration = pulseIn(echoPin, HIGH, 25000); // Timeout 25ms

    float distance = duration * 0.0343 / 2;

    if (distance == 0) return 999; // Trả về 999 nếu timeout
    return distance;
}

// Khởi tạo các chân cảm biến
void setupUltrasonicSensors() {
    pinMode(FLOOD_SENSOR_TRIG, OUTPUT);
    pinMode(FLOOD_SENSOR_ECHO, INPUT);

    pinMode(GARAGE_SENSOR_TRIG, OUTPUT);
    pinMode(GARAGE_SENSOR_ECHO, INPUT);

    Serial.println("Ultrasonic Module OK.");
}

// Lấy khoảng cách từ cảm biến chống ngập
float getFloodDistance() {
    return measureDistance(FLOOD_SENSOR_TRIG, FLOOD_SENSOR_ECHO);
}

// Lấy khoảng cách từ cảm biến garage
float getGarageDistance() {
    return measureDistance(GARAGE_SENSOR_TRIG, GARAGE_SENSOR_ECHO);
}
