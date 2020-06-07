#include "Osal.h"
#include "UltrasonicSensor.h"
#define MAXIMUM_FLIGHT_TIME 23256U

using namespace std;

UltrasonicSensor::UltrasonicSensor(
    Receiver& receiver, Transmitter& transmitter):
        receiver(receiver), transmitter(transmitter)
{
    transmitter.Disable();
    receiver.RegisterListener(this);
    receiver.Disable();
}

void UltrasonicSensor::Enable(void)
{
    transmitter.Enable();
    receiver.Enable();
}

void UltrasonicSensor::Disable(void)
{}

double UltrasonicSensor::GetDistanceCm(void)
{
    return distance;
}

void UltrasonicSensor::HandleEcho(uint32_t delayMicroseconds)
{
    distance = (delayMicroseconds * 0.0344) / 2.0;
}
