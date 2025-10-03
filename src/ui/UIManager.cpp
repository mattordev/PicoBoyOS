#include <Arduino.h>
#include "UIManager.h"

UIManager::UIManager(TFT_eSPI& display) : tft(display) {} // Constructor implementation

void UIManager::printToTFT(String message, int delayTime, int textSize) {
    static String lines[YMAX / TEXT_HEIGHT];

    Serial.println(message);

    // Shift existing lines up
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

    //blinkLED(1);
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

    if (selectedMenuItem == 0) {
        tft.setCursor(SIDEBAR_WIDTH + 10, HEADER_HEIGHT + 10);
        tft.print("Status Screen");
    } else if (selectedMenuItem == 1) {
        tft.setCursor(SIDEBAR_WIDTH + 10, HEADER_HEIGHT + 10);
        tft.print("Items Screen");
    } else if (selectedMenuItem == 2) {
        tft.setCursor(SIDEBAR_WIDTH + 10, HEADER_HEIGHT + 10);
        tft.print("Data Screen");
    } else if (selectedMenuItem == 3) {
        tft.setCursor(SIDEBAR_WIDTH + 10, HEADER_HEIGHT + 10);
        tft.print("Radio Screen");
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

void UIManager::handleTouch(int x, int y){
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
