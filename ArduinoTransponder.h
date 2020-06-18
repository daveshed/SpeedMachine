#ifndef ARDUINOTRANSPONDER_H
#define ARDUINOTRANSPONDER_H
#include <stdint.h>
#include "UltrasonicSensor.h"
using namespace UltrasonicSensor;

class ArduinoTransmitter : public Transmitter {
public:
    ArduinoTransmitter(uint8_t pin);
    bool isEnabled(void) const;
    void enable(void);
    void disable(void);
private:
    const uint8_t pin;
    bool enabled = false;
    void disableTimer1Pwm(void);
    void configureTimer1Pwm(void);
};

class ArduinoReceiver : public Receiver {
public:
    ArduinoReceiver(uint8_t pin);
    bool isEnabled(void) const;
    void enable();
    void disable(void);
    void handleEcho(void);
    void handlePing(void);
    void registerListener(EchoListener* toRegister);
private:
    const uint8_t pin;
    bool enabled = false;
    uint32_t initialTime = 0;
    EchoListener* listener = NULL;
};

#endif // ARDUINOTRANSPONDER_H
