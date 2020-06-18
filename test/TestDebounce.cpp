#include "MockTime.h"
#include "../Debounce.h"

#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"
using namespace std;

class MockListener : public Debounce::InputListener
{
public:
    void handleActivated(void)
    {
        mock().actualCall("handleActivated").onObject(this);
    }
    void handleDeactivated(void)
    {
        mock().actualCall("handleDeactivated").onObject(this);
    }
};

TEST_BASE(DebouncerTestBase)
{
    Debounce::Debouncer* debouncer;
    MockListener* debouncedListener;
    void setup(void)
    {
        mock().disable();
        debouncer = new Debounce::Debouncer();
        debouncedListener = new MockListener();
        debouncer->registerListener(debouncedListener);
        mock().enable();
        MockTime::SetTimeInMicroseconds(0U);
    }
    void teardown(void)
    {
        delete debouncer;
        delete debouncedListener;
        mock().clear();
    }
};

TEST_GROUP_BASE(DebouncerAfterInitTestGroup, DebouncerTestBase)
{};

TEST(DebouncerAfterInitTestGroup, IsActivatedShouldNotReadInputState)
{
    // should never read the input state directly but return some cached value.
    // remember that the raw input is fluctuating and needs to be debounced.
    debouncer->isActivated();
    mock().checkExpectations();
}

TEST(DebouncerAfterInitTestGroup, ShouldBeInactiveOnInit)
{
    // should assume inactive initially. dangerous?...
    CHECK(!debouncer->isActivated());
}

TEST(DebouncerAfterInitTestGroup, InputHighForDebounceTimeShouldGoActive)
{
    // t = 0
    debouncer->handleActivated();
    MockTime::IncrementTimeInMicroseconds(
        debouncer->getDebounceTimeUs() + 1U);
    // t > debounce time - should call the listener...
    mock().expectOneCall("handleActivated").onObject(debouncedListener);
    debouncer->handleActivated();
    mock().checkExpectations();
    CHECK(debouncer->isActivated());
}

TEST(DebouncerAfterInitTestGroup, InputHighLessThanDebounceTimeShouldBeInactive)
{
    // t = 0
    debouncer->handleActivated();
    MockTime::IncrementTimeInMicroseconds(
        debouncer->getDebounceTimeUs() - 1U);
    // t , debounce time - should not call the listener...
    mock().checkExpectations();
    CHECK(!debouncer->isActivated());
}

TEST(DebouncerAfterInitTestGroup, InputHighForDebounceTimeShouldGoActiveNoListener)
{
    debouncer->deregisterListener();
    // t = 0
    debouncer->handleActivated();
    MockTime::IncrementTimeInMicroseconds(
        debouncer->getDebounceTimeUs() + 1U);
    // t > debounce time - should not call listener. Not registered.
    debouncer->handleActivated();
    mock().checkExpectations();
    CHECK(debouncer->isActivated());
}

TEST(DebouncerAfterInitTestGroup, InputStaysLowShouldStayInactive)
{
    // mock listener is never called...
    debouncer->handleDeactivated();
    MockTime::IncrementTimeInMicroseconds(
        debouncer->getDebounceTimeUs() - 1U);
    CHECK(!debouncer->isActivated());
    MockTime::IncrementTimeInMicroseconds(2U);
    CHECK(!debouncer->isActivated());
}

TEST(DebouncerAfterInitTestGroup, HighInputSpikeShouldNotGoActiveResetsTimer)
{
    // a short high spike comes from the input...
    debouncer->handleActivated();
    MockTime::IncrementTimeInMicroseconds(5UL);
    // then input goes low again...
    debouncer->handleDeactivated();
    // before another high input comes...
    MockTime::IncrementTimeInMicroseconds(5UL);
    debouncer->handleActivated();
    // expect to wait the full debounce time before the listener is notified...
    const uint32_t t0 = MockTime::GetTimeInMicroseconds();
    uint32_t t = t0;
    uint32_t elapsed = 0UL;
    while (true)
    {
        elapsed = ++t - t0;
        MockTime::SetTimeInMicroseconds(t);
        if (elapsed > debouncer->getDebounceTimeUs())
        {
            break;
        }
        debouncer->handleActivated();
        CHECK(!debouncer->isActivated());
    }
    CHECK(elapsed > debouncer->getDebounceTimeUs());
    mock().expectOneCall("handleActivated").onObject(debouncedListener);
    debouncer->handleActivated();
    CHECK(debouncer->isActivated());
}

TEST_GROUP_BASE(DebouncerActiveTestGroup, DebouncerTestBase)
{
    void activateDebouncer()
    {
        mock().disable();
        debouncer->handleActivated();
        MockTime::IncrementTimeInMicroseconds(
            debouncer->getDebounceTimeUs() + 1U);
        debouncer->handleActivated();
        mock().enable();
    }

    void setup()
    {
        DebouncerTestBase::setup();
        activateDebouncer();
    }
};

TEST(DebouncerActiveTestGroup, IsActivatedShouldNotReadInputState)
{
    // should never read the input state directly but return some cached value.
    // remember that the raw input is fluctuating and needs to be debounced.
    debouncer->isActivated();
    mock().checkExpectations();
}

TEST(DebouncerActiveTestGroup, InputLowForDebounceTimeShouldGoInactive)
{
    // t = t0
    debouncer->handleDeactivated();
    MockTime::IncrementTimeInMicroseconds(
        debouncer->getDebounceTimeUs() + 1U);
    // t > debounce time - should call the listener...
    mock().expectOneCall("handleDeactivated").onObject(debouncedListener);
    debouncer->handleDeactivated();
    mock().checkExpectations();
    CHECK(!debouncer->isActivated());
}

TEST(DebouncerActiveTestGroup, LowInputSpikeShouldNotGoInactiveResetsTimer)
{
    // debouncer is initially activated.
    // a short low spike comes from the input...
    debouncer->handleDeactivated();
    MockTime::IncrementTimeInMicroseconds(5UL);
    // then input goes back high again...
    debouncer->handleActivated();
    // before another low input comes...
    MockTime::IncrementTimeInMicroseconds(5UL);
    debouncer->handleDeactivated();
    // expect to wait the full debounce time before the listener is notified...
    const uint32_t t0 = MockTime::GetTimeInMicroseconds();
    uint32_t t = t0;
    uint32_t elapsed = 0UL;
    while (true)
    {
        elapsed = ++t - t0;
        MockTime::SetTimeInMicroseconds(t);
        if (elapsed > debouncer->getDebounceTimeUs())
        {
            break;
        }
        debouncer->handleDeactivated();
        // stays activated even though input is deactivated until full debounce
        // time has elapsed.
        CHECK(debouncer->isActivated());
    }
    CHECK(elapsed > debouncer->getDebounceTimeUs());
    mock().expectOneCall("handleDeactivated").onObject(debouncedListener);
    debouncer->handleDeactivated();
    CHECK(!debouncer->isActivated());
}
