#include "Osal.h"
#include "SonarBarrier.h"
#include "UltrasonicSensor.h"
#define MAXIMUM_FLIGHT_TIME 23256U

using namespace UltrasonicSensor;

Sensor::Sensor(
    Receiver& receiver, Transmitter& transmitter):
        receiver(receiver), transmitter(transmitter)
{
    transmitter.disable();
    receiver.registerListener(this);
    receiver.disable();
}

void Sensor::enable(void)
{
    transmitter.enable();
    receiver.enable();
}

void Sensor::disable(void)
{}

double Sensor::getDistanceCm(void)
{
    return distance;
}

void Sensor::handleEcho(uint32_t delayMicroseconds)
{
    distance = (delayMicroseconds * 0.0344) / 2.0;
    if (listener)
    {
        listener->handleDistanceNotification(distance);
    }
}

void Sensor::registerListener(SonarBarrier::DistanceListener* toRegister)
{
    listener = toRegister;
}