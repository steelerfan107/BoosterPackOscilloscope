//
// Screen_ILI9340_Library.cpp
// Library C++ code
// ----------------------------------
// Developed with embedXcode
// http://embedXcode.weebly.com
//
// Project LCD_screen Library Suite
//
// Created by Rei VILO, Feb 18, 2014
// http://embedXcode.weebly.com
//
//
// Copyright    © Rei VILO, 2013-2014
// License      Creative Commons
//              Attribution-NonCommercial-ShareAlike 3.0 Unported (CC BY-NC-SA 3.0)
//
//
// Library header
#include "Screen_ILI9340_Library.h"
///
#define ILINOP     0x00
#define ILISWRESET 0x01
#define ILIRDDID   0x04
#define ILIRDDST   0x09
#define ILISLPIN   0x10
#define ILISLPOUT  0x11
#define ILIPTLON   0x12
#define ILINORON   0x13
#define ILIRDMODE  0x0A
#define ILIRDMADCTL  0x0B
#define ILIRDPIXFMT  0x0C
#define ILIRDIMGFMT  0x0A
#define ILIRDSELFDIAG  0x0F
#define ILIINVOFF  0x20
#define ILIINVON   0x21
#define ILIGAMMASET 0x26
#define ILIDISPOFF 0x28
#define ILIDISPON  0x29
#define ILICASET   0x2A
#define ILIPASET   0x2B
#define ILIRAMWR   0x2C
#define ILIRAMRD   0x2E
#define ILIPTLAR   0x30
#define ILIGRAMMODE  0x36
#define DEFAULTILIGRAMMODE ILIMADCTL_BGR
#define ILIMADCTL_MY  0x80
#define ILIMADCTL_MX  0x40
#define ILIMADCTL_MV  0x20
#define ILIMADCTL_ML  0x10
#define ILIMADCTL_RGB 0x00
#define ILIMADCTL_BGR 0x08
#define ILIMADCTL_MH  0x04
#define ILIPIXFMT  0x3A
#define ILIFRMCTR1 0xB1
#define ILIFRMCTR2 0xB2
#define ILIFRMCTR3 0xB3
#define ILIINVCTR  0xB4
#define ILIDFUNCTR 0xB6
#define ILIPWCTR1  0xC0
#define ILIPWCTR2  0xC1
#define ILIPWCTR3  0xC2
#define ILIPWCTR4  0xC3
#define ILIPWCTR5  0xC4
#define ILIVMCTR1  0xC5
#define ILIVMCTR2  0xC7
#define ILIRDID1   0xDA
#define ILIRDID2   0xDB
#define ILIRDID3   0xDC
#define ILIRDID4   0xDD
#define ILIGMCTRP1 0xE0
#define ILIGMCTRN1 0xE1
#define LCD_HEIGHT      320
#define LCD_WIDTH       240
#define TOUCH_TRIM  0x00
Screen_ILI9340::Screen_ILI9340()
{
    ;
}
void Screen_ILI9340::begin()
{
    pinMode(_pinScreenCS, OUTPUT);
    pinMode(_pinSDcardCS, OUTPUT);
    pinMode(_pinScreenReset, OUTPUT);
    pinMode(_pinScreenDataCommand, OUTPUT);
    pinMode(_pinScreenReset, OUTPUT);
    digitalWrite(_pinScreenDataCommand, HIGH);
#if defined(__LM4F120H5QR__) || defined(__TM4C1230C3PM__)
    SPI.setModule(2);
    SPI.begin();
    SPI.setClockDivider(SPI_CLOCK_DIV2);
#else
    SPI.begin();
    SPI.setClockDivider(SPI_CLOCK_DIV1);
#endif
    SPI.setBitOrder(MSBFIRST);
    SPI.setDataMode(SPI_MODE0);
    delay(1);
    digitalWrite(_pinScreenReset, LOW);
    delay(10);
    digitalWrite(_pinScreenReset, HIGH);
    delay(10);
    _writeCommand8(0xEF);
    _writeData8(0x03);
    _writeData8(0x80);
    _writeData8(0x02);
    _writeCommand8(0xCF);
    _writeData8(0x00);
    _writeData8(0XC1);
    _writeData8(0X30);
    _writeCommand8(0xED);
    _writeData8(0x64);
    _writeData8(0x03);
    _writeData8(0X12);
    _writeData8(0X81);
    _writeCommand8(0xE8);
    _writeData8(0x85);
    _writeData8(0x00);
    _writeData8(0x78);
    _writeCommand8(0xCB);
    _writeData8(0x39);
    _writeData8(0x2C);
    _writeData8(0x00);
    _writeData8(0x34);
    _writeData8(0x02);
    _writeCommand8(0xF7);
    _writeData8(0x20);
    _writeCommand8(0xEA);
    _writeData8(0x00);
    _writeData8(0x00);
    _writeCommand8(ILIPWCTR1);
    _writeData8(0x23);
    _writeCommand8(ILIPWCTR2);
    _writeData8(0x10);
    _writeCommand8(ILIVMCTR1);
    _writeData8(0x3e);
    _writeData8(0x28);
    _writeCommand8(ILIVMCTR2);
    _writeData8(0x86);
    _writeCommand8(ILIGRAMMODE);
    _writeData8(DEFAULTILIGRAMMODE);
    _writeCommand8(ILIPIXFMT);
    _writeData8(0x55);
    _writeCommand8(ILIFRMCTR1);
    _writeData8(0x00);
    _writeData8(0x18);
    _writeCommand8(ILIDFUNCTR);
    _writeData8(0x08);
    _writeData8(0x82);
    _writeData8(0x27);
    _writeCommand8(0xF2);
    _writeData8(0x00);
    _writeCommand8(ILIGAMMASET);
    _writeData8(0x01);
    _writeCommand8(ILIGMCTRP1);
    _writeData8(0x0F);
    _writeData8(0x31);
    _writeData8(0x2B);
    _writeData8(0x0C);
    _writeData8(0x0E);
    _writeData8(0x08);
    _writeData8(0x4E);
    _writeData8(0xF1);
    _writeData8(0x37);
    _writeData8(0x07);
    _writeData8(0x10);
    _writeData8(0x03);
    _writeData8(0x0E);
    _writeData8(0x09);
    _writeData8(0x00);
    _writeCommand8(ILIGMCTRN1);
    _writeData8(0x00);
    _writeData8(0x0E);
    _writeData8(0x14);
    _writeData8(0x03);
    _writeData8(0x11);
    _writeData8(0x07);
    _writeData8(0x31);
    _writeData8(0xC1);
    _writeData8(0x48);
    _writeData8(0x08);
    _writeData8(0x0F);
    _writeData8(0x0C);
    _writeData8(0x31);
    _writeData8(0x36);
    _writeData8(0x0F);
    _writeCommand8(ILISLPOUT);
    delay(120);
    _writeCommand8(ILIDISPON);
    _writeCommand8(ILIINVOFF);
    _writeRegister8(0x55, 0x01);
    _screenWidth = LCD_WIDTH;
    _screenHeigth = LCD_HEIGHT;
    _screenDiagonal = 22;
    _penSolid  = false;
    _fontSolid = true;
    setOrientation(0);
    clear();
}
String Screen_ILI9340::WhoAmI()
{
    return "LCD 2.2\" + SD BoosterPack";
}
void Screen_ILI9340::_setOrientation(uint8_t orientation)
{
    switch (_orientation) {
        case 0:
            _writeRegister8(ILIGRAMMODE, 0b01000000 | DEFAULTILIGRAMMODE);
            break;
        case 1:
            _writeRegister8(ILIGRAMMODE, 0b00100000 | DEFAULTILIGRAMMODE);
            break;
        case 2:
            _writeRegister8(ILIGRAMMODE, 0b10000000 | DEFAULTILIGRAMMODE);
            break;
        case 3:
            _writeRegister8(ILIGRAMMODE, 0b11100000 | DEFAULTILIGRAMMODE);
            break;
    }
}
void Screen_ILI9340::_writeCommand8(uint8_t command8)
{
    digitalWrite(_pinScreenDataCommand, LOW);
    digitalWrite(_pinScreenCS, LOW);
    SPI.transfer(command8);
    digitalWrite(_pinScreenCS, HIGH);
    digitalWrite(_pinScreenDataCommand, HIGH);
}
void Screen_ILI9340::_writeData16(uint16_t data16)
{
    digitalWrite(_pinScreenDataCommand, HIGH);
    digitalWrite(_pinScreenCS, LOW);
    SPI.transfer(highByte(data16));
    SPI.transfer(lowByte(data16));
    digitalWrite(_pinScreenCS, HIGH);
}
void Screen_ILI9340::_writeData8(uint8_t data8)
{
    digitalWrite(_pinScreenCS, LOW);
    SPI.transfer(data8);
    digitalWrite(_pinScreenCS, HIGH);
}
void Screen_ILI9340::_writeData88(uint8_t dataHigh8, uint8_t dataLow8)
{
    digitalWrite(_pinScreenCS, LOW);
    SPI.transfer(dataHigh8);
    SPI.transfer(dataLow8);
    digitalWrite(_pinScreenCS, HIGH);
}
void Screen_ILI9340::_writeRegister16(uint8_t command8, uint16_t data16)
{
    _writeCommand8(command8);
 _writeData88(highByte(data16), lowByte(data16));
}
void Screen_ILI9340::_writeRegister8(uint8_t command8, uint8_t data8)
{
    _writeCommand8(command8);
 _writeData8(data8);
}
void Screen_ILI9340::_setPoint(uint16_t x1, uint16_t y1, uint16_t colour)
{
    _setWindow(x1, y1, x1, y1);
    _writeData88(highByte(colour), lowByte(colour));
}
void Screen_ILI9340::_setWindow(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
    if (x1>x2) _swap(x1, x2);
    if (y1>y2) _swap(y1, y2);
    _writeCommand8(ILICASET);
    _writeData16(x1);
    _writeData16(x2);
    _writeCommand8(ILIPASET);
    _writeData16(y1);
    _writeData16(y2);
    _writeCommand8(ILIRAMWR);
}
void Screen_ILI9340::_fastFill(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t colour)
{
    if (x1 > x2) _swap(x1, x2);
    if (y1 > y2) _swap(y1, y2);
    _setWindow(x1, y1, x2, y2);
    uint8_t hi = highByte(colour);
    uint8_t lo = lowByte(colour);
    for (uint32_t t=(uint32_t)(y2-y1+1)*(x2-x1+1); t>0; t--) {
        _writeData88(hi, lo);
    }
}
uint16_t Screen_ILI9340::readPixel(uint16_t x1, uint16_t y1)
{
    return 0;
}
void Screen_ILI9340::clear(uint16_t colour)
{
    uint8_t oldOrientation = _orientation;
    bool oldPenSolid = _penSolid;
    setOrientation(0);
    setPenSolid();
    rectangle(0, 0, screenSizeX()-1, screenSizeY()-1, colour);
    setOrientation(oldOrientation);
    setPenSolid(oldPenSolid);
}
void Screen_ILI9340::setOrientation(uint8_t orientation)
{
    _orientation = orientation % 4;
    _setOrientation(_orientation);
}
uint8_t Screen_ILI9340::getOrientation()
{
    return _orientation;
}
uint16_t Screen_ILI9340::screenSizeX()
{
    switch (_orientation) {
        case 0:
        case 2:
            return _screenWidth;
            break;
        case 1:
        case 3:
            return _screenHeigth;
            break;
    }
}
uint16_t Screen_ILI9340::screenSizeY()
{
    switch (_orientation) {
        case 0:
        case 2:
            return _screenHeigth;
            break;
        case 1:
        case 3:
            return _screenWidth;
            break;
    }
}
uint16_t Screen_ILI9340::screenDiagonal()
{
    return _screenDiagonal;
}
void Screen_ILI9340::circle(uint16_t x0, uint16_t y0, uint16_t radius, uint16_t colour)
{
    int16_t f = 1 - radius;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * radius;
    int16_t x = 0;
    int16_t y = radius;
    if (_penSolid == false) {
        point(x0, y0+radius, colour);
        point(x0, y0-radius, colour);
        point(x0+radius, y0, colour);
        point(x0-radius, y0, colour);
        while (x<y) {
            if (f >= 0) {
                y--;
                ddF_y += 2;
                f += ddF_y;
            }
            x++;
            ddF_x += 2;
            f += ddF_x;
            point(x0 + x, y0 + y, colour);
            point(x0 - x, y0 + y, colour);
            point(x0 + x, y0 - y, colour);
            point(x0 - x, y0 - y, colour);
            point(x0 + y, y0 + x, colour);
            point(x0 - y, y0 + x, colour);
            point(x0 + y, y0 - x, colour);
            point(x0 - y, y0 - x, colour);
        }
    } else {
        while (x<y) {
            if (f >= 0) {
                y--;
                ddF_y += 2;
                f += ddF_y;
            }
            x++;
            ddF_x += 2;
            f += ddF_x;
            line(x0 + x, y0 + y, x0 - x, y0 + y, colour);
            line(x0 + x, y0 - y, x0 - x, y0 - y, colour);
            line(x0 + y, y0 - x, x0 + y, y0 + x, colour);
            line(x0 - y, y0 - x, x0 - y, y0 + x, colour);
        }
        setPenSolid(true);
        rectangle(x0-x, y0-y, x0+x, y0+y, colour);
    }
}
void Screen_ILI9340::line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t colour)
{
    if ((x1 == x2) && (y1 == y2)) {
        _setPoint(x1, y1, colour);
    } else if ((x1 == x2) || (y1 == y2)) {
        _fastFill(x1, y1, x2, y2, colour);
    } else {
        int16_t wx1 = (int16_t)x1;
        int16_t wx2 = (int16_t)x2;
        int16_t wy1 = (int16_t)y1;
        int16_t wy2 = (int16_t)y2;
        boolean flag = abs(wy2 - wy1) > abs(wx2 - wx1);
        if (flag) {
            _swap(wx1, wy1);
            _swap(wx2, wy2);
        }
        if (wx1 > wx2) {
            _swap(wx1, wx2);
            _swap(wy1, wy2);
        }
        int16_t dx = wx2 - wx1;
        int16_t dy = abs(wy2 - wy1);
        int16_t err = dx / 2;
        int16_t ystep;
        if (wy1 < wy2) ystep = 1;
        else ystep = -1;
        for (; wx1<=wx2; wx1++) {
            if (flag) _setPoint(wy1, wx1, colour);
            else _setPoint(wx1, wy1, colour);
            err -= dy;
            if (err < 0) {
                wy1 += ystep;
                err += dx;
            }
        }
    }
}
void Screen_ILI9340::dLine(uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy, uint16_t colour)
{
    line(x0, y0, x0+dx-1, y0+dy-1, colour);
}
void Screen_ILI9340::setPenSolid(bool flag)
{
    _penSolid = flag;
}
void Screen_ILI9340::point(uint16_t x1, uint16_t y1, uint16_t colour)
{
    _setPoint(x1, y1, colour);
}
void Screen_ILI9340::rectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t colour)
{
    if (_penSolid == false) {
        line(x1, y1, x1, y2, colour);
        line(x1, y1, x2, y1, colour);
        line(x1, y2, x2, y2, colour);
        line(x2, y1, x2, y2, colour);
    } else {
        _fastFill(x1, y1, x2, y2, colour);
    }
}
void Screen_ILI9340::dRectangle(uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy, uint16_t colour)
{
    rectangle(x0, y0, x0+dx-1, y0+dy-1, colour);
}
void Screen_ILI9340::_triangleArea(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, uint16_t colour)
{
    int16_t wx1 = (int16_t)x1;
    int16_t wy1 = (int16_t)y1;
    int16_t wx2 = (int16_t)x2;
    int16_t wy2 = (int16_t)y2;
    int16_t wx3 = (int16_t)x3;
    int16_t wy3 = (int16_t)y3;
    int16_t wx4 = wx1;
    int16_t wy4 = wy1;
    int16_t wx5 = wx1;
    int16_t wy5 = wy1;
    bool changed1 = false;
    bool changed2 = false;
    int16_t dx1 = abs(wx2 - wx1);
    int16_t dy1 = abs(wy2 - wy1);
    int16_t dx2 = abs(wx3 - wx1);
    int16_t dy2 = abs(wy3 - wy1);
    int16_t signx1 = (wx2 >= wx1) ? +1 : -1;
    int16_t signx2 = (wx3 >= wx1) ? +1 : -1;
    int16_t signy1 = (wy2 >= wy1) ? +1 : -1;
    int16_t signy2 = (wy3 >= wy1) ? +1 : -1;
    if (dy1 > dx1) {
        _swap(dx1, dy1);
        changed1 = true;
    }
    if (dy2 > dx2) {
        _swap(dx2, dy2);
        changed2 = true;
    }
    int16_t e1 = 2 * dy1 - dx1;
    int16_t e2 = 2 * dy2 - dx2;
    for (int i = 0; i <= dx1; i++) {
        line(wx4, wy4, wx5, wy5, colour);
        while (e1 >= 0) {
            if (changed1) wx4 += signx1;
            else wy4 += signy1;
            e1 = e1 - 2 * dx1;
        }
        if (changed1) wy4 += signy1;
        else wx4 += signx1;
        e1 = e1 + 2 * dy1;
        while (wy5 != wy4) {
            while (e2 >= 0) {
                if (changed2) wx5 += signx2;
                else wy5 += signy2;
                e2 = e2 - 2 * dx2;
            }
            if (changed2) wy5 += signy2;
            else wx5 += signx2;
            e2 = e2 + 2 * dy2;
        }
    }
}
void Screen_ILI9340::triangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, uint16_t colour)
{
    if (_penSolid) {
        bool b = true;
        while (b) {
            b = false;
            if ((b == false) && (y1 > y2)) {
                _swap(x1, x2);
                _swap(y1, y2);
                b = true;
            }
            if ((b == false) && (y2 > y3)) {
                _swap(x3, x2);
                _swap(y3, y2);
                b = true;
            }
        }
        if (y2 == y3) {
            _triangleArea(x1, y1, x2, y2, x3, y3, colour);
        } else if (y1 == y2) {
            _triangleArea(x3, y3, x1, y1, x2, y2, colour);
        } else {
            uint16_t x4 = (uint16_t)( (int32_t)x1 + (y2 - y1) * (x3 - x1) / (y3 - y1) );
            uint16_t y4 = y2;
            _triangleArea(x1, y1, x2, y2, x4, y4, colour);
            _triangleArea(x3, y3, x2, y2, x4, y4, colour);
        }
    } else {
        line(x1, y1, x2, y2, colour);
        line(x2, y2, x3, y3, colour);
        line(x3, y3, x1, y1, colour);
    }
}
void Screen_ILI9340::_swap(uint16_t &a, uint16_t &b)
{
    uint16_t w = a;
    a = b;
    b = w;
}
void Screen_ILI9340::_swap(int16_t &a, int16_t &b)
{
    int16_t w = a;
    a = b;
    b = w;
}
uint16_t Screen_ILI9340::_check(uint16_t x0, uint16_t xmin, uint16_t xmax)
{
    if (xmin < xmax) {
        if (x0 < xmin) return xmin;
        else if (x0 > xmax) return xmax;
        else return x0;
    } else {
        if (x0 < xmax) return xmax;
        else if (x0 > xmin) return xmin;
        else return x0;
    }
}
void Screen_ILI9340::setFontSolid(bool flag)
{
    _fontSolid = flag;
}
void Screen_ILI9340::setFontSize(uint8_t size)
{
    if (size < MAX_FONT_SIZE) _fontSize = size;
    else _fontSize = MAX_FONT_SIZE -1;
}
uint8_t Screen_ILI9340::fontMax()
{
    return MAX_FONT_SIZE;
}
uint8_t Screen_ILI9340::fontSizeX()
{
#if (MAX_FONT_SIZE > 0)
    if (_fontSize == 0) return 6;
#if (MAX_FONT_SIZE > 1)
    else if (_fontSize == 1) return 8;
#endif
#endif
    else return 0;
}
uint8_t Screen_ILI9340::fontSizeY()
{
#if (MAX_FONT_SIZE > 0)
    if (_fontSize == 0) return 8;
#if (MAX_FONT_SIZE > 1)
    else if (_fontSize == 1) return 12;
#endif
#endif
    else return 0;
}
uint8_t Screen_ILI9340::_getCharacter(uint8_t c, uint8_t i) {
#if defined(ENERGIA)
#if (MAX_FONT_SIZE > 0)
    if (_fontSize == 0)      return Terminal6x8e[c][i];
#if (MAX_FONT_SIZE > 1)
    else if (_fontSize == 1) return Terminal8x12e[c][i];
#endif
#endif
    else return 0;
#else
#if (MAX_FONT_SIZE > 0)
    if (_fontSize == 0)      return pgm_read_byte(&Terminal6x8e[c][i]);
#if (MAX_FONT_SIZE > 1)
    else if (_fontSize == 1) return pgm_read_byte(&Terminal8x12e[c][i]);
#endif
#endif
    else return 0;
#endif
}
void Screen_ILI9340::gText(uint16_t x0, uint16_t y0,
                           String s,
                           uint16_t textColour, uint16_t backColour,
                           uint8_t ix, uint8_t iy)
{
    uint8_t c;
    uint8_t line, line1, line2, line3;
    uint16_t x, y;
    uint8_t i, j, k;
    if ((ix > 1) || (iy > 1) || !_fontSolid) {
        if ((_fontSize == 0) && ((ix > 1) || (iy > 1))) {
            bool oldPenSolid = _penSolid;
            setPenSolid(true);
            for (k=0; k<s.length(); k++) {
                x = x0 + 6 * k * ix;
                y = y0;
                c = s.charAt(k)-' ';
                for ( i=0; i<6; i++ ) {
                    line = _getCharacter(c, i);
                    for ( j = 0; j<8; j++) {
                        if (bitRead(line, j)) rectangle(x+i*ix, y+j*iy, x+i*ix+ix-1, y+j*iy+iy-1, textColour);
                        else if (_fontSolid)  rectangle(x+i*ix, y+j*iy, x+i*ix+ix-1, y+j*iy+iy-1, backColour);
                    }
                }
            }
            setPenSolid(oldPenSolid);
        } else {
#if (MAX_FONT_SIZE > 0)
            if (_fontSize == 0) {
                for (k=0; k<s.length(); k++) {
                    c = s.charAt(k)-' ';
                    for (i=0; i<6; i++) {
                        line = _getCharacter(c, i);
                        for (j=0; j<8; j++)
                            if (bitRead(line, j)) point(x0 + 6*k + i, y0 + j, textColour);
                            else if (_fontSolid)  point(x0 + 6*k + i, y0 + j, backColour);
                    }
                }
            }
#if (MAX_FONT_SIZE > 1)
            else if (_fontSize == 1) {
                for (k=0; k<s.length(); k++) {
                    c = s.charAt(k)-' ';
                    for (i=0; i<8; i++) {
                        line = _getCharacter(c, 2*i);
                        line1 =_getCharacter(c, 2*i+1);
                        for (j=0; j<8; j++) {
                            if (bitRead(line, j))   point(x0 + 8*k + i, y0 + j, textColour);
                            else if (_fontSolid)    point(x0 + 8*k + i, y0 + j, backColour);
                            if (bitRead(line1, j))  point(x0 + 8*k + i, y0 +8 + j, textColour);
                            else if ((_fontSolid) && (j<4)) point(x0 + 8*k + i, y0 +8 + j, backColour);
                        }
                    }
                }
            }
#endif
#endif
        }
    } else {
        uint8_t c;
        uint8_t line, line1;
        uint16_t x, y;
        uint8_t i, j, k;
        uint8_t highTextColour = highByte(textColour);
        uint8_t lowTextColour  = lowByte(textColour);
        uint8_t highBackColour = highByte(backColour);
        uint8_t lowBackColour  = lowByte(backColour);
#if (MAX_FONT_SIZE > 0)
        if (_fontSize == 0) {
            for (k=0; k<s.length(); k++) {
                c = s.charAt(k)-' ';
                _setWindow(x0 +fontSizeX()*k, y0, x0 +fontSizeX()*(k+1)-1, y0+fontSizeY()-1);
                for (j=0; j<8; j++) {
                    for (i=0; i<6; i++) {
                        line = _getCharacter(c, i);
                        if (bitRead(line, j))   _writeData88(highTextColour, lowTextColour);
                        else                    _writeData88(highBackColour, lowBackColour);
                    }
                }
            }
        }
#if (MAX_FONT_SIZE > 1)
        else if (_fontSize == 1) {
            for (k=0; k<s.length(); k++) {
                c = s.charAt(k)-' ';
                _setWindow(x0 +fontSizeX()*k, y0, x0 +fontSizeX()*(k+1)-1, y0+fontSizeY()-1);
                for (j=0; j<8; j++) {
                    for (i=0; i<8; i++) {
                        line = _getCharacter(c, 2*i);
                        if (bitRead(line, j))   _writeData88(highTextColour, lowTextColour);
                        else                    _writeData88(highBackColour, lowBackColour);
                    }
                }
                for (j=0; j<4; j++) {
                    for (i=0; i<8; i++) {
                        line = _getCharacter(c, 2*i+1);
                        if (bitRead(line, j))   _writeData88(highTextColour, lowTextColour);
                        else                    _writeData88(highBackColour, lowBackColour);
                    }
                }
            }
        }
#endif
#endif
    }
}
uint16_t Screen_ILI9340::calculate(uint8_t red, uint8_t green, uint8_t blue)
{
    return (red >> 3) << 11 | (green >> 2) << 5 | (blue >> 3);
}
