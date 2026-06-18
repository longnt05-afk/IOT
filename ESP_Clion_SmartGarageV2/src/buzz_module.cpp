//
// Created by nguye on 19/10/2025.
//
#include "buzz_module.h"
#include "config.h"   // Để lấy BUZZER_PIN
#include <Arduino.h>  // Để dùng tone(), noTone(), delay()

// --- Cấu hình âm thanh (Tần số Hz, Thời gian ms) ---
const int TONE_FAIL = 1000;    // Âm trung
const int DURATION_FAIL = 1000; // 1 bíp dài

const int TONE_ALERT = 1500;   // Âm cao hơn
const int DURATION_ALERT = 100;// Bíp ngắn
const int PAUSE_ALERT = 50;    // Nghỉ ngắn

const int TONE_WARN = 2000;    // Âm cao nhất
const int DURATION_WARN = 150; // Bíp trung bình
const int PAUSE_WARN = 80;     // Nghỉ ngắn


// Khởi tạo chân còi
void setupBuzzer() {
    pinMode(BUZZER_PIN, OUTPUT);
    digitalWrite(BUZZER_PIN, LOW); // Đảm bảo tắt khi khởi động
    Serial.println("Buzzer Module OK.");
}

// Phát 1 tiếng bíp thất bại
void beepFail() {
    tone(BUZZER_PIN, TONE_FAIL, DURATION_FAIL);

}

// Phát 3 tiếng bíp cảnh báo
void beepAlert() {
    for (int i = 0; i < 3; i++) {
        tone(BUZZER_PIN, TONE_ALERT, DURATION_ALERT);
        delay(DURATION_ALERT + PAUSE_ALERT);
    }
    noTone(BUZZER_PIN);
}

// Phát 5 tiếng bíp cảnh báo nguy hiểm
void beepWarning() {
    for (int i = 0; i < 5; i++) {
        tone(BUZZER_PIN, TONE_WARN, DURATION_WARN);
        delay(DURATION_WARN + PAUSE_WARN);
    }
    noTone(BUZZER_PIN);
}