//
// Created by nguye on 19/10/2025.
//

#ifndef ESP_CLION_MOTOR_MODULE_H
#define ESP_CLION_MOTOR_MODULE_H
// Khởi tạo các chân cắm L298N
void setupMotors();
// Khởi tạo chân cắm máy bơm
void setupPump();
//  xử lý việc tự động dừng động cơ (non-blocking).
void motor_loop();

// Kích hoạt hành động Mở/Đóng cửa (tự động đảo trạng thái).
void changeGarageDoorState();

// Kích hoạt động cơ chạy ĐÓNG CỬA (Sẽ tự động dừng sau GARAGE_MOTOR_RUN_TIME_MS)
bool isGarageDoorOpen();

// Bật hoặc Tắt máy bơm.
void setPumpState(bool on);
#endif //ESP_CLION_MOTOR_MODULE_H
