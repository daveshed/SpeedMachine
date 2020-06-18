#ifndef OSAL_H
#define OSAL_H
#include <cstdint>

namespace Osal
{
    uint32_t GetTimeInMicroseconds(void);
    uint32_t GetTimeInMilliseconds(void);
    void SleepMicroseconds(uint32_t sleepFor);
}
#endif // OSAL_H