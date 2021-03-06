#include "Arduino.h"
#include "LedMatrixDisplay.h"

byte LedMatrixDisplay::font[95][8] = {
    {0,0,0,0,0,0,0,0}, // SPACE
    {0x10,0x18,0x18,0x18,0x18,0x00,0x18,0x18}, // EXCL
    {0x28,0x28,0x08,0x00,0x00,0x00,0x00,0x00}, // QUOT
    {0x00,0x0a,0x7f,0x14,0x28,0xfe,0x50,0x00}, // #
    {0x10,0x38,0x54,0x70,0x1c,0x54,0x38,0x10}, // $
    {0x00,0x60,0x66,0x08,0x10,0x66,0x06,0x00}, // %
    {0,0,0,0,0,0,0,0}, // &
    {0x00,0x10,0x18,0x18,0x08,0x00,0x00,0x00}, // '
    {0x02,0x04,0x08,0x08,0x08,0x08,0x08,0x04}, // (
    {0x40,0x20,0x10,0x10,0x10,0x10,0x10,0x20}, // )
    {0x00,0x10,0x54,0x38,0x10,0x38,0x54,0x10}, // *
    {0x00,0x08,0x08,0x08,0x7f,0x08,0x08,0x08}, // +
    {0x00,0x00,0x00,0x00,0x00,0x18,0x18,0x08}, // COMMA
    {0x00,0x00,0x00,0x00,0x7e,0x00,0x00,0x00}, // -
    {0x00,0x00,0x00,0x00,0x00,0x00,0x06,0x06}, // DOT
    {0x00,0x04,0x04,0x08,0x10,0x20,0x40,0x40}, // /
    {0x00,0x38,0x44,0x4c,0x54,0x64,0x44,0x38}, // 0
    {0x04,0x0c,0x14,0x24,0x04,0x04,0x04,0x04}, // 1
    {0x00,0x30,0x48,0x04,0x04,0x38,0x40,0x7c}, // 2
    {0x00,0x38,0x04,0x04,0x18,0x04,0x44,0x38}, // 3
    {0x00,0x04,0x0c,0x14,0x24,0x7e,0x04,0x04}, // 4
    {0x00,0x7c,0x40,0x40,0x78,0x04,0x04,0x38}, // 5
    {0x00,0x38,0x40,0x40,0x78,0x44,0x44,0x38}, // 6
    {0x00,0x7c,0x04,0x04,0x08,0x08,0x10,0x10}, // 7
    {0x00,0x3c,0x44,0x44,0x38,0x44,0x44,0x78}, // 8
    {0x00,0x38,0x44,0x44,0x3c,0x04,0x04,0x78}, // 9
    {0x00,0x18,0x18,0x00,0x00,0x18,0x18,0x00}, // :
    {0x00,0x18,0x18,0x00,0x00,0x18,0x18,0x08}, // ;
    {0x00,0x10,0x20,0x40,0x80,0x40,0x20,0x10}, // <
    {0x00,0x00,0x7e,0x00,0x00,0xfc,0x00,0x00}, // =
    {0x00,0x08,0x04,0x02,0x01,0x02,0x04,0x08}, // >
    {0x00,0x38,0x44,0x04,0x08,0x10,0x00,0x10}, // ?
    {0x00,0x30,0x48,0xba,0xba,0x84,0x78,0x00}, // @
    {0x00,0x1c,0x22,0x42,0x42,0x7e,0x42,0x42}, // A
    {0x00,0x78,0x44,0x44,0x78,0x44,0x44,0x7c}, // B
    {0x00,0x3c,0x44,0x40,0x40,0x40,0x44,0x7c}, // C
    {0x00,0x7c,0x42,0x42,0x42,0x42,0x44,0x78}, // D
    {0x00,0x78,0x40,0x40,0x70,0x40,0x40,0x7c}, // E
    {0x00,0x7c,0x40,0x40,0x78,0x40,0x40,0x40}, // F
    {0x00,0x3c,0x40,0x40,0x5c,0x44,0x44,0x78}, // G
    {0x00,0x42,0x42,0x42,0x7e,0x42,0x42,0x42}, // H
    {0x00,0x7c,0x10,0x10,0x10,0x10,0x10,0x7e}, // I
    {0x00,0x7e,0x02,0x02,0x02,0x02,0x04,0x38}, // J
    {0x00,0x44,0x48,0x50,0x60,0x50,0x48,0x44}, // K
    {0x00,0x40,0x40,0x40,0x40,0x40,0x40,0x7c}, // L
    {0x00,0x82,0xc6,0xaa,0x92,0x82,0x82,0x82}, // M
    {0x00,0x42,0x42,0x62,0x52,0x4a,0x46,0x42}, // N
    {0x00,0x3c,0x42,0x42,0x42,0x42,0x44,0x38}, // O
    {0x00,0x78,0x44,0x44,0x48,0x70,0x40,0x40}, // P
    {0x00,0x3c,0x42,0x42,0x52,0x4a,0x44,0x3a}, // Q
    {0x00,0x78,0x44,0x44,0x78,0x50,0x48,0x44}, // R
    {0x00,0x38,0x40,0x40,0x38,0x04,0x04,0x78}, // S
    {0x00,0x7e,0x90,0x10,0x10,0x10,0x10,0x10}, // T
    {0x00,0x42,0x42,0x42,0x42,0x42,0x42,0x3e}, // U
    {0x00,0x42,0x42,0x42,0x42,0x44,0x28,0x10}, // V
    {0x80,0x82,0x82,0x92,0x92,0x92,0x94,0x78}, // W
    {0x00,0x42,0x42,0x24,0x18,0x24,0x42,0x42}, // X
    {0x00,0x44,0x44,0x28,0x10,0x10,0x10,0x10}, // Y
    {0x00,0x7c,0x04,0x08,0x7c,0x20,0x40,0xfe}, // Z
};    // {}, //

