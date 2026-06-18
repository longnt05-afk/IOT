//
// Created by nguye on 19/10/2025.
//

#ifndef ESP_CLION_BUZZ_MODULE_H
#define ESP_CLION_BUZZ_MODULE_H
// Khởi tạo chân cắm cho còi buzzer
void setupBuzzer();

// Phát 1 tiếng bíp ngắn (thẻ không hợp lệ)
void beepFail();

// Phát 3 tiếng bíp (cảnh báo cửa lũ)
void beepAlert();

// Phát 5 tiếng bíp liên tục (cảnh báo ngập trong gara)
void beepWarning();
#endif //ESP_CLION_BUZZ_MODULE_H