#include "ArduinoChannel.h"

ArduinoOutputChannel::ArduinoOutputChannel(int pin) : pin(pin)
{
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
}

bool ArduinoOutputChannel::IsActive(void) const
{
    return state;
}

void ArduinoOutputChannel::Activate(void)
{
    digitalWrite(pin, HIGH);
    state = true;
}

void ArduinoOutputChannel::Deactivate(void)
{
    digitalWrite(pin, LOW);
    state = false;
}

ArduinoInputChannel::ArduinoInputChannel(int pin) : pin(pin)
{
    pinMode(pin, INPUT);
    Read();
}

bool ArduinoInputChannel::Read(void)
{
    if (newState == currentState) return currentState;
    else if (currentState)
    {
        if (listener) listener->HandleFallingEdgeSignal();
    }
    else
    {
        if (listener) listener->HandleRisingEdgeSignal();
    }
    currentState = newState;
    return currentState;
}

void ArduinoInputChannel::RegisterListener(InputListener* toRegister)
{
    listener = toRegister;
}
