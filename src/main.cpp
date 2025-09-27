// picoboy OS v0.1

// Include the libraries
#include <WiFi.h>
#include <SPI.h>
#include <TFT_eSPI.h>
#include <XPT2046_Touchscreen.h>

#include <Encoder.h>
#include "main.h"

// Encoder Pins
#define ENCODER_PIN_A 12
#define ENCODER_PIN_B 13
#define ENCODER_BUTTON_PIN 14
bool previousButtonState = HIGH; // Initialize the previous button state as released

// Hardware Vars
TFT_eSPI tft = TFT_eSPI();
XPT2046_Touchscreen ts(TOUCH_CS); // Initialize the touch screen with the CS pin
Encoder* myEnc; // Initialize the encoder

// Wifi Vars
const char* ssid = "SKYNEQLU";
const char* password = "XwVw41rr7Tg8";
int wifiTimeout = 20000; // timeout in milliseconds
bool offline = false;

// TFT Pins
#define TFT_CS 17  
#define TOUCH_CS 22 // Chip select pin (T_CS) of touch screen



// TFT display constants
#define TEXT_HEIGHT 16 // Height of text to be printed
#define YMAX 320 // Height of the display in pixels
#define XMAX 480 // Width of the display in pixels
#define MAX_LINES (YMAX / TEXT_HEIGHT) // Maximum number of lines that can be displayed
#define HEADER_HEIGHT 40
#define SIDEBAR_WIDTH 80
#define MAIN_AREA_WIDTH (XMAX - SIDEBAR_WIDTH)
#define MAIN_AREA_HEIGHT (YMAX - HEADER_HEIGHT)
#define MESSAGE_AREA_HEIGHT 40

// Menu items
const char* menuItems[] = {"Status", "Items", "Data", "Radio"};
int selectedMenuItem = 0;
int totalMenuItems = sizeof(menuItems) / sizeof(menuItems[0]);

// Debounce variables
unsigned long lastButtonPress = 0;
const unsigned long debounceDelay = 50; // milliseconds

// Encoder position
long oldPosition = -999;

// Custom print function to handle both Serial and TFT output with a delay and optional text size
void printToTFT(String message, int delayTime, int textSize) {
  static int lineCount = 0; // Counter for the current line
  static String lines[MAX_LINES]; // Array to store the lines
  
  Serial.println(message);

  // Shift existing lines up
  for (int i = 0; i < MAX_LINES - 1; i++) {
    lines[i] = lines[i + 1];
  }

  // Add the new line
  lines[MAX_LINES - 1] = message;

  // Clear screen
  tft.fillScreen(TFT_BLACK);

  // Set text size globally
  tft.setTextSize(textSize);

  // Print lines to TFT
  for (int i = 0; i < MAX_LINES; i++) {
    tft.setCursor(0, i * TEXT_HEIGHT); // Set cursor position
    tft.print(lines[i]); // Print line
  }

  blinkLED(1);

  // Add a delay
  delay(delayTime);
}

void setup() {
  pinMode(ENCODER_BUTTON_PIN, INPUT_PULLUP);

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
  Serial.begin(1155200);

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

  printToTFT("System: Starting PicoBoyOS v0.1", 2000); // Add a delay of 2000 milliseconds
  printToTFT("System: Booting as new...", 2000); // Add a delay of 2000 milliseconds
  printToTFT("System: Checking vitals...", 2000); 
  get_temp();
  delay(400);

  printToTFT("System: Checking active data stream...", 2000); 
  // Check WiFi connection, connect if possible.
  offline = setup_wifi();

  printToTFT("System: Boot complete. Starting UI...", 2500);

  // Blink LED to indicate boot complete
  blinkLED(100);
  delay(100);
  blinkLED(100);
  delay(100);
  blinkLED(100);
  delay(100);
  blinkLED(100);


  // Start the UI
  startUI();
}

bool setup_wifi() {
  // Operate in WiFi station mode
  WiFi.mode(WIFI_STA);

  WiFi.setTimeout(wifiTimeout);
  WiFi.begin(ssid, password);

  if (WiFi.waitForConnectResult() == WL_CONNECTED) {
    // Connection established
    printToTFT("System: PicoBoy is connected to WiFi network " + String(WiFi.SSID()), 2000); 

    // Print IP Address
    printToTFT("System: Assigned IP Address: " + WiFi.localIP().toString(), 2000); 
    return true;
  } else {
    // No connection established
    printToTFT("System: PicoBoy is NOT connected to a WiFi network. Running in offline mode.", 2000); 
    return false;
  }
}

void get_temp() {
  float tempC;

  while (true) {
    // Read the internal temperature
    tempC = analogReadTemp(); // Get internal temperature

    // Print temperature readings
    printToTFT("System: Temperature (ºC) = " + String(tempC), 2000); 

    // Check if the temperature is within a reasonable range
    if (tempC > -20.0 && tempC < 150.0) {
      break; // Exit the loop if the temperature is valid
    }
  }
}

