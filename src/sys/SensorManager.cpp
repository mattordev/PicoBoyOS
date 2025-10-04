#include "SensorManager.h"
#include "../ui/UIManager.h"   // full definition only here
#include <Arduino.h>

SensorManager::SensorManager(UIManager &uiRef) : ui(uiRef) {}

void SensorManager::begin() {
    // Initialize any sensors (or LED for now)
    pinMode(LED_BUILTIN, OUTPUT);
}

// Simple onboard LED blink
void SensorManager::blinkLED(int delayTime) {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(delayTime);
    digitalWrite(LED_BUILTIN, LOW);
}

float SensorManager::getTemperature() {
    float tempC;
    while (true) {
        tempC = analogReadTemp(); // Internal temperature sensor (RP2040)

        ui.printToTFT("System: Temperature (ºC) = " + String(tempC), 2000);

        // Use config constants for validity
        if (tempC > PB_TEMP_MIN_C && tempC < PB_TEMP_MAX_C) {
            break; // Valid reading
        }
    }
    return tempC;
}

void SensorManager::update() {
    // Placeholder for async sensor polling (future expansion)
}
