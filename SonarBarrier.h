#ifndef SONARBARRIER_H
#define SONARBARRIER_H
#include "Debounce.h"
#include "SpeedMachine.h"

namespace SonarBarrier
{
    class Indicator
    {
    public:
        virtual void enable(void) = 0;
        virtual void disable(void) = 0;
        virtual bool isEnabled(void) = 0;
        virtual void activate(void) = 0;
        virtual void deactivate(void) = 0;
    };

    class DistanceListener
    {
    public:
        virtual void handleDistanceNotification(double distance) = 0;
    };

    class SonarDevice
    {
    public:
        virtual void enable(void) = 0;
        virtual void disable(void) = 0;
        virtual bool isEnabled(void) const = 0;
        virtual double getDistanceCm(void) = 0;
        virtual void handleEcho(uint32_t delayMicroseconds) = 0;
        virtual void registerListener(DistanceListener*) = 0;
    };

    typedef enum {
        NORMAL,
        TRIGGERED,
        TRIGGERING,
        DETRIGGERING,
        UNKOWN,
    } State;

    class Barrier :
        public SpeedMachine::Barrier, public DistanceListener, public Debounce::InputListener
    {
    public:
        Barrier(SonarDevice&, Indicator&);
        bool isEnabled(void) const;
        bool isTriggered(void) const;
        void enable(void);
        void disable(void);
        uint32_t triggeredAt(void);
        void handleDistanceNotification(double distance);
        // InputListener...
        void handleActivated(void);
        void handleDeactivated(void);
        //...
        static double distanceToTriggerCm;
        static uint32_t debounceTimeMs;

    private:
        SonarDevice& _device;
        Indicator& _indicator;
        Debounce::Debouncer _debouncer;
        uint32_t _stateChangedAtMs;
        uint32_t _triggeredTimeUs = 0UL;
    };
}
#endif // SONARBARRIER_H