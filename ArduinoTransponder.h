#ifndef ARDUINOTRANSPONDER_H
#define ARDUINOTRANSPONDER_H
#include <stdint.h>
#include "UltrasonicSensor.h"

class ArduinoTransmitter : public Transmitter {
public:
    ArduinoTransmitter(uint8_t pin);
    bool IsEnabled(void) const;
    void Enable(void);
    void Disable(void);
private:
    const uint8_t pin;
    bool enabled = false;
    void DisableTimer1Pwm(void);
    void ConfigureTimer1Pwm(void);
};

class ArduinoReceiver : public Receiver {
public:
    ArduinoReceiver(uint8_t pin);
    bool IsEnabled(void) const;
    void Enable();
    void Disable(void);
    void HandleEcho(void);
    void HandlePing(void);
    void RegisterListener(EchoListener* toRegister);
private:
    const uint8_t pin;
    bool enabled = false;
    uint32_t initialTime = 0;
    EchoListener* listener = NULL;
    static void HandleInterrupt(void* context);
};

#endif // ARDUINOTRANSPONDER_H
