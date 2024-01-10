// Adopted by Bodmer to support TFT_eSPI library.

// Font structures for newer Adafruit_GFX (1.1 and later).
// Example fonts are included in 'Fonts' directory.
// To use a font in your Arduino sketch, #include the corresponding .h
// file and pass address of GFXfont struct to setFont().  Pass NULL to
// revert to 'classic' fixed-space bitmap font.
#pragma once

#ifdef LOAD_GFXFF

typedef struct { // Data stored PER GLYPH
    uint32_t bitmapOffset; // Pointer into GFXfont->bitmap
    uint8_t width, height; // Bitmap dimensions in pixels
    uint8_t xAdvance; // Distance to advance cursor (x axis)
    int8_t xOffset, yOffset; // Dist from cursor pos to UL corner
} GFXglyph;

typedef struct { // Data stored for FONT AS A WHOLE:
    uint8_t *bitmap; // Glyph bitmaps, concatenated
    GFXglyph *glyph; // Glyph array
    uint16_t first, last; // ASCII extents
    uint8_t yAdvance; // Newline distance (y axis)
} GFXfont;

extern const GFXfont TomThumb;

extern const GFXfont FreeMono9pt7b;
extern const GFXfont FreeMono12pt7b;
extern const GFXfont FreeMono18pt7b;
extern const GFXfont FreeMono24pt7b;

extern const GFXfont FreeMonoOblique9pt7b;
extern const GFXfont FreeMonoOblique12pt7b;
extern const GFXfont FreeMonoOblique18pt7b;
extern const GFXfont FreeMonoOblique24pt7b;

extern const GFXfont FreeMonoBold9pt7b;
extern const GFXfont FreeMonoBold12pt7b;
extern const GFXfont FreeMonoBold18pt7b;
extern const GFXfont FreeMonoBold24pt7b;

extern const GFXfont FreeMonoBoldOblique9pt7b;
extern const GFXfont FreeMonoBoldOblique12pt7b;
extern const GFXfont FreeMonoBoldOblique18pt7b;
extern const GFXfont FreeMonoBoldOblique24pt7b;

extern const GFXfont FreeSans9pt7b;
extern const GFXfont FreeSans12pt7b;
extern const GFXfont FreeSans18pt7b;
extern const GFXfont FreeSans24pt7b;

extern const GFXfont FreeSansOblique9pt7b;
extern const GFXfont FreeSansOblique12pt7b;
extern const GFXfont FreeSansOblique18pt7b;
extern const GFXfont FreeSansOblique24pt7b;

extern const GFXfont FreeSansBold9pt7b;
extern const GFXfont FreeSansBold12pt7b;
extern const GFXfont FreeSansBold18pt7b;
extern const GFXfont FreeSansBold24pt7b;

extern const GFXfont FreeSansBoldOblique9pt7b;
extern const GFXfont FreeSansBoldOblique12pt7b;
extern const GFXfont FreeSansBoldOblique18pt7b;
extern const GFXfont FreeSansBoldOblique24pt7b;

extern const GFXfont FreeSerif9pt7b;
extern const GFXfont FreeSerif12pt7b;
extern const GFXfont FreeSerif18pt7b;
extern const GFXfont FreeSerif24pt7b;

extern const GFXfont FreeSerifItalic9pt7b;
extern const GFXfont FreeSerifItalic12pt7b;
extern const GFXfont FreeSerifItalic18pt7b;
extern const GFXfont FreeSerifItalic24pt7b;

extern const GFXfont FreeSerifBold9pt7b;
extern const GFXfont FreeSerifBold12pt7b;
extern const GFXfont FreeSerifBold18pt7b;
extern const GFXfont FreeSerifBold24pt7b;

extern const GFXfont FreeSerifBoldItalic9pt7b;
extern const GFXfont FreeSerifBoldItalic12pt7b;
extern const GFXfont FreeSerifBoldItalic18pt7b;
extern const GFXfont FreeSerifBoldItalic24pt7b;

#endif // LOAD_GFXFF
