// picoboy OS v0.1

// Include the libraries
#include <WiFi.h>
#include <SPI.h>
#include <XPT2046_Touchscreen.h>

#include "main.h"
#include "ui/UIManager.h"
#include "input/InputManager.h"


// Hardware Vars
TFT_eSPI tft = TFT_eSPI();
UIManager ui(tft); // Create an instance of UIManager
InputManager input(12, 13, 14);
#define TOUCH_CS 22 // Chip select pin (T_CS) of touch screen
XPT2046_Touchscreen ts(TOUCH_CS); // Initialize the touch screen with the CS pin

// Wifi Vars
#include "secrets.h"
int wifiTimeout = 20000; // timeout in milliseconds
bool offline = false;

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
  // Start serial
  Serial.begin(115200);

  

  // Deselect all SPI devices
  pinMode(TFT_CS, OUTPUT);
  pinMode(TOUCH_CS, OUTPUT);
  digitalWrite(TFT_CS, HIGH);
  digitalWrite(TOUCH_CS, HIGH);

  // Start TFT
  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);

  // Initialize the touch screen
  ts.begin();
  ts.setRotation(1);

  ui.printToTFT("System: Starting PicoBoyOS v0.1", 2000); // Add a delay of 2000 milliseconds
  ui.printToTFT("System: Booting as new...", 2000); // Add a delay of 2000 milliseconds
  ui.printToTFT("System: Checking vitals...", 2000);
  get_temp();
  delay(400);

  ui.printToTFT("System: Checking active data stream...", 2000);
  // Check WiFi connection, connect if possible.
  offline = setup_wifi();

  ui.printToTFT("System: Boot complete. Starting UI & Input", 2500);

  // Blink LED to indicate boot complete
  blinkLED(100);
  delay(100);
  blinkLED(100);
  delay(100);
  blinkLED(100);
  delay(100);
  blinkLED(100);


  // Start the UI
  ui.startUI();

  // Start Input Manager
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

bool setup_wifi() {
  // Operate in WiFi station mode
  WiFi.mode(WIFI_STA);

  WiFi.setTimeout(wifiTimeout);
  WiFi.begin(ssid, password);

  if (WiFi.waitForConnectResult() == WL_CONNECTED) {
    // Connection established
    ui.printToTFT("System: PicoBoy is connected to WiFi network " + String(WiFi.SSID()), 2000); 

    // Print IP Address
    ui.printToTFT("System: Assigned IP Address: " + WiFi.localIP().toString(), 2000); 
    return true;
  } else {
    // No connection established
    ui.printToTFT("System: PicoBoy is NOT connected to a WiFi network. Running in offline mode.", 2000); 
    return false;
  }
}

void get_temp() {
  float tempC;

  while (true) {
    // Read the internal temperature
    tempC = analogReadTemp(); // Get internal temperature

    // Print temperature readings
    ui.printToTFT("System: Temperature (ºC) = " + String(tempC), 2000); 

    // Check if the temperature is within a reasonable range
    if (tempC > -20.0 && tempC < 150.0) {
      break; // Exit the loop if the temperature is valid
    }
  }
}


void loop() {
  // call input to update encoder and button states
  input.update();
}