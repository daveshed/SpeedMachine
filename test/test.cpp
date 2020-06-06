#include <iostream>
#include <limits>

#include "MockTime.h"
#include "../Osal.h"
#include "../UltrasonicSensor.h"

#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"
using namespace std;

class MockTransmitter : public Transmitter
{
public:
    bool IsEnabled(void) const {return enabled;}
    void Enable(void)
    {
        mock().actualCall("Enable").onObject(this);
        enabled = true;
    }
    void Disable(void) {
        mock().actualCall("Disable").onObject(this);
        enabled = false;
    }
    unsigned long GetPingElapsedTimeMicroseconds(void) const {
        return elapsedTime;
    }
    // make the time public so that the tests can modify it...
    unsigned long elapsedTime = 0U;
private:
    bool enabled = false;
};

class MockReceiver : public Receiver
{
public:
    bool IsEnabled(void) const {return enabled;}
    void Enable(void) {
        mock().actualCall("Enable").onObject(this);
        enabled = true;
    }
    void Disable(void)
    {
        mock().actualCall("Disable").onObject(this);
        enabled = false;
    }
    void RegisterListener(EchoListener* toRegister) {listener = toRegister;}
    // make the listener public for the purposes of tests. This way we can
    // easily simulate the arrival of an echo or a timeout because one doesn't
    // arrive when it should do.
    EchoListener* GetEchoListener(void) {return listener;}
private:
    EchoListener* listener = NULL;
    bool enabled = false;
};

TEST_GROUP(SensorSetupTestGroup)
{
    void setup()
    {
        MockTime::SetTimeInMicroseconds(0U);
    }
    void teardown()
    {
        mock().clear();
    }
};

TEST(SensorSetupTestGroup, SensorDisabledByDefault)
{
    MockReceiver rx = MockReceiver();
    MockTransmitter tx = MockTransmitter();
    mock().expectOneCall("Disable").onObject(&tx);
    mock().expectOneCall("Disable").onObject(&rx);
    UltrasonicSensor sensor = UltrasonicSensor(rx, tx);
    mock().checkExpectations();
}

TEST(SensorSetupTestGroup, EnableSensorEnablesPeripherals)
{
    mock().disable();
    MockReceiver rx = MockReceiver();
    MockTransmitter tx = MockTransmitter();
    UltrasonicSensor sensor = UltrasonicSensor(rx, tx);
    mock().enable();
    mock().expectOneCall("Enable").onObject(&tx);
    mock().expectOneCall("Enable").onObject(&rx);
    sensor.Enable();
    mock().checkExpectations();
}

TEST_GROUP(SensorOperationTestGroup)
{
    UltrasonicSensor* sensor;
    MockReceiver* receiver;
    MockTransmitter* transmitter;
    void setup()
    {
        MockTime::SetTimeInMicroseconds(0U);
        transmitter = new MockTransmitter();
        receiver = new MockReceiver();
        mock().disable();
        sensor = new UltrasonicSensor(*receiver, *transmitter);
        mock().enable();
    }
    void teardown()
    {
        mock().clear();
        delete transmitter;
        delete receiver;
        delete sensor;
    }
};

TEST(SensorOperationTestGroup, DetectSurfaceOneMeterAway)
{
    //distance should be zero initialised...
    DOUBLES_EQUAL(0.0, sensor->GetDistanceCm(), 1.0);
    //say the sensor detects a surface 1m away. Sound traveling at 344m/s will
    //take 2907us to reach it -> 5814us round trip. Therefore, the we should
    //expect the receiver to get an echo after this...
    transmitter->elapsedTime = 5814U;
    receiver->GetEchoListener()->HandleEcho();
    DOUBLES_EQUAL(100.0, sensor->GetDistanceCm(), 1.0);
}

TEST(SensorOperationTestGroup, NoEchoReceivedShouldReturnMaxDistance)
{
    DOUBLES_EQUAL(0.0, sensor->GetDistanceCm(), 1.0);
    //maximum range of the sensor is 4m (8m round trip) so the time between
    //pings can never be more than this. This will give the maximum distance.
    transmitter->elapsedTime = 23256U;
    receiver->GetEchoListener()->HandleEcho();
    DOUBLES_EQUAL(400.0, sensor->GetDistanceCm(), 1.0);
}
