#pragma once

/***************************************************
  Arduino TFT graphics library targeted at ESP8266
  and ESP32 based boards.

  This is a stand-alone library that contains the
  hardware driver, the graphics functions and the
  proportional fonts.

  The built-in fonts 4, 6, 7 and 8 are Run Length
  Encoded (RLE) to reduce the FLASH footprint.

  Last review/edit by Bodmer: 04/02/22
 ****************************************************/

#include <stdint.h>
#include <stdbool.h>

#define TFT_ESPI_VERSION "2.5.43"

// Identical looking TFT displays may have a different colour ordering in the 16-bit colour
#define TFT_BGR 0   // Colour order Blue-Green-Red
#define TFT_RGB 1   // Colour order Red-Green-Blue

// Invoke 18-bit colour for selected displays
#if defined (ILI9481_DRIVER) || defined (ILI9486_DRIVER) || defined (ILI9488_DRIVER)
#define SPI_18BIT_DRIVER
#endif

// Load the right driver definition - do not tinker here !
#if defined(ILI9341_DRIVER) || defined(ILI9341_2_DRIVER) || defined(ILI9342_DRIVER)
#include <TFT_Drivers/ILI9341_Defines.h>
#define  TFT_DRIVER 0x9341
#elif defined (ST7735_DRIVER)
#include <TFT_Drivers/ST7735_Defines.h>
#define  TFT_DRIVER 0x7735
#elif defined (ILI9163_DRIVER)
#include <TFT_Drivers/ILI9163_Defines.h>
#define  TFT_DRIVER 0x9163
#elif defined (S6D02A1_DRIVER)
#include <TFT_Drivers/S6D02A1_Defines.h>
#define  TFT_DRIVER 0x6D02
#elif defined (ST7796_DRIVER)
#include "TFT_Drivers/ST7796_Defines.h"
#define  TFT_DRIVER 0x7796
#elif defined (ILI9486_DRIVER)
#include <TFT_Drivers/ILI9486_Defines.h>
#define  TFT_DRIVER 0x9486
#elif defined (ILI9481_DRIVER)
#include <TFT_Drivers/ILI9481_Defines.h>
#define  TFT_DRIVER 0x9481
#elif defined (ILI9488_DRIVER)
#include <TFT_Drivers/ILI9488_Defines.h>
#define  TFT_DRIVER 0x9488
#elif defined (HX8357D_DRIVER)
#include "TFT_Drivers/HX8357D_Defines.h"
#define  TFT_DRIVER 0x8357
#elif defined (EPD_DRIVER)
#include "TFT_Drivers/EPD_Defines.h"
#define  TFT_DRIVER 0xE9D
#elif defined (ST7789_DRIVER)
#include "TFT_Drivers/ST7789_Defines.h"
#define  TFT_DRIVER 0x7789
#elif defined (R61581_DRIVER)
#include "TFT_Drivers/R61581_Defines.h"
#define  TFT_DRIVER 0x6158
#elif defined (ST7789_2_DRIVER)
#include "TFT_Drivers/ST7789_2_Defines.h"
#define  TFT_DRIVER 0x778B
#elif defined (RM68140_DRIVER)
#include "TFT_Drivers/RM68140_Defines.h"
#define  TFT_DRIVER 0x6814
#elif defined (SSD1351_DRIVER)
#include "TFT_Drivers/SSD1351_Defines.h"
#define  TFT_DRIVER 0x1351
#elif defined (SSD1963_480_DRIVER)
#include "TFT_Drivers/SSD1963_Defines.h"
#define  TFT_DRIVER 0x1963
#elif defined (SSD1963_800_DRIVER)
#include "TFT_Drivers/SSD1963_Defines.h"
#define  TFT_DRIVER 0x1963
#elif defined (SSD1963_800ALT_DRIVER)
#include "TFT_Drivers/SSD1963_Defines.h"
#define  TFT_DRIVER 0x1963
#elif defined (SSD1963_800BD_DRIVER)
#include "TFT_Drivers/SSD1963_Defines.h"
#define  TFT_DRIVER 0x1963
#elif defined (GC9A01_DRIVER)
#include "TFT_Drivers/GC9A01_Defines.h"
#define  TFT_DRIVER 0x9A01
#elif defined (ILI9225_DRIVER)
#include "TFT_Drivers/ILI9225_Defines.h"
#define  TFT_DRIVER 0x9225
#elif defined (RM68120_DRIVER)
#include "TFT_Drivers/RM68120_Defines.h"
#define  TFT_DRIVER 0x6812
#elif defined (HX8357B_DRIVER)
#include "TFT_Drivers/HX8357B_Defines.h"
#define  TFT_DRIVER 0x835B
#elif defined (HX8357C_DRIVER)
#include "TFT_Drivers/HX8357C_Defines.h"
#define  TFT_DRIVER 0x835C
#else
#define  TFT_DRIVER 0x0000
#endif

