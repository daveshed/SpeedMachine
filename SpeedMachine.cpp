#include <stdlib.h>

#include "Logging.h"
#include "SpeedMachine.h"
using namespace SpeedMachine;

uint32_t Controller::maxTransitSec = 5UL;
double Controller::barrierDistanceMetres = 1.0;
Units Controller::units = Units::METRES_PER_SEC;
double Controller::conversionFactors[Units::NUM_UNITS] = {1.0, 3.6, 2.23694};

    typedef enum {
        METRES_PER_SEC,
        KM_PER_HR,
        MI_PER_HR,
        NUM_UNITS,
    } Units;


Controller::Controller(
    Barrier* barrier1,
    Barrier* barrier2,
    Display* display) :
        barrier1(barrier1), barrier2(barrier2), display(display)
{}

void Controller::start(void)
{
    barrier1->registerListener(this);
    barrier2->registerListener(this);
    barrier1->enable();
    barrier2->enable();
    display->clear();
    display->doAnimation();
}

bool Controller::isReady(void)
{
    return (barrier1->isEnabled() && barrier2->isEnabled());
}

void Controller::stop(void)
{
    barrier1->disable();
    barrier2->disable();
}

void Controller::setUnits(SpeedMachine::Units newUnit)
{
    units = newUnit;
}

void Controller::setBarrierDistanceM(double value)
{
    barrierDistanceMetres = value;
}

void Controller::handleBarrierTriggered(Barrier* barrier)
{
    int32_t transitUs = barrier2->triggeredAt() - barrier1->triggeredAt();
    double transitSec = transitUs / 1E6L;
    bool tookAges = abs(transitSec) > maxTransitSec;
    if (tookAges) return;
    double velocity = calculateVelocity(transitSec);
    display->clear();
    display->showDecimal(velocity, 1U);
    LOG_DEBUG("Transit -> ");
    LOG_DEBUG(transitUs);
    LOG_DEBUG("us\n");
    LOG_DEBUG("Got velocity -> ");
    LOG_DEBUG(velocity);
    LOG_DEBUG("units\n");
}

double Controller::calculateVelocity(double transitSec)
{
    return barrierDistanceMetres / transitSec * conversionFactors[units];
}