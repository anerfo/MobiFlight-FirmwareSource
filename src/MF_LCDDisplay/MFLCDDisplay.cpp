//
// MFLCDDisplay.cpp
//
// (C) MobiFlight Project 2022
//

#include "MFLCDDisplay.h"
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Fonts/FreeMono12pt7b.h>

#define SCREEN_WIDTH  128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32  // OLED display height, in pixels

#define FONT_HEIGHT 16
#define FONT_WIDTH  12

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library.
// On an arduino UNO:       A4(SDA), A5(SCL)
// On an arduino MEGA 2560: 20(SDA), 21(SCL)
// On an arduino LEONARDO:   2(SDA),  3(SCL), ...
#define OLED_RESET     -1   // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32

#define TCA9548A_I2C_ADDRESS 0x70

MFLCDDisplay::MFLCDDisplay()
    : _lcdDisplay(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET)
{
    _initialized = false;
}

void setTCAChannel(byte i)
{
    Wire.beginTransmission(TCA9548A_I2C_ADDRESS);
    Wire.write(1 << i);
    Wire.endTransmission();
}

void MFLCDDisplay::display(const char *string)
{
    if (!_initialized)
        return;

    setTCAChannel((int)(_address % SCREEN_ADDRESS));
    _lcdDisplay.clearDisplay();
    _lcdDisplay.setTextColor(SSD1306_WHITE);
    // _lcdDisplay.setTextSize(1);
    // _lcdDisplay.setCursor(0, 0);
    // _lcdDisplay.print("ALT");
    // _lcdDisplay.setCursor(0, 16);
    // _lcdDisplay.print("V/S");

    _lcdDisplay.setTextSize(2);
    _lcdDisplay.setCursor(0, 0);
    _lcdDisplay.print(string);
    _lcdDisplay.display();
}

void MFLCDDisplay::attach(byte address, byte cols, byte lines)
{
    _address     = address;
    _cols        = cols;
    _lines       = lines;
    _initialized = true;

    _lcdDisplay.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
    Wire.setClock(400000);
    test();
}

void MFLCDDisplay::detach()
{
    if (!_initialized)
        return;
    _initialized = false;
}

void MFLCDDisplay::powerSavingMode(bool state)
{
    if (state)
        _lcdDisplay.dim(true);
    else
        _lcdDisplay.dim(false);
}

void MFLCDDisplay::test()
{
    display("MobiFlight   Rocks!");
}

void MFLCDDisplay::_printCentered(const char *str, uint8_t line)
{
    uint8_t startCol  = 0;
    uint8_t printChar = _cols;

    if (_cols > strlen(str)) {
        startCol  = (_cols - strlen(str)) / 2;
        printChar = strlen(str);
    }

    _lcdDisplay.setCursor(startCol * FONT_WIDTH, line * FONT_HEIGHT);

    _lcdDisplay.print(str);
}

// MFLCDDisplay.cpp