// Callback prototype for smooth font pixel colour read
typedef uint16_t (*getColorCallback)(uint16_t x, uint16_t y);

// Handle FLASH based storage e.g. PROGMEM
#define pgm_read_byte(addr)   (*(const unsigned char *)(addr))

#define pgm_read_word(addr) ({ \
  typeof(addr) _addr = (addr); \
  *(const unsigned short *)(_addr); \
})
#define pgm_read_dword(addr) ({ \
  typeof(addr) _addr = (addr); \
  *(const unsigned long *)(_addr); \
})

#ifndef PROGMEM
#define PROGMEM
#endif

/***************************************************************************************
**                         Section 3: Interface setup
***************************************************************************************/
#ifndef TAB_COLOUR
#define TAB_COLOUR 0
#endif

// If the SPI frequency is not defined, set a default
#ifndef SPI_FREQUENCY
#define SPI_FREQUENCY  20000000
#endif

// If the SPI read frequency is not defined, set a default
#ifndef SPI_READ_FREQUENCY
#define SPI_READ_FREQUENCY 10000000
#endif

// Some ST7789 boards do not work with Mode 0
#ifndef TFT_SPI_MODE
#if defined(ST7789_DRIVER) || defined(ST7789_2_DRIVER)
#define TFT_SPI_MODE SPI_MODE3
#else
#define TFT_SPI_MODE SPI_MODE0
#endif
#endif

#ifndef SPI_BUSY_CHECK
#define SPI_BUSY_CHECK
#endif

// If half duplex SDA mode is defined then MISO pin should be -1
#ifdef TFT_SDA_READ
#ifdef TFT_MISO
#if TFT_MISO != -1
#undef TFT_MISO
#define TFT_MISO -1
#warning TFT_MISO set to -1
#endif
#endif
#endif

/***************************************************************************************
**                         Section 4: Setup fonts
***************************************************************************************/
// Only load the fonts defined in User_Setup.h (to save space)
// Set flag so RLE rendering code is optionally compiled
#ifdef LOAD_GLCD
#include <Fonts/glcdfont.h>
#endif

#ifdef LOAD_FONT2
#include <Fonts/Font16.h>
#endif

#ifdef LOAD_FONT4
#include <Fonts/Font32rle.h>
#define LOAD_RLE
#endif

#ifdef LOAD_FONT6
#include <Fonts/Font64rle.h>
#ifndef LOAD_RLE
#define LOAD_RLE
#endif
#endif

#ifdef LOAD_FONT7
#include <Fonts/Font7srle.h>
#ifndef LOAD_RLE
#define LOAD_RLE
#endif
#endif

#ifdef LOAD_FONT8
#include <Fonts/Font72rle.h>
#ifndef LOAD_RLE
#define LOAD_RLE
#endif
#elif defined LOAD_FONT8N // Optional narrower version
#define LOAD_FONT8
#include <Fonts/Font72x53rle.h>
#ifndef LOAD_RLE
#define LOAD_RLE
#endif
#endif

#ifdef LOAD_GFXFF
// We can include all the free fonts and they will only be built into
// the sketch if they are used
#include <Fonts/GFXFF/gfxfont.h>
#endif // #ifdef LOAD_GFXFF

