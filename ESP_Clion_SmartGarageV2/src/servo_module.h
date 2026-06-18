//
// Created by nguye on 18/10/2025.
//
// Servo module header

#ifndef ESP_CLION_SERVO_MODULE_H
#define ESP_CLION_SERVO_MODULE_H

// Khởi tạo tất cả các servo trong dự án
void setupServos();

// Đặt góc quay cho servo cửa chống lũ

void setFloodDoorAngle(int angle);

#endif //ESP_CLION_SERVO_MODULE_H