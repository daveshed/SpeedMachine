#include "MockTime.h"
#include "../Osal.h"

// TODO - should use stdint!
unsigned long Osal::GetTimeInMicroseconds(void)
{
    return MockTime::GetTimeInMicroseconds();
}

void Osal::SleepMicroseconds(unsigned long sleepFor)
{
    unsigned long currentTime = MockTime::GetTimeInMicroseconds();
    MockTime::SetTimeInMicroseconds(currentTime + sleepFor);
}