/***************************************************************************************
**                         Section 5: Font datum enumeration
***************************************************************************************/
//These enumerate the text plotting alignment (reference datum point)
#define TL_DATUM 0 // Top left (default)
#define TC_DATUM 1 // Top centre
#define TR_DATUM 2 // Top right
#define ML_DATUM 3 // Middle left
#define CL_DATUM 3 // Centre left, same as above
#define MC_DATUM 4 // Middle centre
#define CC_DATUM 4 // Centre centre, same as above
#define MR_DATUM 5 // Middle right
#define CR_DATUM 5 // Centre right, same as above
#define BL_DATUM 6 // Bottom left
#define BC_DATUM 7 // Bottom centre
#define BR_DATUM 8 // Bottom right
#define L_BASELINE  9 // Left character baseline (Line the 'A' character would sit on)
#define C_BASELINE 10 // Centre character baseline
#define R_BASELINE 11 // Right character baseline

/***************************************************************************************
**                         Section 6: Colour enumeration
***************************************************************************************/
// Default color definitions
#define TFT_BLACK       0x0000      /*   0,   0,   0 */
#define TFT_NAVY        0x000F      /*   0,   0, 128 */
#define TFT_DARKGREEN   0x03E0      /*   0, 128,   0 */
#define TFT_DARKCYAN    0x03EF      /*   0, 128, 128 */
#define TFT_MAROON      0x7800      /* 128,   0,   0 */
#define TFT_PURPLE      0x780F      /* 128,   0, 128 */
#define TFT_OLIVE       0x7BE0      /* 128, 128,   0 */
#define TFT_LIGHTGREY   0xD69A      /* 211, 211, 211 */
#define TFT_DARKGREY    0x7BEF      /* 128, 128, 128 */
#define TFT_BLUE        0x001F      /*   0,   0, 255 */
#define TFT_GREEN       0x07E0      /*   0, 255,   0 */
#define TFT_CYAN        0x07FF      /*   0, 255, 255 */
#define TFT_RED         0xF800      /* 255,   0,   0 */
#define TFT_MAGENTA     0xF81F      /* 255,   0, 255 */
#define TFT_YELLOW      0xFFE0      /* 255, 255,   0 */
#define TFT_WHITE       0xFFFF      /* 255, 255, 255 */
#define TFT_ORANGE      0xFDA0      /* 255, 180,   0 */
#define TFT_GREENYELLOW 0xB7E0      /* 180, 255,   0 */
#define TFT_PINK        0xFE19      /* 255, 192, 203 */ //Lighter pink, was 0xFC9F
#define TFT_BROWN       0x9A60      /* 150,  75,   0 */
#define TFT_GOLD        0xFEA0      /* 255, 215,   0 */
#define TFT_SILVER      0xC618      /* 192, 192, 192 */
#define TFT_SKYBLUE     0x867D      /* 135, 206, 235 */
#define TFT_VIOLET      0x915C      /* 180,  46, 226 */

// Next is a special 16-bit colour value that encodes to 8 bits
// and will then decode back to the same 16-bit value.
// Convenient for 8-bit and 16-bit transparent sprites.
#define TFT_TRANSPARENT 0x0120 // This is actually a dark green

/***************************************************************************************
**                         Section 8: Class member and support functions
***************************************************************************************/

void displayInit(int16_t w, int16_t h);

// init() and begin() are equivalent, begin() included for backwards compatibility
// Sketch defined tab colour option is for ST7735 displays only
void begin(uint8_t tc);

// These are virtual so the TFT_eSprite class can override them with sprite specific functions
void drawPixel(int32_t x, int32_t y, uint32_t color);
void drawChar(int32_t x, int32_t y, uint16_t c, uint32_t color, uint32_t bg, uint8_t size);
int16_t drawCharUnicode(uint16_t uniCode, int32_t x, int32_t y, uint8_t font);

void drawLine(int32_t xs, int32_t ys, int32_t xe, int32_t ye, uint32_t color);
void drawFastVLine(int32_t x, int32_t y, int32_t h, uint32_t color);
void drawFastHLine(int32_t x, int32_t y, int32_t w, uint32_t color);
void fillRect(int32_t x, int32_t y, int32_t w, int32_t h, uint32_t color);

int16_t height(void);
int16_t width(void);

// Read the colour of a pixel at x,y and return value in 565 format
uint16_t readPixel(int32_t x, int32_t y);

void setWindow(int32_t xs, int32_t ys, int32_t xe, int32_t ye); // Note: start + end coordinates

