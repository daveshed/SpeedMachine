#include <stdint.h>
#include "Arduino.h"
#include "ArduinoTransponder.h"

#define ENABLE_INTERRUPTS    sei()
#define DISABLE_INTERRUPTS   cli()

ArduinoTransmitter::ArduinoTransmitter(uint8_t pin) : pin(pin)
{}

bool ArduinoTransmitter::IsEnabled(void) const
{
    return enabled;
}

void ArduinoTransmitter::Enable(void)
{
    Serial.println("Enabling transmitter...");
    if (enabled) return;
    ArduinoTransmitter::ConfigureTimer1Pwm();
    enabled = true;
}

void ArduinoTransmitter::Disable(void)
{
    Serial.println("Disabling transmitter...");
    if (!enabled) return;
    ArduinoTransmitter::DisableTimer1Pwm();
    enabled = false;
}

void ArduinoTransmitter::DisableTimer1Pwm(void)
{
    DISABLE_INTERRUPTS;
    TCCR1A = 0;
    TCCR1B = 0;
    ENABLE_INTERRUPTS;
}

void ArduinoTransmitter::ConfigureTimer1Pwm(void)
{
    Serial.println("Configuring pwm on timer1...");
    DISABLE_INTERRUPTS;
    pinMode(pin, OUTPUT);
    TCCR1A = _BV(COM1A1) | _BV(COM1B1) |_BV(WGM10) | _BV(WGM11);
    TCCR1B = _BV(WGM12) | _BV(WGM13) | _BV(CS11);
    OCR1A = 47903; //TODO remove these magic numbers
    OCR1B = 19;
    ENABLE_INTERRUPTS;
}

ArduinoReceiver::ArduinoReceiver(uint8_t pin) : pin(pin)
{}

bool ArduinoReceiver::IsEnabled(void) const
{
    return enabled;
}

void ArduinoReceiver::Enable(void)
{
    Serial.println("Enabling receiver...");
    if (enabled) return;
    // fix this...enabling is done externally right now. How could this class do it.
//    DISABLE_INTERRUPTS;
//    pinMode(pin, INPUT);
//    attachInterrupt(
//      digitalPinToInterrupt(pin),
//      HandleInterrupt,
//      CHANGE);
//    ENABLE_INTERRUPTS;
//    enabled = true;
}

void ArduinoReceiver::Disable(void)
{
    if (!enabled) return;
    DISABLE_INTERRUPTS;
    detachInterrupt(digitalPinToInterrupt(pin));
    ENABLE_INTERRUPTS;
    enabled = false;
}

void ArduinoReceiver::HandleEcho(void)
{
    //Serial.println("Handling echo...");
    if (listener)
    {
        uint32_t elapsedTime = micros() - initialTime;
        listener->HandleEcho(elapsedTime);
    }
}

void ArduinoReceiver::HandlePing(void)
{
    //Serial.println("Handling ping...");
    initialTime = micros();
}

void ArduinoReceiver::RegisterListener(EchoListener* toRegister)
{
    listener = toRegister;
}