LedMatrixDisplay::LedMatrixDisplay(uint8_t segments, uint8_t csPin)
    : display(segments, csPin) {}

void LedMatrixDisplay::setup(void)
{
    display.setEnabled(true);
    display.setIntensity(8);   // 0 = low, 10 = high
    display.clear();
}

void LedMatrixDisplay::clear(void)
{
    display.clear();
}

void LedMatrixDisplay::showDecimal(double decimal, uint8_t precision)
{
    String toDisplay = String((decimal), precision);
    drawString(toDisplay);
}

void LedMatrixDisplay::drawString(String& toDraw)
{
    int xOffset = 0;
    int yOffset = 0;
    char text[LEDMATRIX_SEGMENTS] = {'\0'};
    int len = toDraw.length();
    toDraw.toCharArray(text, (toDraw.length() + 1));
    for( int idx = 0; idx < len; idx ++ )
    {
        int c = text[idx] - 32;
        // stop if char is outside visible area
        if( xOffset + idx * 8  > LEDMATRIX_WIDTH ) return;
        // only draw if char is visible
        if( 8 + xOffset + idx * 8 > 0 ) drawSprite(
            font[c], xOffset + idx * 8, yOffset, 8, 8 );
    }
}

byte a[8]={ B00011000,
            B00100100,
            B00100100,
            B00011000,
            B01111110,
            B00011000,
            B00100100,
            B01000010};

byte b[8]={ B00011000,
            B00100100,
            B00100100,
            B00011010,
            B01111100,
            B00011000,
            B01100100,
            B00000010};

byte c[8]={ B00011000,
            B00100100,
            B00100100,
            B00011010,
            B00111100,
            B01011000,
            B00110100,
            B00000100};

byte d[8]={ B00011000,
            B00100100,
            B00100100,
            B00011010,
            B00111100,
            B01011000,
            B00011000,
            B00011000};

byte e[8]={ B00011000,
            B00100100,
            B00100100,
            B00011010,
            B00111100,
            B01011000,
            B00010100,
            B00010000};

byte f[8]={ B00011000,
            B00100100,
            B00100100,
            B00011000,
            B00111110,
            B01011000,
            B00010100,
            B00010100};


void LedMatrixDisplay::doAnimation(void)
{
    const int ANIM_DELAY = 100;
    int x = 0;
    while (x < LEDMATRIX_WIDTH)
    {
        drawSprite( (byte*)&a, x++, 0, 8, 8 );
        display.display();
        delay(ANIM_DELAY);

        display.clear();
        drawSprite( (byte*)&b, x++, 0, 8, 8 );
        display.display();
        delay(ANIM_DELAY);

        display.clear();
        drawSprite( (byte*)&c, x++, 0, 8, 8 );
        display.display();
        delay(ANIM_DELAY);

        display.clear();
        drawSprite( (byte*)&d, x++, 0, 8, 8 );
        display.display();
        delay(ANIM_DELAY);

        display.clear();
        drawSprite( (byte*)&e, x++, 0, 8, 8 );
        display.display();
        delay(ANIM_DELAY);

        display.clear();
        drawSprite( (byte*)&f, x++, 0, 8, 8 );
        display.display();
        delay(ANIM_DELAY);
    }
}

void LedMatrixDisplay::drawSprite( byte* sprite, int x, int y, int width, int height )
{
    // The mask is used to get the column bit from the sprite row
    byte mask = B10000000;

    for( int iy = 0; iy < height; iy++ )
    {
        for( int ix = 0; ix < width; ix++ )
        {
            display.setPixel(x + ix, y + iy, (bool)(sprite[iy] & mask ));
            // shift the mask by one pixel to the right
            mask = mask >> 1;
        }
        // reset column mask
        mask = B10000000;
    }
    display.display();
}