// Push (aka write pixel) colours to the set window
void pushColor(uint16_t color);
void pushColorN(uint16_t color, uint32_t len);

void setRotation(uint8_t r); // Set the display image orientation to 0, 1, 2 or 3
uint8_t getRotation(void); // Read the current rotation

// Change the origin position from the default top left
// Note: setRotation, setViewport and resetViewport will revert origin to top left corner of screen/sprite
void setOrigin(int32_t x, int32_t y);
int32_t getOriginX(void);
int32_t getOriginY(void);

void invertDisplay(bool i); // Tell TFT to invert all displayed colours

// The TFT_eSprite class inherits the following functions (not all are useful to Sprite class
void setAddrWindow(int32_t xs, int32_t ys, int32_t w, int32_t h); // Note: start coordinates + width and height

// Viewport commands, see "Viewport_Demo" sketch
void setViewport(int32_t x, int32_t y, int32_t w, int32_t h, bool vpDatum);
bool checkViewport(int32_t x, int32_t y, int32_t w, int32_t h);
int32_t getViewportX(void);
int32_t getViewportY(void);
int32_t getViewportWidth(void);
int32_t getViewportHeight(void);
bool getViewportDatum(void);
void frameViewport(uint16_t color, int32_t w);
void resetViewport(void);

// Clip input window to viewport bounds, return false if whole area is out of bounds
bool clipAddrWindow(int32_t *x, int32_t *y, int32_t *w, int32_t *h);
// Clip input window area to viewport bounds, return false if whole area is out of bounds
bool clipWindow(int32_t *xs, int32_t *ys, int32_t *xe, int32_t *ye);

// Push (aka write pixel) colours to the TFT (use setAddrWindow() first)
void pushColors(uint8_t *data, uint32_t len);
void pushColorsSwap(uint16_t *data, uint32_t len, bool swap); // With byte swap option

// Support for half duplex (bi-directional SDA) SPI bus where MOSI must be switched to input
#ifdef TFT_SDA_READ
#if defined (TFT_eSPI_ENABLE_8_BIT_READ)
uint8_t tft_Read_8(void); // Read 8-bit value from TFT command register
#endif
void begin_SDA_Read(void); // Begin a read on a half duplex (bi-directional SDA) SPI bus - sets MOSI to input
void end_SDA_Read(void); // Restore MOSI to output
#endif


// Graphics drawing
void fillScreen(uint32_t color);
void drawRect(int32_t x, int32_t y, int32_t w, int32_t h, uint32_t color);
void drawRoundRect(int32_t x, int32_t y, int32_t w, int32_t h, int32_t radius, uint32_t color);
void fillRoundRect(int32_t x, int32_t y, int32_t w, int32_t h, int32_t radius, uint32_t color);

void fillRectVGradient(int16_t x, int16_t y, int16_t w, int16_t h, uint32_t color1, uint32_t color2);
void fillRectHGradient(int16_t x, int16_t y, int16_t w, int16_t h, uint32_t color1, uint32_t color2);

void drawCircle(int32_t x, int32_t y, int32_t r, uint32_t color);
void drawCircleHelper(int32_t x, int32_t y, int32_t r, uint8_t cornername, uint32_t color);
void fillCircle(int32_t x, int32_t y, int32_t r, uint32_t color);
void fillCircleHelper(int32_t x, int32_t y, int32_t r, uint8_t cornername, int32_t delta, uint32_t color);

void drawEllipse(int16_t x, int16_t y, int32_t rx, int32_t ry, uint16_t color);
void fillEllipse(int16_t x, int16_t y, int32_t rx, int32_t ry, uint16_t color);

// Corner 1               Corner 2               Corner 3
void drawTriangle(int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t x3, int32_t y3, uint32_t color);
void fillTriangle(int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t x3, int32_t y3, uint32_t color);


// Smooth (anti-aliased) graphics drawing
// Draw a pixel blended with the background pixel colour (bg_color) specified,  return blended colour
// If the bg_color is not specified, the background pixel colour will be read from TFT or sprite
uint16_t drawPixelAlpha(int32_t x, int32_t y, uint32_t color, uint8_t alpha, uint32_t bg_color);

