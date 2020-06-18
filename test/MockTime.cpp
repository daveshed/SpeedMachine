#include <cstdint>
#include <iostream>
#include "MockTime.h"
#define MS_TO_USEC 1000UL
using namespace std;

static uint32_t timeInMicroseconds = 0U;

uint32_t MockTime::GetTimeInMicroseconds(void)
{
    cout << "time = " << timeInMicroseconds << endl;
    return timeInMicroseconds;
}

uint32_t MockTime::GetTimeInMilliseconds(void)
{
    uint32_t result = (timeInMicroseconds / MS_TO_USEC);
    cout << "time = " << result << "ms" << endl;
    return result;
}

void MockTime::SetTimeInMicroseconds(uint32_t time)
{
    timeInMicroseconds = time;
}

void MockTime::SetTimeInMilliseconds(uint32_t time)
{
    timeInMicroseconds = time * MS_TO_USEC;
}

void MockTime::IncrementTimeInMilliseconds(uint32_t time)
{
    timeInMicroseconds += time * MS_TO_USEC;
}

void MockTime::IncrementTimeInMicroseconds(uint32_t time)
{
    timeInMicroseconds += time;
}
