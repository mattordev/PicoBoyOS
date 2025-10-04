#ifndef UI_MANAGER_H
#define UI_MANAGER_H

#include <TFT_eSPI.h> // Hardware-specific library

class UIManager {
private:
    TFT_eSPI& tft; // Reference to the TFT display object

    static const int TEXT_HEIGHT = 16;
    static const int YMAX = 320;
    static const int XMAX = 480;
    static const int HEADER_HEIGHT = 40;
    static const int SIDEBAR_WIDTH = 80;
    static const int MAIN_AREA_WIDTH = XMAX - SIDEBAR_WIDTH;
    static const int MAIN_AREA_HEIGHT = YMAX - HEADER_HEIGHT;
    static const int MESSAGE_AREA_HEIGHT = 40;

    // Menu items
    const char* menuItems[5] = {"Status", "Items", "Data", "Radio", "Settings"};
    int selectedMenuItem = 0;
    int totalMenuItems = sizeof(menuItems) / sizeof(menuItems[0]);

    void blinkLED(int delayTime);

public:
    UIManager(TFT_eSPI& display); // constructor

    // Initialize TFT hardware and clear screen
    void initTFT();

    // Print messages with optional delay
    void printToTFT(String message, int delayTime = 0, int textSize = 1);

    // Start UI and draw layout
    void startUI();
    void drawSidebarMenu();
    void drawMainContent();

    // Menu navigation
    void selectNextMenuItem();
    void selectPreviousMenuItem();
    void executeMenuItem();

    // Handle touchscreen input
    void handleTouch(int x, int y);
};

#endif