// Draw an anti-aliased (smooth) arc between start and end angles. Arc ends are anti-aliased.
// By default the arc is drawn with square ends unless the "roundEnds" parameter is included and set true
// Angle = 0 is at 6 o'clock position, 90 at 9 o'clock etc. The angles must be in range 0-360 or they will be clipped to these limits
// The start angle may be larger than the end angle. Arcs are always drawn clockwise from the start angle.
void drawSmoothArc(int32_t x, int32_t y, int32_t r, int32_t ir, uint32_t startAngle, uint32_t endAngle, uint32_t fg_color, uint32_t bg_color, bool roundEnds);

// As per "drawSmoothArc" except the ends of the arc are NOT anti-aliased, this facilitates dynamic arc length changes with
// arc segments and ensures clean segment joints.
// The sides of the arc are anti-aliased by default. If smoothArc is false sides will NOT be anti-aliased
void drawArc(int32_t x, int32_t y, int32_t r, int32_t ir, uint32_t startAngle, uint32_t endAngle, uint32_t fg_color, uint32_t bg_color, bool smoothArc);

// Draw an anti-aliased filled circle at x, y with radius r
// Note: The thickness of line is 3 pixels to reduce the visible "braiding" effect of anti-aliasing narrow lines
// this means the inner anti-alias zone is always at r-1 and the outer zone at r+1
void drawSmoothCircle(int32_t x, int32_t y, int32_t r, uint32_t fg_color, uint32_t bg_color);

// Draw an anti-aliased filled circle at x, y with radius r
// If bg_color is not included the background pixel colour will be read from TFT or sprite
void fillSmoothCircle(int32_t x, int32_t y, int32_t r, uint32_t color, uint32_t bg_color);

// Draw a rounded rectangle that has a line thickness of r-ir+1 and bounding box defined by x,y and w,h
// The outer corner radius is r, inner corner radius is ir
// The inside and outside of the border are anti-aliased
void drawSmoothRoundRect(int32_t x, int32_t y, int32_t r, int32_t ir, int32_t w, int32_t h, uint32_t fg_color, uint32_t bg_color, uint8_t quadrants);

// Draw a filled rounded rectangle , corner radius r and bounding box defined by x,y and w,h
void fillSmoothRoundRect(int32_t x, int32_t y, int32_t w, int32_t h, int32_t radius, uint32_t color, uint32_t bg_color);

// Draw a small anti-aliased filled circle at ax,ay with radius r (uses drawWideLine)
// If bg_color is not included the background pixel colour will be read from TFT or sprite
void drawSpot(float ax, float ay, float r, uint32_t fg_color, uint32_t bg_color);

// Draw an anti-aliased wide line from ax,ay to bx,by width wd with radiused ends (radius is wd/2)
// If bg_color is not included the background pixel colour will be read from TFT or sprite
void drawWideLine(float ax, float ay, float bx, float by, float wd, uint32_t fg_color, uint32_t bg_color);

// Draw an anti-aliased wide line from ax,ay to bx,by with different width at each end aw, bw and with radiused ends
// If bg_color is not included the background pixel colour will be read from TFT or sprite
void drawWedgeLine(float ax, float ay, float bx, float by, float aw, float bw, uint32_t fg_color, uint32_t bg_color);


// Image rendering
// Swap the byte order for pushImage() and pushPixels() - corrects endianness
void setSwapBytes(bool swap);
bool getSwapBytes(void);

// Draw bitmap
void drawBitmap(int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, uint16_t fgcolor);
void drawBitmapBG(int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, uint16_t fgcolor, uint16_t bgcolor);
void drawXBitmap(int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, uint16_t fgcolor);
void drawXBitmapBG(int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, uint16_t fgcolor, uint16_t bgcolor);
void setBitmapColor(uint16_t fgcolor, uint16_t bgcolor); // Define the 2 colours for 1bpp sprites

// Set TFT pivot point (use when rendering rotated sprites)
void setPivot(int16_t x, int16_t y);
int16_t getPivotX(void), // Get pivot x
        getPivotY(void); // Get pivot y

