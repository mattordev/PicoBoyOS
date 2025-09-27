#ifndef MAIN_H
#define MAIN_H

// Function prototypes
void printToTFT(String message, int delayTime, int textSize = 1);
void blinkLED(int delayTime);
void s_boot();
bool setup_wifi();
void get_temp();
void startUI();
void drawSidebarMenu();
void drawMainContent();
void selectNextMenuItem();
void selectPreviousMenuItem();
void executeMenuItem();
void handleTouch(int x, int y);

#endif // MAIN_H
