#pragma once
#include <WiFi.h>
#include "../ui/UIManager.h"
#include "../sys/PBOS_Config.h"

class NetworkManager {
public:
    NetworkManager(UIManager &uiRef);

    // Start connection
    void begin(const char* ssid_, const char* password_, unsigned long timeoutMs = PB_WIFI_TIMEOUT_MS);

    // Poll updates
    void update();

    // Status queries
    bool isConnected() const { return connected; }
    bool isFinished() const { return finished; }
    String getIP() const;

private:
    UIManager &ui;
    const char* ssid;
    const char* password;
    unsigned long timeout = PB_WIFI_TIMEOUT_MS;
    unsigned long startTime = 0;
    bool started = false;
    bool finished = false;
    bool connected = false;
};