// The next functions can be used as a pair to copy screen blocks (or horizontal/vertical lines) to another location
// Read a block of pixels to a data buffer, buffer is 16-bit and the size must be at least w * h
void readRect(int32_t x, int32_t y, int32_t w, int32_t h, uint16_t *data);
// Write a block of pixels to the screen which have been read by readRect()
void pushRect(int32_t x, int32_t y, int32_t w, int32_t h, uint16_t *data);

// These are used to render images stored in FLASH (PROGMEM)
void pushImage(int32_t x, int32_t y, int32_t w, int32_t h, const uint16_t *data);
void pushImageTrans(int32_t x, int32_t y, int32_t w, int32_t h, const uint16_t *data, uint16_t transparent);

// They are not intended to be used with user sketches (but could be)
// Set bpp8 true for 8bpp sprites, false otherwise. The cmap pointer must be specified for 4bpp
void pushImage8(int32_t x, int32_t y, int32_t w, int32_t h, const uint8_t *data, bool bpp8, uint16_t *cmap);
void pushImage8Trans(int32_t x, int32_t y, int32_t w, int32_t h, const uint8_t *data, uint8_t transparent, bool bpp8, uint16_t *cmap);
// FLASH version

// Render a 16-bit colour image with a 1bpp mask
void pushMaskedImage(int32_t x, int32_t y, int32_t w, int32_t h, const uint16_t *img, uint8_t *mask);

// This next function has been used successfully to dump the TFT screen to a PC for documentation purposes
// It reads a screen area and returns the 3 RGB 8-bit colour values of each pixel in the buffer
// Set w and h to 1 to read 1 pixel's colour. The data buffer must be at least w * h * 3 bytes
void readRectRGB(int32_t x, int32_t y, int32_t w, int32_t h, uint8_t *data);


// Text rendering - value returned is the pixel width of the rendered text
int16_t drawNumber(long intNumber, int32_t x, int32_t y, uint8_t font); // Draw integer using specified font number

// Decimal is the number of decimal places to render
// Use with setTextDatum() to position values on TFT, and setTextPadding() to blank old displayed values
int16_t drawFloat(float floatNumber, uint8_t decimal, int32_t x, int32_t y, uint8_t font); // Draw float using specified font number

// Handle char arrays
// Use with setTextDatum() to position string on TFT, and setTextPadding() to blank old displayed strings
int16_t drawString(const char *string, int32_t x, int32_t y, uint8_t font); // Draw string using specified font number

int16_t drawCentreString(const char *string, int32_t x, int32_t y, uint8_t font); // Deprecated, use setTextDatum() and drawString()
int16_t drawRightString(const char *string, int32_t x, int32_t y, uint8_t font); // Deprecated, use setTextDatum() and drawString()


// Text rendering and font handling support functions
void setCursor(int16_t x, int16_t y, uint8_t font); // Set cursor and font number for tft.print()

int16_t getCursorX(void), // Read current cursor x position (moves with tft.print())
        getCursorY(void); // Read current cursor y position

void setTextColor(uint16_t color); // Set character (glyph) color only (background not over-written)
void setTextColorAll(uint16_t fgcolor, uint16_t bgcolor, bool bgfill); // Set character (glyph) foreground and background colour, optional background fill for smooth fonts
void setTextSize(uint8_t size); // Set character size multiplier (this increases pixel size)

void setTextWrap(bool wrapX, bool wrapY); // Turn on/off wrapping of text in TFT width and/or height

void setTextDatum(uint8_t datum); // Set text datum position (default is top left), see Section 5 above
uint8_t getTextDatum(void);

void setTextPadding(uint16_t x_width); // Set text padding (background blanking/over-write) width in pixels
uint16_t getTextPadding(void); // Get text padding

#ifdef LOAD_GFXFF
void setFreeFont(const GFXfont *f); // Select the GFX Free Font
void setTextFont(uint8_t font); // Set the font number to use in future
#else
void setFreeFont(uint8_t font); // Not used, historical fix to prevent an error
void setTextFont(uint8_t font); // Set the font number to use in future
#endif

int16_t textWidth(const char *string, uint8_t font); // Returns pixel width of string in specified font
int16_t fontHeight(int16_t font); // Returns pixel height of specified font

