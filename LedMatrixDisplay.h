#ifndef LEDMATRIXDISPLAY_H
#define LEDMATRIXDISPLAY_H
#include <LEDMatrixDriver.hpp>
#include "SpeedMachine.h"

class LedMatrixDisplay : public SpeedMachine::Display
{
public:
    LedMatrixDisplay(uint8_t segments, uint8_t csPin);
    void setup(void);
    void clear(void);
    void showDecimal(double decimal, uint8_t precision);
    void doAnimation(void);

private:
    void drawString(String&);
    void drawSprite(byte* sprite, int x, int y, int width, int height);

private:
    LEDMatrixDriver display;
    static byte font[][8];
    static const int LEDMATRIX_SEGMENTS = 4;
    static const int LEDMATRIX_WIDTH =
        LEDMATRIX_SEGMENTS * 8; // segments * pixels
};
#endif //LEDMATRIXDISPLAY_H