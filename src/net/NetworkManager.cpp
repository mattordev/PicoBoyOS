#include "NetworkManager.h"

NetworkManager::NetworkManager(UIManager &uiRef) : ui(uiRef) {}

void NetworkManager::begin(const char* ssid_, const char* password_, unsigned long timeoutMs) {
    // Store credentials and timeout
    ssid = ssid_;
    password = password_;
    timeout = timeoutMs;

    // Set WiFi mode and start connection
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);

    // Track timing and state for async updates
    startTime = millis();
    started = true;
    finished = false;
    connected = false;

    // Provide immediate feedback on the TFT
    ui.printToTFT("System: Connecting to WiFi network " + String(ssid), 0); // 0 = no delay
}

void NetworkManager::update() {
    if (!started || finished) return;

    wl_status_t status = (wl_status_t)WiFi.status();
    unsigned long now = millis();

    if (status == WL_CONNECTED) {
        connected = true;
        finished = true;
        ui.printToTFT("System: PicoBoy connected to WiFi network " + String(WiFi.SSID()), 2000);
        ui.printToTFT("System: Assigned IP Address: " + WiFi.localIP().toString(), 2000);
    } else if (now - startTime >= timeout) {
        connected = false;
        finished = true;
        ui.printToTFT("System: WiFi connection failed. Running in offline mode.", 2000);
    }
}

String NetworkManager::getIP() const {
    return connected ? WiFi.localIP().toString() : String("0.0.0.0");
}
