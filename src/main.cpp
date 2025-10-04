// picoboy OS v0.1

#include <WiFi.h>
#include <SPI.h>
#include <XPT2046_Touchscreen.h>

#include "secrets.h"
#include "sys/PBOS_Config.h" // Configuration constants
#include "main.h"
#include "boot/BootManager.h"
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
BootManager boot(ui, input, net, sensors);

// Touchscreen
XPT2046_Touchscreen ts(PB_TOUCH_CS);

void setup() {
    boot.runBoot(ssid, password);
}

void blinkLED(int delayTime) {
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(delayTime);
    digitalWrite(LED_BUILTIN, LOW);
}



void loop() {
    input.update();
    net.update();
}
