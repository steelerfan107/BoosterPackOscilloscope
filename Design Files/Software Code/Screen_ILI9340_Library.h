///
/// @file       Screen_ILI9340_Library.h
/// @brief      Library header
/// @details ILI9225 screen library
/// @n
/// @n @b       Project LCD_screen Library Suite
/// @n @a       Developed with [embedXcode](http://embedXcode.weebly.com)
///
/// @author     Rei VILO
/// @author     http://embedXcode.weebly.com
/// @date       Feb 18, 2014
/// @version 101
///
/// @copyright (c) Rei VILO, 2013
/// @copyright Attribution-NonCommercial-ShareAlike 3.0 Unported (CC BY-NC-SA 3.0)
///
/// @n  Single license: Attribution-NonCommercial-ShareAlike 3.0 Unported (CC BY-NC-SA 3.0)
///
/// @see
/// *  Rob's Color LCD Booster Pack (320x240 with microSD card socket) in Store
/// @n  https://www.tindie.com/products/RobG/color-lcd-booster-pack-320x240-with-microsd-card-socket/
/// *  Rob's (Universal) Color LCD graphics library (2) Posted 12 September 2013 - 01:41 PM
/// @n  http://forum.43oh.com/topic/4352-universal-color-lcd-graphics-library-2/#entry39090
/// *   Fonts generated with MikroElektronika GLCD Font Creator 1.2.0.0
#if defined(MPIDE)
#include "WProgram.h"
#elif defined(DIGISPARK)
#include "Arduino.h"
#elif defined(ENERGIA)
#include "Energia.h"
#elif defined(MAPLE_IDE)
#include "WProgram.h"
#elif defined(CORE_TEENSY)
#include "WProgram.h"
#elif defined(WIRING)
#include "Wiring.h"
#elif defined(ARDUINO)
#include "Arduino.h"
#endif
#ifndef SCREEN_ILI9340_LIBRARY_RELEASE
#define SCREEN_ILI9340_LIBRARY_RELEASE 101
#define MAX_FONT_SIZE 1
#if (MAX_FONT_SIZE > 0)
#include "Terminal6e.h"
#if (MAX_FONT_SIZE > 1)
#include "Terminal8e.h"
#endif
#endif
#include "SPI.h"
#define colour_black    0b0000000000000000
#define colour_white    0b1111111111111111
#define colour_red      0b1111100000000000
#define colour_green    0b0000011111100000
#define colour_blue     0b0000000000011111
#define colour_yellow   0b1111111111100000
#define colour_cyan     0b0000011111111111
#define colour_orange   0b1111101111100000
#define colour_magenta  0b1111100000001111
#define colour_violet   0b1111100000011111
#define colour_grey     0b0111101111101111
#define colour_darkGrey 0b0011100111100111
#define colour_brown    0b0111100000000000
#define _pinScreenCS 2
#define _pinSDcardCS 13
#define _pinScreenReset 20
#define _pinScreenDataCommand 5
class Screen_ILI9340 {
public:
    Screen_ILI9340();
    void begin();
    String WhoAmI();
 
 
 
    void clear(uint16_t colour = colour_black);
 
 
 
 
    void setOrientation(uint8_t orientation);
    uint8_t getOrientation();
 
    uint16_t screenSizeX();
    uint16_t screenSizeY();
 
    uint16_t screenDiagonal();
    void circle(uint16_t x0, uint16_t y0, uint16_t radius, uint16_t colour);
    void line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t colour);
    void dLine(uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy, uint16_t colour);
    void setPenSolid(bool flag = true);
 
 
 
 
 
 
 
 
    void triangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, uint16_t colour);
    void rectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t colour);
    void dRectangle(uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy, uint16_t colour);
    void point(uint16_t x1, uint16_t y1, uint16_t colour);
 
 
 
 
 
    void setFontSize(uint8_t size);
 
 
 
 
 
    void setFontSolid(bool flag = true);
 
 
 
 
    uint8_t fontSizeX();
 
 
 
 
    uint8_t fontSizeY();
 
 
 
    uint8_t fontMax();
 
 
 
 
 
 
 
 
 
 
    void gText(uint16_t x0, uint16_t y0,
               String s,
               uint16_t textColour = colour_white, uint16_t backColour = colour_black,
               uint8_t ix = 1, uint8_t iy = 1);
 
    uint16_t readPixel(uint16_t x1, uint16_t y1);
 
    uint16_t calculate(uint8_t red, uint8_t green, uint8_t blue);
private:
    uint8_t      _fontX, _fontY, _fontSize;
    uint8_t      _orientation;
    bool         _penSolid, _fontSolid;
    uint16_t     _screenWidth, _screenHeigth, _screenDiagonal;
 
    void _setOrientation(uint8_t orientation);
 
    void _setWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
    void _closeWindow();
 void _setPoint(uint16_t x1, uint16_t y1, uint16_t colour);
    void _fastFill(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t colour);
    void _writeData88(uint8_t dataHigh8, uint8_t dataLow8);;
    void _writeData8(uint8_t data8);
    void _writeData16(uint16_t data16);
    void _writeRegister8(uint8_t command8, uint8_t data8);
    void _writeRegister16(uint8_t command8, uint16_t data16);
    void _writeCommand8(uint8_t command8);
    void         _triangleArea(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, uint16_t colour);
    uint8_t _getCharacter(uint8_t c, uint8_t i);
 
 void         _swap(int16_t &a, int16_t &b);
 void         _swap(uint16_t &a, uint16_t &b);
 uint16_t     _check(uint16_t x0, uint16_t xmin, uint16_t xmax);
};
#endif
