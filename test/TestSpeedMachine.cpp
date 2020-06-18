#include <cassert>
#include <string>

#include "MockTime.h"
#include "../SpeedMachine.h"

#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"
using namespace std;

class MockBarrier : public SpeedMachine::Barrier
{
public:
    bool isEnabled(void) const
    {
        mock().actualCall("isEnabled").onObject(this);
        return enabled;
    }
    bool isTriggered(void) const {return triggered;}
    void enable(void)
    {
        mock().actualCall("enable").onObject(this);
        enabled = true;
    }
    void disable(void)
    {
        mock().actualCall("disable").onObject(this);
        enabled = false;
    }
    uint32_t triggeredAt(void)
    {
        mock().actualCall("triggeredAt").onObject(this);
        return triggeredTime;
    }
    // utilities accessible from tests...
    void simulateTriggered(void)
    {
        assert(controller && "No controller registered. Cannot simulate!");
        controller->barrierTriggered(this);
    }
    uint32_t triggeredTime = 0UL;
    bool enabled = false;
private:
    bool triggered = false;
};

class MockDisplay : public SpeedMachine::Display
{
public:
    void showDecimal(double decimal, uint8_t precision)
    {
        mock()
            .actualCall("showDecimal")
            .onObject(this)
            .withParameter("decimal", decimal)
            .withParameter("precision", precision);
        lastValue = decimal;
    }
    void showText(string text)
    {
        // mock()
        //     .actualCall("showText")
        //     .onObject(this)
        //     .withParameter("text", text);
    }
    void clear(void)
    {
        mock().actualCall("clear").onObject(this);
    }
    double lastValue = 0.0;
};

TEST_GROUP(SpeedMachineTestGroup)
{
    SpeedMachine::Controller* machine;
    MockBarrier* barrier1;
    MockBarrier* barrier2;
    MockDisplay* display;
    void setup()
    {
        barrier1 = new MockBarrier;
        barrier2 = new MockBarrier;
        display = new MockDisplay;
        machine = new SpeedMachine::Controller(barrier1, barrier2, display);
        machine->setUnits(SpeedMachine::Units::METRES_PER_SEC);
    }
    void teardown()
    {
        mock().clear();
        delete barrier1;
        delete barrier2;
        delete display;
        delete machine;
    }
};

TEST(SpeedMachineTestGroup, ShouldEnableBarriersAndClearDisplayOnStart)
{
    mock().expectOneCall("enable").onObject(barrier1);
    mock().expectOneCall("enable").onObject(barrier2);
    mock().expectOneCall("clear").onObject(display);
    machine->start();
    mock().checkExpectations();
}

TEST(SpeedMachineTestGroup, ShouldDisableBarriersOnStop)
{
    mock().expectOneCall("disable").onObject(barrier1);
    mock().expectOneCall("disable").onObject(barrier2);
    machine->stop();
    mock().checkExpectations();
}

TEST(SpeedMachineTestGroup, ShouldNotBeReadyWhenBarriersAreDisabled)
{
    // assume not ready on startup...
    mock().expectOneCall("isEnabled").onObject(barrier1);
    mock().expectOneCall("isEnabled").onObject(barrier2);
    CHECK(!machine->isReady());
}

TEST(SpeedMachineTestGroup, ShouldBeReadyWhenBarriersAreEnabled)
{
    barrier1->enabled = true;
    barrier2->enabled = true;
    mock().expectOneCall("isEnabled").onObject(barrier1);
    mock().expectOneCall("isEnabled").onObject(barrier2);
    CHECK(machine->isReady());
}

TEST(SpeedMachineTestGroup, BarriersTriggeredShouldDisplaySpeed)
{
    mock().disable();
    machine->start();
    mock().enable();
    // assuming that barriers are 1m apart and speed is measured in m/s
    barrier1->triggeredTime = 0UL;
    barrier2->triggeredTime = 1000UL;
    mock().expectOneCall("triggeredAt").onObject(barrier2);
    mock().expectOneCall("triggeredAt").onObject(barrier1);
    mock().expectOneCall("clear").onObject(display);
    mock().expectOneCall("showDecimal")
        .onObject(display)
        .withParameter("decimal", 1.0)
        .withParameter("precision", 1);
    barrier2->simulateTriggered();
    mock().checkExpectations();
}

TEST(
    SpeedMachineTestGroup,
    BarriersTriggeredReverseOrderShouldGetNegativeVelocity)
{
    // assume that the target crosses barrier 2 first.
    mock().disable();
    machine->start();
    mock().enable();
    barrier2->triggeredTime = 4320123UL;
    barrier1->triggeredTime = 432UL;  // triggered ages ago
    // Nothing should be written to the display until barrier 1 is then
    // triggered...
    mock().expectOneCall("triggeredAt").onObject(barrier2);
    mock().expectOneCall("triggeredAt").onObject(barrier1);
    barrier2->simulateTriggered();
    mock().checkExpectations();
    // barrier 1 is triggered and the display should be written to after times
    // are checked...
    mock().expectOneCall("triggeredAt").onObject(barrier2);
    mock().expectOneCall("triggeredAt").onObject(barrier1);
    mock().expectOneCall("clear").onObject(display);
    mock().expectOneCall("showDecimal")
        .onObject(display)
        .withParameter("decimal", -1.0)
        .withParameter("precision", 1);
    barrier1->triggeredTime = barrier2->triggeredTime + 1000UL;
    barrier1->simulateTriggered();
    mock().checkExpectations();
}

TEST(
    SpeedMachineTestGroup,
    BarriersTriggeredShouldNotNotify)
{
    // assume that the target crosses barrier 2 first.
    mock().disable();
    machine->start();
    mock().enable();
    barrier1->triggeredTime = 432UL;  // triggered ages ago
    barrier2->triggeredTime = 4320123UL;
    // Nothing should be written to the display because the transit is way too
    // long.
    mock().expectOneCall("triggeredAt").onObject(barrier2);
    mock().expectOneCall("triggeredAt").onObject(barrier1);
    barrier2->simulateTriggered();
    mock().checkExpectations();
}

TEST(SpeedMachineTestGroup, SettingUnitsToKmhSpeedShouldBeDisplayedInMph)
{
    mock().disable();
    machine->start();
    machine->setUnits(SpeedMachine::Units::KM_PER_HR);
    barrier1->triggeredTime = 0UL;
    barrier2->triggeredTime = 1000UL;
    barrier2->simulateTriggered();
    mock().enable();
    // 1 m/s = 3.6km/h
    DOUBLES_EQUAL(3.6, display->lastValue, 0.1);
}

TEST(SpeedMachineTestGroup, SettingUnitsToMphSpeedShouldBeDisplayedInMph)
{
    mock().disable();
    machine->start();
    machine->setUnits(SpeedMachine::Units::MI_PER_HR);
    barrier1->triggeredTime = 0UL;
    barrier2->triggeredTime = 1000UL;
    barrier2->simulateTriggered();
    mock().enable();
    // 1 m/s = 2.24 m/h
    DOUBLES_EQUAL(2.24, display->lastValue, 0.1);
}
