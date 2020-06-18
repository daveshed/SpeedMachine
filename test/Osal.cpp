#include <cstdint>

#include "MockTime.h"
#include "../Osal.h"

uint32_t Osal::GetTimeInMicroseconds(void)
{
    return MockTime::GetTimeInMicroseconds();
}

uint32_t Osal::GetTimeInMilliseconds(void)
{
    return MockTime::GetTimeInMilliseconds();
}

void Osal::SleepMicroseconds(uint32_t sleepFor)
{
    uint32_t currentTime = MockTime::GetTimeInMicroseconds();
    MockTime::SetTimeInMicroseconds(currentTime + sleepFor);
}