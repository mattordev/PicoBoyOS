#pragma once
#include "../sys/PBOS_Config.h"

// ✅ Forward declare instead of including UIManager.h
class UIManager;

class SensorManager {
public:
    explicit SensorManager(UIManager &uiRef);

    void begin();
    float getTemperature();
    void blinkLED(int delayTime = 100);  // lowercase for consistency
    void update();

private:
    UIManager &ui;
};
