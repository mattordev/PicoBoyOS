// picoboy OS v0.1

// Include the libraries
#include <WiFi.h>
#include <SPI.h>
#include <XPT2046_Touchscreen.h>

#include "secrets.h"  
#include "main.h"
#include "ui/UIManager.h"
#include "input/InputManager.h"
#include "net/NetworkManager.h"
#include "sys/SensorManager.h"



// Hardware Vars
TFT_eSPI tft = TFT_eSPI();
UIManager ui(tft); // Create an instance of UIManager
InputManager input(12, 13, 14);
NetworkManager net(ui); // Pass UI reference and optional timeout
SensorManager sensors(ui);


#define TOUCH_CS 22 // Chip select pin (T_CS) of touch screen
XPT2046_Touchscreen ts(TOUCH_CS); // Initialize the touch screen with the CS pin


// TFT Pins
#define TFT_CS 17  

void setup() {
  // Start boot checks, power screen, check temperatures, log start up date
  s_boot();
}

void blinkLED(int delayTime) {
  // Initialize digital pin LED_BUILTIN as an output. Blink to show power up.
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(delayTime);
  digitalWrite(LED_BUILTIN, LOW);
}

void s_boot() {
    Serial.begin(115200);

    // SPI devices deselect
    pinMode(TFT_CS, OUTPUT);
    pinMode(TOUCH_CS, OUTPUT);
    digitalWrite(TFT_CS, HIGH);
    digitalWrite(TOUCH_CS, HIGH);

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
    net.begin(ssid, password, 20000);

    // Wait for network result (or timeout) before starting UI
    unsigned long start = millis();
    while (!net.isFinished() && millis() - start < 21000) { // slight buffer over timeout
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
        blinkLED(100);
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
  // call input to update encoder and button states
  input.update();
  // call network to update connection state
  net.update();
}