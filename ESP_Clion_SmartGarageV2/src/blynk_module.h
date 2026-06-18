//
// Created by nguye on 18/10/2025.
//
#include <Arduino.h>
#ifndef ESP_CLION_BLYNK_MODULE_H
#define ESP_CLION_BLYNK_MODULE_H

void setupBlynk();

void runBlynk();

void sendDHTDataToBlynk(float temp, float humi);

void sendOutsideFloodDataToBlynk(float distance, bool isDoorOpen);

void sendBlynkLogEvent(String eventCode);

void syncGarageDoorState(bool isOpen);

void sendInsideFloodDataToBlynk(float distance, bool isPumpOn);
#endif //ESP_CLION_BLYNK_MODULE_H
