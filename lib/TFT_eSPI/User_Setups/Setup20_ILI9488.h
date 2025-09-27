// See SetupX_Template.h for all options available
#define USER_SETUP_ID 20

#define ILI9488_DRIVER

// For NodeMCU - use pin numbers in the form PIN_Dx where Dx is the NodeMCU pin designation 
#define TFT_MISO PIN_D50 // (leave TFT SDO disconnected if other SPI devices share MISO) 
#define TFT_MOSI PIN_D51
#define TFT_SCLK PIN_D52
#define TFT_CS PIN_D53 // Chip select control pin D8
#define TFT_DC PIN_D9 // Data Command control pin
#define TFT_RST PIN_D8 // Reset pin (could connect to NodeMCU RST, see next line) 
//#define TFT_RST -1 // Set TFT RST to -1 if the display RESET is connected to NodeMCU RST or 3.3V

#define TOUCH_CS PIN_D7

// Chip select pin (T_CS) of touch screen
#define LOAD_GLCD // Font 1. Original Adafruit 8 pixel font needs ~1820 bytes in FLASH 
#define LOAD_FONT2 // Font 2. Small 16 pixel high font, needs ~3534 bytes in FLASH, 96 characters 
#define LOAD_FONT4 // Font 4. Medium 26 pixel high font, needs ~5848 bytes in FLASH, 96 characters 
#define LOAD_FONT6 // Font 6. Large 48 pixel font, needs ~2666 bytes in FLASH, only characters 1234567890:-.apm 
#define LOAD_FONT7 // Font 7. 7 segment 48 pixel font, needs ~2438 bytes in FLASH, only characters 1234567890:. 
#define LOAD_FONTS // Font 8. Large 75 pixel font needs ~3256 bytes in FLASH, only characters 1234567890:-. 
#define LOAD_GFXFF // FreeFonts. Include access to the 48 Adafruit_GFX free fonts FF1 to FF48 and custom fonts
#define SMOOTH_FONT
#define USE_HSPI_PORT

// #define SPI_FREQUENCY 20000000
// #define SPI_FREQUENCY 27000000
// #define SPI_FREQUENCY 40000000 
#define SPI_FREQUENCY 75000000 
// #define SPI FREQUENCY 80000000

// Optional reduced SPI frequency for reading TFT 
#define SPI_READ_FREQUENCY 16000000

#define SPI_TOUCH_FREQUENCY 2500000