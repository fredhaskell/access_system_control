
enum class States
{
    OK,
    WiFi_Error,
    ServerConnect_Error,
    AccessDenied_Error
};

void blinkLed(int nTimes, int msDelay)
{
    for (int i=0; i < nTimes; ++i)
    {
        digitalWrite(LED_LOCK, HIGH);
        delay(msDelay);
        digitalWrite(LED_LOCK, LOW);
        delay(msDelay);
    }
}

void IndicateState(States code)
{
    switch (code)
    {
    case States::OK:
        blinkLed(2, 1000);
        break;
    case States::WiFi_Error:
        blinkLed(7, 500);
        break;
    case States::ServerConnect_Error:
        blinkLed(15, 100);
        break;
    case States::AccessDenied_Error:
        blinkLed(2, 200);
        break;
    default:
        break;
    }
}