// Used by library and Smooth font class to extract Unicode point codes from a UTF8 encoded string
uint16_t decodeUTF8Buffer(uint8_t *buf, uint16_t *index, uint16_t remaining);
uint16_t decodeUTF8(uint8_t c);

// Support function to UTF8 decode and draw characters piped through print stream
size_t write(uint8_t);

// Used by Smooth font class to fetch a pixel colour for the anti-aliasing
void setCallback(getColorCallback getCol);

uint16_t fontsLoaded(void); // Each bit in returned value represents a font type that is loaded - used for debug/error handling only

// Low level read/write
void writecommand(uint8_t c); // Send an 8-bit command, function resets DC/RS high ready for data
void writedata(uint8_t d); // Send data with DC/RS set high

uint8_t readcommand8(uint8_t cmd_function, uint8_t index); // read 8 bits from TFT
uint16_t readcommand16(uint8_t cmd_function, uint8_t index); // read 16 bits from TFT
uint32_t readcommand32(uint8_t cmd_function, uint8_t index); // read 32 bits from TFT

// Colour conversion
// Convert 8-bit red, green and blue to 16 bits
uint16_t color565(uint8_t red, uint8_t green, uint8_t blue);

// Convert 8-bit colour to 16 bits
uint16_t color8to16(uint8_t color332);
// Convert 16-bit colour to 8 bits
uint8_t color16to8(uint16_t color565);

// Convert 16-bit colour to/from 24-bit, R+G+B concatenated into LS 24 bits
uint32_t color16to24(uint16_t color565);
uint32_t color24to16(uint32_t color888);

// Alpha blend 2 colours, see generic "alphaBlend_Test" example
// alpha =   0 = 100% background colour
// alpha = 255 = 100% foreground colour
uint16_t alphaBlend(uint8_t alpha, uint16_t fgc, uint16_t bgc);

// 16-bit colour alphaBlend with alpha dither (dither reduces colour banding)
uint16_t alphaBlendDither(uint8_t alpha, uint16_t fgc, uint16_t bgc, uint8_t dither);
// 24-bit colour alphaBlend with optional alpha dither
uint32_t alphaBlend24(uint8_t alpha, uint32_t fgc, uint32_t bgc, uint8_t dither);

// Bare metal functions
void startWrite(void); // Begin SPI transaction
void writeColor(uint16_t color, uint32_t len); // Deprecated, use pushBlock()
void endWrite(void); // End SPI transaction

// Set/get an arbitrary library configuration attribute or option
// Use to switch ON/OFF capabilities such as UTF8 decoding - each attribute has a unique ID
// id = 0: reserved - may be used in future to reset all attributes to a default state
// id = 1: Turn on (a=true) or off (a=false) GLCD cp437 font character error correction
// id = 2: Turn on (a=true) or off (a=false) UTF8 decoding
#define CP437_SWITCH 1
#define UTF8_SWITCH  2
void setAttribute(uint8_t id, uint8_t a); // Set attribute value
uint8_t getAttribute(uint8_t id); // Get attribute value

// New begin and end prototypes
// begin/end a TFT write transaction
// For SPI bus the transmit clock rate is set
inline void begin_tft_write() __attribute__((always_inline));
inline void end_tft_write() __attribute__((always_inline));

// begin/end a TFT read transaction
// For SPI bus: begin lowers SPI clock rate, end reinstates transmit clock rate
inline void begin_tft_read() __attribute__((always_inline));
inline void end_tft_read() __attribute__((always_inline));

// Initialise the data bus GPIO and hardware interfaces
void initBus(void);

// Temporary  library development function  TODO: remove need for this
void pushSwapBytePixels(const void *data_in, uint32_t len);

// Same as setAddrWindow but exits with CGRAM in read mode
void readAddrWindow(int32_t xs, int32_t ys, int32_t w, int32_t h);

// Byte read prototype
uint8_t readByte(void);

// GPIO parallel bus input/output direction control
void busDir(uint32_t mask, uint8_t mode);

// Single GPIO input/output direction control
void gpioMode(uint8_t gpio, uint8_t mode);

// Smooth graphics helper
uint8_t sqrt_fraction(uint32_t num);

// Helper function: calculate distance of a point from a finite length line between two points
float wedgeLineDistance(float pax, float pay, float bax, float bay, float dr);
