#include "InputManager.h"
#include <Arduino.h>

#define ENCODER_DO_NOT_USE_INTERRUPTS

InputManager::InputManager(int pinA, int pinB, int buttonPin)
    : pinA(pinA), pinB(pinB), buttonPin(buttonPin) {
    encoder = new Encoder(pinA, pinB);
}

void InputManager::begin() {
    pinMode(pinA, INPUT_PULLUP);
    pinMode(pinB, INPUT_PULLUP);
    pinMode(buttonPin, INPUT_PULLUP);
}

void InputManager::update() {
    unsigned long now = millis();

    // ---- Encoder check ----
    if (now - lastEncoderCheck >= encoderInterval) {
        long newPosition = encoder->read() / 4;

        if (newPosition == stablePosition) {
            if (stablePosition != lastReportedPosition) {
                if (stablePosition > lastReportedPosition) {
                    if (encoderCWCallback) encoderCWCallback();
                } else {
                    if (encoderCCWCallback) encoderCCWCallback();
                }
                lastReportedPosition = stablePosition;
            }
        }
        stablePosition = newPosition;
        lastEncoderCheck = now;
    }

    // ---- Button check ----
    if (now - lastButtonCheck >= buttonInterval) {
        bool currentButtonState = digitalRead(buttonPin);
        if (currentButtonState == LOW && previousButtonState == HIGH) {
            if (now - lastButtonPress > debounceDelay) {
                if (buttonPressCallback) buttonPressCallback();
                lastButtonPress = now;
            }
        }
        previousButtonState = currentButtonState;
        lastButtonCheck = now;
    }
}

// ---- Callback setters ----
void InputManager::onEncoderClockwise(std::function<void()> cb) {
    encoderCWCallback = cb;
}

void InputManager::onEncoderCounterClockwise(std::function<void()> cb) {
    encoderCCWCallback = cb;
}

void InputManager::onButtonPress(std::function<void()> cb) {
    buttonPressCallback = cb;
}