void loop() {
  // Read the current position of the encoder
  long newPosition = myEnc->read() / 4; // Divide by 4 if using quadrature encoder

  if (newPosition != oldPosition) {
    if (newPosition > oldPosition) {
      selectNextMenuItem();
    } else {
      selectPreviousMenuItem();
    }
    oldPosition = newPosition;
  }

  bool currentButtonState = digitalRead(ENCODER_BUTTON_PIN);

  // Check if the button state has changed from released to pressed
  if (currentButtonState == LOW && previousButtonState == HIGH) {
    unsigned long currentTime = millis();
    if (currentTime - lastButtonPress > debounceDelay) {
      // Button is pressed
      executeMenuItem();
      lastButtonPress = currentTime;
    }
  }

  // Update the previous button state
  previousButtonState = currentButtonState;
}

void startUI() {
  // Draw the header
  tft.fillRect(0, 0, XMAX, HEADER_HEIGHT, TFT_DARKGREEN);
  tft.setCursor(10, 10);
  tft.setTextColor(TFT_WHITE, TFT_DARKGREEN);
  tft.setTextSize(2);
  tft.print("PicoBoyOS v0.1");

  // Draw the sidebar menu
  drawSidebarMenu();

  // Draw the main content area
  drawMainContent();
}

void drawSidebarMenu() {
  tft.fillRect(0, HEADER_HEIGHT, SIDEBAR_WIDTH, YMAX - HEADER_HEIGHT, TFT_DARKGREY);
  tft.setTextColor(TFT_WHITE, TFT_DARKGREY);
  tft.setTextSize(2);

  for (int i = 0; i < totalMenuItems; i++) {
    tft.setCursor(10, HEADER_HEIGHT + i * TEXT_HEIGHT + 10);
    if (i == selectedMenuItem) {
      tft.setTextColor(TFT_BLACK, TFT_WHITE); // Highlight selected item
    } else {
      tft.setTextColor(TFT_WHITE, TFT_DARKGREY);
    }
    tft.print(menuItems[i]);
  }
}

void drawMainContent() {
  tft.fillRect(SIDEBAR_WIDTH, HEADER_HEIGHT, MAIN_AREA_WIDTH, MAIN_AREA_HEIGHT, TFT_BLACK);
  tft.setTextSize(2);
  tft.setTextColor(TFT_WHITE);

  // Display content based on the selected menu item
  if (selectedMenuItem == 0) {
    // Status Screen
    tft.setCursor(SIDEBAR_WIDTH + 10, HEADER_HEIGHT + 10);
    tft.print("Status Screen");

    // Filler content for status screen
    tft.setTextSize(1);
    tft.setCursor(SIDEBAR_WIDTH + 10, HEADER_HEIGHT + 40);
    tft.print("Battery Level: 90%");
    tft.setCursor(SIDEBAR_WIDTH + 10, HEADER_HEIGHT + 60);
    tft.print("CPU Usage: 25%");
    // Add more status details here
  } else if (selectedMenuItem == 1) {
    // Items Screen
    tft.setCursor(SIDEBAR_WIDTH + 10, HEADER_HEIGHT + 10);
    tft.print("Items Screen");

    // Filler content for items screen
    tft.setTextSize(1);
    tft.setCursor(SIDEBAR_WIDTH + 10, HEADER_HEIGHT + 40);
    tft.print("Item 1: 10");
    tft.setCursor(SIDEBAR_WIDTH + 10, HEADER_HEIGHT + 60);
    tft.print("Item 2: 5");
    // Add more item details here
  } else if (selectedMenuItem == 2) {
    // Data Screen
    tft.setCursor(SIDEBAR_WIDTH + 10, HEADER_HEIGHT + 10);
    tft.print("Data Screen");

    // Filler content for data screen
    tft.setTextSize(1);
    tft.setCursor(SIDEBAR_WIDTH + 10, HEADER_HEIGHT + 40);
    tft.print("Data 1: 100");
    tft.setCursor(SIDEBAR_WIDTH + 10, HEADER_HEIGHT + 60);
    tft.print("Data 2: 200");
    // Add more data details here
  } else if (selectedMenuItem == 3) {
    // Radio Screen
    tft.setCursor(SIDEBAR_WIDTH + 10, HEADER_HEIGHT + 10);
    tft.print("Radio Screen");

    // Filler content for radio screen
    tft.setTextSize(1);
    tft.setCursor(SIDEBAR_WIDTH + 10, HEADER_HEIGHT + 40);
    tft.print("Radio Station: 98.5 FM");
    // Add more radio details here
  }
}

void selectNextMenuItem() {
  selectedMenuItem = (selectedMenuItem + 1) % totalMenuItems;
  drawSidebarMenu();
  drawMainContent();
}

void selectPreviousMenuItem() {
  selectedMenuItem = (selectedMenuItem - 1 + totalMenuItems) % totalMenuItems;
  drawSidebarMenu();
  drawMainContent();
}

void executeMenuItem() {
  // Execute action for the selected menu item
  selectNextMenuItem();
}

void handleTouch(int x, int y) {
  // For now, just print the touch coordinates
  Serial.print("Touch at: ");
  Serial.print(x);
  Serial.print(", ");
  Serial.println(y);

  // Add touch handling logic based on coordinates
  if (y < HEADER_HEIGHT) {
    // Handle touches in the header area
  } else if (x < SIDEBAR_WIDTH) {
    // Handle touches in the sidebar menu area
    int touchedItem = (y - HEADER_HEIGHT) / TEXT_HEIGHT;
    if (touchedItem >= 0 && touchedItem < totalMenuItems) {
      selectedMenuItem = touchedItem;
      drawSidebarMenu();
      drawMainContent();
    }
  } else {
    // Handle touches in the main content area
  }
}
