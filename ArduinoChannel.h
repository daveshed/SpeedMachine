#ifndef ARDUINOCHANNEL_H
#define ARDUINOCHANNEL_H
#include "Arduino.h"
#include "Channel.h"

class ArduinoOutputChannel : public OutputChannel
{
public:
    ArduinoOutputChannel(int pin);
    bool IsActive(void) const;
	void Activate(void);
    void Deactivate(void);
private:
    const int pin;
    bool state = false;
};

class ArduinoInputChannel : public InputChannel
{
public:
    ArduinoInputChannel(int pin);
    bool Read(void);
    void RegisterListener(InputListener* toRegister);
private:
    const int pin;
    bool newState = false;
    bool currentState = false;
    InputListener* listener = NULL;
};

#endif //ARDUINOCHANNEL_H
