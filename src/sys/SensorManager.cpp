#include "SensorManager.h"
#include <Arduino.h>

SensorManager::SensorManager(UIManager &uiRef) : ui(uiRef) {}

void SensorManager::begin() {
    // Any sensor initialization goes here
    // For now, just the internal temp is enough
}

float SensorManager::getTemperature() {
    float tempC;
    while (true) {
        tempC = analogReadTemp(); // Internal temp reading

        ui.printToTFT("System: Temperature (ºC) = " + String(tempC), 2000);

        if (tempC > -20.0 && tempC < 150.0) {
            break; // Valid reading
        }
    }
    return tempC;
}

void SensorManager::update() {
    // Placeholder for async polling if needed later
}
