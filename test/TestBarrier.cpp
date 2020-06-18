#include "MockTime.h"
#include "../SonarBarrier.h"
#include "../SpeedMachine.h"

#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

class MockSonarDevice : public SonarBarrier::SonarDevice
{
public:
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
    bool isEnabled(void) const
    {
        mock().actualCall("isEnabled").onObject(this);
        return enabled;
    }
    double getDistanceCm(void)
    {
        mock().actualCall("getDistanceCm").onObject(this);
        return distance;
    }
    // this method should not be here on the interface!!
    void handleEcho(uint32_t delayMicroseconds)
    {}
    void registerListener(SonarBarrier::DistanceListener* toRegister)
    {
        listener = toRegister;
    }
    double distance = 1.0E6; // this must be large to ensure it's not triggered
    bool enabled = false;
    SonarBarrier::DistanceListener* listener = NULL;
};

class MockIndicator : public SonarBarrier::Indicator
{
public:
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
    bool isEnabled(void)
    {
        mock().actualCall("isEnabled").onObject(this);
        return enabled;
    }
    void activate(void)
    {
        mock().actualCall("activate").onObject(this);
    }
    void deactivate(void)
    {
        mock().actualCall("deactivate").onObject(this);
    }
    bool enabled = false;
};

class MockBarrierListener : public SpeedMachine::BarrierListener
{
    void handleBarrierTriggered(SpeedMachine::Barrier* barrier)
    {
        mock()
            .actualCall("handleBarrierTriggered")
            .onObject(this)
            .withParameter("barrier", barrier);
    }
};

TEST_GROUP(BarrierTestGroup)
{
    MockSonarDevice* device;
    MockIndicator* indicator;
    MockBarrierListener* listener;
    SonarBarrier::Barrier* barrier;

    void setup()
    {
        MockTime::SetTimeInMilliseconds(0UL);
        device = new MockSonarDevice();
        indicator = new MockIndicator();
        barrier = new SonarBarrier::Barrier(*device, *indicator);
        listener = new MockBarrierListener();
        barrier->registerListener(listener);
    }
    void teardown()
    {
        mock().clear();
        delete device;
        delete indicator;
        delete listener;
        delete barrier;
    }
};

TEST(BarrierTestGroup, EnableBarrierEnablesDependencies)
{
    mock().expectOneCall("enable").onObject(device);
    mock().expectOneCall("enable").onObject(indicator);
    barrier->enable();
    mock().expectOneCall("isEnabled").onObject(device);
    mock().expectOneCall("isEnabled").onObject(indicator);
    CHECK(barrier->isEnabled());
    mock().checkExpectations();
}

TEST(BarrierTestGroup, DisableBarrierDisablesDependencies)
{
    mock().expectOneCall("disable").onObject(device);
    mock().expectOneCall("disable").onObject(indicator);
    barrier->disable();
    mock().checkExpectations();
}

TEST(BarrierTestGroup, ShouldBeDisabledByDefault)
{
    mock().disable();
    CHECK(!barrier->isEnabled());
    mock().enable();
}

TEST(BarrierTestGroup, DistanceToTargetTooLargeShouldNotTriggerNotNotify)
{
    barrier->handleDistanceNotification(
        SonarBarrier::Barrier::distanceToTriggerCm * 2.0);
    CHECK(!barrier->isTriggered());
    mock().checkExpectations();
}

TEST(BarrierTestGroup, DistanceToTargetBelowThresholdShouldTrigger)
{
    mock().disable();
    barrier->enable();
    CHECK_EQUAL(0UL, barrier->triggeredAt());
    double distanceToTarget = SonarBarrier::Barrier::distanceToTriggerCm / 2.0;
    device->distance = distanceToTarget;
    barrier->handleDistanceNotification(distanceToTarget);
    // fast forward time and barrier is still triggered - note that the input is
    // debounced and so needs to remain high long enough to transition
    MockTime::IncrementTimeInMicroseconds(1000UL);
    barrier->handleDistanceNotification(distanceToTarget);
    // the barrier should then report that it is triggered...
    CHECK(barrier->isTriggered());
    CHECK_EQUAL(1000UL, barrier->triggeredAt());
    mock().enable();
}

TEST(BarrierTestGroup, DistanceToTargetBelowThresholdShouldNotify)
{
    MockTime::SetTimeInMicroseconds(1000UL);
    // fast forward time and barrier is triggered...
    double distanceToTarget = SonarBarrier::Barrier::distanceToTriggerCm / 2.0;
    device->distance = distanceToTarget;
    barrier->handleDistanceNotification(distanceToTarget);
    // barrier remains triggered for longer than debounce time...
    MockTime::IncrementTimeInMilliseconds(
        SonarBarrier::Barrier::debounceTimeMs + 10.0);
    // another distance notification comes and should notify...
    // cue up notifications expected...
    mock().expectOneCall("handleBarrierTriggered")
        .onObject(listener)
        .withParameter("barrier", barrier);
    mock().expectOneCall("activate")
        .onObject(indicator);
    barrier->handleDistanceNotification(distanceToTarget);
    mock().checkExpectations();
}

TEST(BarrierTestGroup, DistanceSpikesDownShouldNotNotify)
{
    // the input to the barrier should be debounced so that short changes in the
    // signal are not registered as trigger events.
    MockTime::SetTimeInMilliseconds(100UL);
    // fast forward time and barrier is triggered...
    double distanceToTarget = SonarBarrier::Barrier::distanceToTriggerCm / 2.0;
    device->distance = distanceToTarget;
    barrier->handleDistanceNotification(distanceToTarget);
    // the distance suddenly increases again...
    MockTime::IncrementTimeInMilliseconds(
        SonarBarrier::Barrier::debounceTimeMs / 2);
    // fast forward time and barrier is triggered...
    distanceToTarget = SonarBarrier::Barrier::distanceToTriggerCm * 2.0;
    device->distance = distanceToTarget;
    barrier->handleDistanceNotification(distanceToTarget);
    // should never have been notified...
    mock().checkExpectations();
}