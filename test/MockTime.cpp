#include <cstdint>
#include <iostream>
#include "MockTime.h"
using namespace std;

static uint32_t timeInMicroseconds = 0U;

uint32_t MockTime::GetTimeInMicroseconds(void)
{
    cout << "time = " << timeInMicroseconds << endl;
    return timeInMicroseconds;
}

uint32_t MockTime::GetTimeInMilliseconds(void)
{
    uint32_t result = (timeInMicroseconds / 1000UL);
    cout << "time = " << result << endl;
    return result;
}

void MockTime::SetTimeInMicroseconds(uint32_t time)
{
    timeInMicroseconds = time;
}

void MockTime::SetTimeInMilliseconds(uint32_t time)
{
    timeInMicroseconds = time * 1000UL;
}