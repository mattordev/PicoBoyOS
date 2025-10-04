#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#include "../sys/PBOS_Config.h"

#include <Encoder.h>
#include <functional>

class InputManager {
public:
    InputManager(int pinA = PB_ENCODER_PIN_A, int pinB = PB_ENCODER_PIN_B, int buttonPin = PB_ENCODER_BUTTON_PIN);

    void begin();
    void update();

    // Callback setters
    void onEncoderClockwise(std::function<void()> cb);
    void onEncoderCounterClockwise(std::function<void()> cb);
    void onButtonPress(std::function<void()> cb);

private:
    int pinA;
    int pinB;
    int buttonPin;

    Encoder* encoder;
    long stablePosition = 0;
    long lastReportedPosition = 0;

    bool previousButtonState = HIGH;
    unsigned long lastButtonPress = 0;
    const unsigned long debounceDelay = PB_DEBOUNCE_DELAY;

    unsigned long lastEncoderCheck = 0;
    unsigned long encoderInterval = PB_ENCODER_INTERVAL; // ms
    unsigned long lastButtonCheck = 0;
    unsigned long buttonInterval = PB_BUTTON_INTERVAL; // ms

    // Callbacks
    std::function<void()> encoderCWCallback;
    std::function<void()> encoderCCWCallback;
    std::function<void()> buttonPressCallback;
};

#endif
