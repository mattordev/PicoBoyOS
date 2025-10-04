#include "UIManager.h"
#include <Arduino.h>

UIManager::UIManager(TFT_eSPI& display) : tft(display) {} // Constructor

// Initialize TFT hardware
void UIManager::initTFT() {
    tft.init();
    tft.setRotation(1);
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
}

void UIManager::printToTFT(String message, int delayTime, int textSize) {
    static String lines[YMAX / TEXT_HEIGHT];

    Serial.println(message);

    // Shift lines up
    for (int i = 0; i < (YMAX / TEXT_HEIGHT) - 1; i++) {
        lines[i] = lines[i + 1];
    }
    lines[(YMAX / TEXT_HEIGHT) - 1] = message;

    // Clear screen and redraw
    tft.fillScreen(TFT_BLACK);
    tft.setTextSize(textSize);
    for (int i = 0; i < (YMAX / TEXT_HEIGHT); i++) {
        tft.setCursor(0, i * TEXT_HEIGHT);
        tft.print(lines[i]);
    }

    delay(delayTime);
}

void UIManager::startUI() {
    tft.fillRect(0, 0, XMAX, HEADER_HEIGHT, TFT_DARKGREEN);
    tft.setCursor(10, 10);
    tft.setTextColor(TFT_WHITE, TFT_DARKGREEN);
    tft.setTextSize(2);
    tft.print("PicoBoyOS v0.1");

    drawSidebarMenu();
    drawMainContent();
}

void UIManager::drawSidebarMenu() {
    tft.fillRect(0, HEADER_HEIGHT, SIDEBAR_WIDTH, YMAX - HEADER_HEIGHT, TFT_DARKGREY);
    tft.setTextSize(2);

    for (int i = 0; i < totalMenuItems; i++) {
        tft.setCursor(10, HEADER_HEIGHT + i * TEXT_HEIGHT + 10);
        if (i == selectedMenuItem) {
            tft.setTextColor(TFT_BLACK, TFT_WHITE);
        } else {
            tft.setTextColor(TFT_WHITE, TFT_DARKGREY);
        }
        tft.print(menuItems[i]);
    }
}

void UIManager::drawMainContent() {
    tft.fillRect(SIDEBAR_WIDTH, HEADER_HEIGHT, MAIN_AREA_WIDTH, MAIN_AREA_HEIGHT, TFT_BLACK);
    tft.setTextSize(2);
    tft.setTextColor(TFT_WHITE);

    tft.setCursor(SIDEBAR_WIDTH + 10, HEADER_HEIGHT + 10);
    switch (selectedMenuItem) {
        case 0: tft.print("Status Screen"); break;
        case 1: tft.print("Items Screen"); break;
        case 2: tft.print("Data Screen"); break;
        case 3: tft.print("Radio Screen"); break;
        case 4: tft.print("Settings Screen"); break;
    }
}

void UIManager::selectNextMenuItem() {
    selectedMenuItem = (selectedMenuItem + 1) % totalMenuItems;
    drawSidebarMenu();
    drawMainContent();
}

void UIManager::selectPreviousMenuItem() {
    selectedMenuItem = (selectedMenuItem - 1 + totalMenuItems) % totalMenuItems;
    drawSidebarMenu();
    drawMainContent();
}

void UIManager::executeMenuItem() {
    selectNextMenuItem();
}

void UIManager::handleTouch(int x, int y) {
    Serial.print("Touch at: ");
    Serial.print(x);
    Serial.print(", ");
    Serial.println(y);

    if (y < HEADER_HEIGHT) {
        // header touch area
    } else if (x < SIDEBAR_WIDTH) {
        int touchedItem = (y - HEADER_HEIGHT) / TEXT_HEIGHT;
        if (touchedItem >= 0 && touchedItem < totalMenuItems) {
            selectedMenuItem = touchedItem;
            drawSidebarMenu();
            drawMainContent();
        }
    } else {
        // main content area
    }
}
