#include <stdint.h>
#include "Arduino.h"
#include "ArduinoLed.h"

ArduinoLedIndicator::ArduinoLedIndicator(
    uint8_t pinIdx) : _pinIdx(pinIdx)
{}

void ArduinoLedIndicator::enable(void)
{
    pinMode(_pinIdx, OUTPUT);
}

bool ArduinoLedIndicator::isEnabled(void)
{
    return digitalRead(_pinIdx);
}

void ArduinoLedIndicator::disable(void)
{
    deactivate();
}

void ArduinoLedIndicator::activate(void)
{
    digitalWrite(_pinIdx, HIGH);
}

void ArduinoLedIndicator::deactivate(void)
{
    digitalWrite(_pinIdx, LOW);
}
