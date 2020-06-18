#include "Debounce.h"
#include "Osal.h"
#include "SonarBarrier.h"
using namespace SonarBarrier;

double Barrier::distanceToTriggerCm = 30.0;
uint32_t Barrier::debounceTimeMs = 100UL;

Barrier::Barrier(
    SonarDevice& device,
    Indicator& indicator) :
    _device(device), _indicator(indicator), _debouncer()
{
    _debouncer.registerListener(this);
}

bool Barrier::isEnabled(void) const
{
    return _device.isEnabled() && _indicator.isEnabled();
}

bool Barrier::isTriggered(void) const
{
    return _debouncer.isActivated();
}

void Barrier::enable(void)
{
    _device.enable();
    _device.registerListener(this);
    _indicator.enable();
}

void Barrier::disable(void)
{
    _device.disable();
    _indicator.disable();
}

uint32_t Barrier::triggeredAt(void)
{
    return _triggeredTimeUs;
}

void Barrier::handleDistanceNotification(double distance)
{
    bool targetInRange = distance < distanceToTriggerCm;
    if (targetInRange)
    {
        _debouncer.handleActivated();
    }
    else
    {
        _debouncer.handleDeactivated();
    }
}

void Barrier::handleActivated(void)
{
    _triggeredTimeUs = Osal::GetTimeInMicroseconds();
    _indicator.activate();
    if (listener) listener->handleBarrierTriggered(this);
}

void Barrier::handleDeactivated(void)
{
    _indicator.deactivate();
}
