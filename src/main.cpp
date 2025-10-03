// picoboy OS v0.1

// Include the libraries
#include <WiFi.h>
#include <SPI.h>
#include <XPT2046_Touchscreen.h>

#define ENCODER_DO_NOT_USE_INTERRUPTS
#include <Encoder.h>
#include "main.h"
#include "ui/UIManager.h"

// Encoder Pins
#define ENCODER_PIN_A      12    // EC11 phase A
#define ENCODER_PIN_B      13    // EC11 phase B
#define ENCODER_BUTTON_PIN 14    // EC11 push-button
bool previousButtonState = HIGH; // Initialize the previous button state as released

// Hardware Vars
TFT_eSPI tft = TFT_eSPI();
UIManager ui(tft); // Create an instance of UIManager
#define TOUCH_CS 22 // Chip select pin (T_CS) of touch screen
XPT2046_Touchscreen ts(TOUCH_CS); // Initialize the touch screen with the CS pin
Encoder* myEnc; // Initialize the encoder

// Wifi Vars
#include "secrets.h"
int wifiTimeout = 20000; // timeout in milliseconds
bool offline = false;

// TFT Pins
#define TFT_CS 17  


// Menu items
const char* menuItems[] = {"Status", "Items", "Data", "Radio"};
int selectedMenuItem = 0;
int totalMenuItems = sizeof(menuItems) / sizeof(menuItems[0]);

// Debounce variables
unsigned long lastButtonPress = 0;
const unsigned long debounceDelay = 50; // milliseconds

// Encoder position
long oldPosition = -999;

void setup() {
  pinMode(ENCODER_PIN_A, INPUT_PULLUP);
  pinMode(ENCODER_PIN_B, INPUT_PULLUP);
  pinMode(ENCODER_BUTTON_PIN, INPUT_PULLUP);

  myEnc = new Encoder(ENCODER_PIN_A, ENCODER_PIN_B);

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

  myEnc = new Encoder (12, 13);
  pinMode(ENCODER_BUTTON_PIN, INPUT_PULLUP);

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

  ui.printToTFT("System: Boot complete. Starting UI...", 2500);

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

// Non-blocking timing variables
unsigned long lastBlink = 0;
unsigned long blinkInterval = 100; // ms
unsigned long lastEncoderCheck = 0;
unsigned long encoderInterval = 10; // ms
unsigned long lastButtonCheck = 0;
unsigned long buttonInterval = 10; // ms

void loop() {
  unsigned long now = millis();

  // Non-blocking encoder check
  if (now - lastEncoderCheck >= encoderInterval) {
    long newPosition = myEnc->read()/4;
    Serial.print("Raw encoder value: ");
    Serial.println(newPosition);
    if (newPosition != oldPosition) {
      Serial.print("Encoder position changed: ");
      Serial.print(oldPosition);
      Serial.print(" -> ");
      Serial.println(newPosition);
      if (newPosition > oldPosition) {
        Serial.println("Menu: Next item");
        ui.selectNextMenuItem();
      } else {
        Serial.println("Menu: Previous item");
        ui.selectPreviousMenuItem();
      }
      oldPosition = newPosition;
    }
    lastEncoderCheck = now;
  }

  // Non-blocking button check
  if (now - lastButtonCheck >= buttonInterval) {
    bool currentButtonState = digitalRead(ENCODER_BUTTON_PIN);
    if (currentButtonState == LOW && previousButtonState == HIGH) {
      Serial.println("Encoder button pressed");
      if (now - lastButtonPress > debounceDelay) {
        ui.executeMenuItem();
        lastButtonPress = now;
      }
    }
    previousButtonState = currentButtonState;
    lastButtonCheck = now;
  }
}
