// WiFi
const char* ssid = "YOUR_SSID";
const char* pass = "YOUR_PASS";
const char maxTryConnect = 20;
// see local ip, gateway, subnet in ConnectToWiFi() func

// Server
const char* server_ip = "YOUR_SERVER_IP_OR_NAME";
uint16_t server_port = 5000;
#define ALLOWED "1"
#define GET_OK 200

// RFID
#define RFID_RST_PIN D3
#define RFID_SS_PIN  D4
// SCK - D5
// MOSI - D7
// MISO - D6


// SERVICE
#define LOCK_PIN D2
#define LED_BTN_OPEN D0
#define LED_LOCK D8
#define BTN_OPEN D1