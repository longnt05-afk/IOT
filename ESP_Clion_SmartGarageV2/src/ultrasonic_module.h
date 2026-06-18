//
// Created by nguye on 18/10/2025.
//
#ifndef ESP_CLION_ULTRASONIC_MODULE_H
#define ESP_CLION_ULTRASONIC_MODULE_H

// Khởi tạo (pinMode) cho tất cả cảm biến siêu âm
void setupUltrasonicSensors();

// Lấy khoảng cách từ cảm biến chống ngập (sensor 1 - Bên ngoài)
float getFloodDistance();

// Lấy khoảng cách từ cảm biến garage (sensor 2 - Trong gara)
float getGarageDistance();
#endif //ESP_CLION_ULTRASONIC_MODULE_H