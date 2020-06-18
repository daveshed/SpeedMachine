#include <stdint.h>
#include "SonarBarrier.h"

class ArduinoLedIndicator : public SonarBarrier::Indicator
{
public:
    ArduinoLedIndicator(uint8_t pinIdx);
    void enable(void);
    void disable(void);
    bool isEnabled(void);
    void activate(void);
    void deactivate(void);

private:
    uint8_t _pinIdx;
};
