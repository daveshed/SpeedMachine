#include <stddef.h>

#include "Arduino.h"
#include "Osal.h"

uint32_t Osal::GetTimeInMicroseconds(void)
{
    return micros();
}

uint32_t Osal::GetTimeInMilliseconds(void)
{
    return millis();
}

void SleepMicroseconds(uint32_t sleepFor)
{
    delay(sleepFor);
}
