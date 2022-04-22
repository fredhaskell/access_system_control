#pragma once

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <MFRC522.h>
#include <SPI.h>

class Door
{
public:
    void Init();
    bool ConnectToWiFi();
    void CheckCard();
    void CheckButton();
    void closeLock(bool ingoreMillis = false);

private:
    uint32_t getCardID();
    bool hasAccess(uint32_t);
    void openLock();

private:
    WiFiClient m_client;
    MFRC522 rfid;
    bool keepOpen = false;
    bool isLockClose = false;
    long lastRequest = 1;
    long lastOpen = 1;
};