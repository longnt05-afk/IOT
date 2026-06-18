//
// Created by nguye on 18/10/2025.
//

#ifndef ESP_CLION_RFID_MODULE_H
#define ESP_CLION_RFID_MODULE_H
#pragma once
#include <Arduino.h>

void setupRFID();

String checkNewCardUID();

bool isFamilyCard(String uid);
#endif //ESP_CLION_RFID_MODULE_H
