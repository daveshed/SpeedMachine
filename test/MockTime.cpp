#include <iostream>
#include "MockTime.h"
using namespace std;

static unsigned long timeInMicroseconds = 0U;

unsigned long MockTime::GetTimeInMicroseconds(void)
{
    cout << "time = " << timeInMicroseconds << endl;
    return timeInMicroseconds;
}

void MockTime::SetTimeInMicroseconds(unsigned long time)
{
    timeInMicroseconds = time;
}