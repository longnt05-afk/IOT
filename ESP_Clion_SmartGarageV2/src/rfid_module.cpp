//
// Created by nguye on 18/10/2025.
//
#include "rfid_module.h"
#include "config.h"
#include <SPI.h>
#include <MFRC522.h>

MFRC522 mfrc522(RFID_SS_PIN, RFID_RST_PIN);

void setupRFID() {
    SPI.begin(RFID_SCK_PIN, RFID_MISO_PIN, RFID_MOSI_PIN, RFID_SS_PIN);
    mfrc522.PCD_Init();

    byte version = mfrc522.PCD_ReadRegister(mfrc522.VersionReg);
    //8-bit
    if (version != 0x00 && version != 0xFF) {
        Serial.println("Da tim thay dau doc RFID MFRC522.");
    } else {
        Serial.println("LOI: Khong tim thay MFRC522. Kiem tra day noi!");
    }
}

String checkNewCardUID() {
    if (!mfrc522.PICC_IsNewCardPresent()) {
        return "";
    }
    if (!mfrc522.PICC_ReadCardSerial()) {
        return "";
    }

    String uid = "";
    for (byte i = 0; i < mfrc522.uid.size; i++) {
        uid += (mfrc522.uid.uidByte[i] < 0x10 ? "0" : "");
        uid += String(mfrc522.uid.uidByte[i], HEX);
    }
    uid.toUpperCase();
    mfrc522.PICC_HaltA();
    return uid;
}

bool isFamilyCard(String uid) {
    if (uid.isEmpty()) {
        return false;
    }
    for (int i = 0; i < NUM_FAMILY_CARDS; i++) {
        if (uid.equals(FAMILY_UIDS[i])) {
            return true;
        }
    }
    return false;
}
