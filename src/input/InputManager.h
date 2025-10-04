#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#include <Encoder.h>
#include <functional>

class InputManager {
public:
    InputManager(int pinA, int pinB, int buttonPin);

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
    const unsigned long debounceDelay = 50;

    unsigned long lastEncoderCheck = 0;
    unsigned long encoderInterval = 10; // ms
    unsigned long lastButtonCheck = 0;
    unsigned long buttonInterval = 10; // ms

    // Callbacks
    std::function<void()> encoderCWCallback;
    std::function<void()> encoderCCWCallback;
    std::function<void()> buttonPressCallback;
};

#endif