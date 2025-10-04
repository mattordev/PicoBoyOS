// picoboy OS v0.1

#include <WiFi.h>
#include <SPI.h>
#include <XPT2046_Touchscreen.h>

#include "secrets.h"
#include "sys/PBOS_Config.h" // Configuration constants
#include "main.h"
#include "ui/UIManager.h"
#include "input/InputManager.h"
#include "net/NetworkManager.h"
#include "sys/SensorManager.h"

// Hardware Vars
TFT_eSPI tft = TFT_eSPI();
UIManager ui(tft);
InputManager input(PB_ENCODER_PIN_A, PB_ENCODER_PIN_B, PB_ENCODER_BUTTON_PIN);
NetworkManager net(ui);
SensorManager sensors(ui);

// Touchscreen
XPT2046_Touchscreen ts(PB_TOUCH_CS);

void setup() {
    s_boot();
}

void blinkLED(int delayTime) {
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(delayTime);
    digitalWrite(LED_BUILTIN, LOW);
}

void s_boot() {
    Serial.begin(115200);

    // SPI devices deselect
    pinMode(PB_TFT_CS, OUTPUT);
    pinMode(PB_TOUCH_CS, OUTPUT);
    digitalWrite(PB_TFT_CS, HIGH);
    digitalWrite(PB_TOUCH_CS, HIGH);

    // TFT setup
    tft.init();
    tft.setRotation(1);
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);

    // Touchscreen init
    ts.begin();
    ts.setRotation(1);

    ui.printToTFT("System: Starting PicoBoyOS v0.1", 2000);
    ui.printToTFT("System: Booting as new...", 2000);
    ui.printToTFT("System: Checking vitals...", 2000);

    // Start SensorManager
    sensors.begin();
    sensors.getTemperature();
    delay(400);

    ui.printToTFT("System: Starting network...", 2000);
    net.begin(ssid, password, PB_WIFI_TIMEOUT_MS);

    // Wait for network result (or timeout) before starting UI
    unsigned long start = millis();
    while (!net.isFinished() && millis() - start < PB_WIFI_TIMEOUT_MS + 1000) {
        net.update();
        delay(10); // tiny delay to allow TFT prints to render
    }

    if (net.isConnected()) {
        ui.printToTFT("System: WiFi connected!", 1000);
    } else {
        ui.printToTFT("System: Running offline mode.", 1000);
    }

    ui.printToTFT("System: Boot complete. Starting UI & Input", 2500);

    // Blink LED to indicate boot complete
    for (int i = 0; i < 4; i++) {
        blinkLED(PB_BOOT_BLINK_INTERVAL);
        delay(100);
    }

    // Start UI and Input
    ui.startUI();
    input.begin();

    // Register input callbacks
    input.onEncoderClockwise([&](){
        Serial.println("Encoder CW → Next menu item");
        ui.selectNextMenuItem();
    });

    input.onEncoderCounterClockwise([&](){
        Serial.println("Encoder CCW → Previous menu item");
        ui.selectPreviousMenuItem();
    });

    input.onButtonPress([&](){
        Serial.println("Encoder Button → Execute menu item");
        ui.executeMenuItem();
    });
}

void loop() {
    input.update();
    net.update();
}
