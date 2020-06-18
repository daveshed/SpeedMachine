#ifndef MOCKTIME_H
#define MOCKTIME_H
#include <cstdint>

namespace MockTime
{
    uint32_t GetTimeInMicroseconds(void);
    uint32_t GetTimeInMilliseconds(void);
    void SetTimeInMicroseconds(uint32_t time);
    void SetTimeInMilliseconds(uint32_t time);
    void IncrementTimeInMilliseconds(uint32_t time);
    void IncrementTimeInMicroseconds(uint32_t time);
}
#endif // MOCKTIME_H