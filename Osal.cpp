#include "Arduino.h"
#include "Osal.h"

unsigned long Osal::GetTimeInMicroseconds(void)
{
    return micros();
}