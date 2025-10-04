#include "BootManager.h"
#include "../ui/UIManager.h"

#include <Arduino.h>
#include <XPT2046_Touchscreen.h>

extern XPT2046_Touchscreen ts; // Touchscreen object from main.cpp

BootManager::BootManager(UIManager &uiRef, InputManager &inputRef, NetworkManager &netRef, SensorManager &sensorRef)
    : ui(uiRef), input(inputRef), net(netRef), sensors(sensorRef) {}

// Simple LED blink
void BootManager::blinkLED(int delayTime) {
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(delayTime);
    digitalWrite(LED_BUILTIN, LOW);
}

// Full boot sequence
void BootManager::runBoot(const char* ssid, const char* password) {
    Serial.begin(115200);

    // TFT & Touchscreen init
    pinMode(PB_TFT_CS, OUTPUT);
    pinMode(PB_TOUCH_CS, OUTPUT);
    digitalWrite(PB_TFT_CS, HIGH);
    digitalWrite(PB_TOUCH_CS, HIGH);

    ui.initTFT();

    ui.printToTFT("System: Starting PicoBoyOS v0.1", 2000);
    ui.printToTFT("System: Booting as new...", 2000);
    ui.printToTFT("System: Checking vitals...", 2000);

    // Sensor init & temperature read
    sensors.begin();
    sensors.getTemperature();
    delay(400);

    // Network init
    ui.printToTFT("System: Starting network...", 2000);
    net.begin(ssid, password, PB_WIFI_TIMEOUT_MS);

    // Wait for network (or timeout)
    unsigned long start = millis();
    while (!net.isFinished() && millis() - start < PB_WIFI_TIMEOUT_MS + 1000) {
        net.update();
        delay(10);
    }

    if (net.isConnected()) {
        ui.printToTFT("System: WiFi connected!", 1000);
    } else {
        ui.printToTFT("System: Running offline mode.", 1000);
    }

    ui.printToTFT("System: Boot complete. Starting UI & Input...", 2500);

    // LED blink
    for (int i = 0; i < 4; i++) {
        blinkLED(PB_BOOT_BLINK_INTERVAL);
        delay(100);
    }

    // Start input & register callbacks
    input.begin();
    input.onEncoderClockwise([&]() { ui.selectNextMenuItem(); });
    input.onEncoderCounterClockwise([&]() { ui.selectPreviousMenuItem(); });
    input.onButtonPress([&]() { ui.executeMenuItem(); });

    // Start UI
    ui.startUI();
}
