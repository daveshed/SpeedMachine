#ifndef ULTRASONICSENSOR_H
#define ULTRASONICSENSOR_H
#include <stddef.h>
#include <stdint.h>

#include "SonarBarrier.h"

namespace UltrasonicSensor
{
    class Transmitter {
    public:
        virtual bool isEnabled(void) const = 0;
        virtual void enable(void) = 0;
        virtual void disable(void) = 0;
    };

    // should remove this - the Sensor could just be a listener of the receiver.
    class EchoListener {
    public:
        virtual void handleEcho(uint32_t delayMicroseconds) = 0;
    };

    class Receiver {
    public:
        virtual bool isEnabled(void) const = 0;
        virtual void enable(void) = 0;
        virtual void disable(void) = 0;
        virtual void handleEcho(void) = 0;
        virtual void handlePing(void) = 0;
        virtual void registerListener(EchoListener*) = 0;
    };

    class Sensor : public EchoListener, public SonarBarrier::SonarDevice
    {
    public:
        Sensor(Receiver& receiver, Transmitter& transmitter);
        void enable(void);
        void disable(void);
        bool isEnabled(void) const {return enabled; }
        double getDistanceCm(void);
        void handleEcho(uint32_t delayMicroseconds);
        void registerListener(SonarBarrier::DistanceListener*);
    private:
        Receiver& receiver;
        Transmitter& transmitter;
        SonarBarrier::DistanceListener* listener = NULL;
        double distance = 0.0;
        bool enabled = false;
    };
}
#endif // ULTRASONICSENSOR_H
