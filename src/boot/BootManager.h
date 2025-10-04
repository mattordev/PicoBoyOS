#pragma once
#include "../ui/UIManager.h"
#include "../input/InputManager.h"
#include "../net/NetworkManager.h"
#include "../sys/SensorManager.h"

class BootManager {
public:
    BootManager(UIManager &uiRef, InputManager &inputRef, NetworkManager &netRef, SensorManager &sensorsRef);

    // Run full boot sequence
    void runBoot(const char* ssid, const char* password);

private:
    UIManager &ui;
    InputManager &input;
    NetworkManager &net;
    SensorManager &sensors;

    void blinkLED(int delayTime);
};
