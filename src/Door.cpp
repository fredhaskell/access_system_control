#include "Door.h"
#include "credits.h"
#include <ESP8266HTTPClient.h>
#include "ErrorHandler.h"

void Door::Init()
{
    // Internal initialization
    pinMode(LOCK_PIN, OUTPUT);
    pinMode(LED_BTN_OPEN, OUTPUT);
    pinMode(LED_LOCK, OUTPUT);
    pinMode(BTN_OPEN, INPUT);
    closeLock(true);

    SPI.begin();
    rfid.PCD_Init(RFID_SS_PIN, RFID_RST_PIN);
    delay(4); // Some board do need more time after init to be ready
    rfid.PCD_DumpVersionToSerial();
    
    // External initialization
    while (!ConnectToWiFi())
    {
        // Indicate Wi_Fi connection error
        IndicateState(States::WiFi_Error);
    }
  
    IndicateState(States::OK);
}

bool Door::ConnectToWiFi()
{
    IPAddress localIp(192,168,1,88);
    IPAddress gateway(192,168,1,201);
    IPAddress subnet(255,255,254,0);

    if (!WiFi.config(localIp, gateway, subnet))
        return false;

    WiFi.begin(ssid, pass);
    unsigned char tryConnect = 0;
    while (WiFi.status() != WL_CONNECTED
        && tryConnect < maxTryConnect)
    {
        delay(500);
        ++tryConnect;
    }

    return WiFi.status() == WL_CONNECTED;
}

void Door::CheckCard()
{
    uint32_t id = getCardID();
    if (id == 0)
        return;
    
    if (millis() - lastRequest < 1000)
        return;
    
    if (!hasAccess(id))
    {
        IndicateState(States::AccessDenied_Error);
        return;
    }

    openLock();
}

void Door::CheckButton()
{
    if (digitalRead(BTN_OPEN) != LOW)
        return;

    long prev = millis();
    delay(30);
    openLock();

    while (digitalRead(BTN_OPEN) == LOW)
    {
        delay(30);
        if (millis() - prev > 3000)
        {
            keepOpen = !keepOpen;
            if (keepOpen)
            {
                // set green indicate
                digitalWrite(LED_BTN_OPEN, HIGH);
            }
            else
            {
                // set off indicate
                digitalWrite(LED_BTN_OPEN, LOW);
                delay(2000);
            }

            prev = millis();
        }
    }
}

uint32_t Door::getCardID()
{
    uint32_t id = 0;
    if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial())
    {
        for (byte i = 0; i < 4; ++i)
        {
            id <<=8;
            id |= rfid.uid.uidByte[i];
        }
    }
    return id;
}

bool Door::hasAccess(uint32_t card_id)
{
    lastRequest = millis();
    HTTPClient http;   
    String uri = "/device_status?is_open=" + String(card_id, HEX);
    
    http.begin(m_client, server_ip, server_port, uri);
    
    bool answer = false;
    if (http.GET() == GET_OK)
    {
        if (http.getString() == ALLOWED)
            answer = true;
    }
    else
    {
        IndicateState(States::ServerConnect_Error);
        answer = false;
    }

    http.end();
    return answer;
}

void Door::openLock()
{
    digitalWrite(LED_LOCK, isLockClose ? HIGH : LOW);
    if (!isLockClose)
        return;
    
    digitalWrite(LOCK_PIN, LOW);
    digitalWrite(LED_LOCK, LOW);
    isLockClose = false;
    lastOpen = millis();
}

void Door::closeLock(bool ingoreMillis)
{
    digitalWrite(LED_LOCK, isLockClose ? HIGH : LOW);

    if (keepOpen || isLockClose)
        return;

    if (ingoreMillis)
    {
        isLockClose = true;
    }
    else
    {
        isLockClose = millis() - lastOpen > 5000;
    }

    if (isLockClose)
    {
        digitalWrite(LOCK_PIN, HIGH);
        digitalWrite(LED_LOCK, HIGH);
    }
}