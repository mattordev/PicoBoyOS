#pragma once
#include "../ui/UIManager.h"
#include "../sys/PBOS_Config.h"

class SensorManager {
public:
    SensorManager(UIManager &uiRef);

    // Initialize sensors
    void begin();

    // Get internal temperature in °C
    float getTemperature();

    // Optional: poll sensors (for async expansion)
    void update();

private:
    UIManager &ui;
};
