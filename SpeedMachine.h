#ifndef SPEEDMACHINE_H
#define SPEEDMACHINE_H
#include <stddef.h>
#include <stdint.h>
#include <string.h>
using namespace std;

namespace SpeedMachine
{
    class Barrier;
    class BarrierListener;
    class Controller;
    class Display;

    typedef enum {
        METRES_PER_SEC,
        KM_PER_HR,
        MI_PER_HR,
        NUM_UNITS,
    } Units;

    class BarrierListener
    {
    public:
        virtual void handleBarrierTriggered(Barrier*) = 0;
    };

    class Barrier
    {
    public:
        virtual bool isEnabled(void) const = 0;
        virtual bool isTriggered(void) const = 0;
        virtual void enable(void) = 0;
        virtual void disable(void) = 0;
        virtual uint32_t triggeredAt(void) = 0;
        void registerListener(BarrierListener* toRegister) {listener = toRegister;}
    protected:
        BarrierListener* listener = NULL;
    };

    class Display
    {
    public:
        virtual void clear(void) = 0;
        virtual void showDecimal(double decimal, uint8_t precision) = 0;
        virtual void doAnimation(void) = 0;
    };

    class Controller : public BarrierListener
    {
    public:
        Controller(
            Barrier* barrier1, Barrier* barrier2, Display* display);// :
        void start(void);
        bool isReady(void);
        void stop(void);
        void setUnits(Units);
        void setBarrierDistanceM(double);
        // need to obscure this somehow...
        void handleBarrierTriggered(Barrier* barrier);
    private:
        double calculateVelocity(double transitSec);
        static uint32_t maxTransitSec;
        static double barrierDistanceMetres;
        static double conversionFactors[];
        static Units units;
        Barrier* barrier1;
        Barrier* barrier2;
        Display* display;
    };
}

#endif // SPEEDMACHINE_H