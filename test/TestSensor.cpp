#include "MockTime.h"
#include "../SonarBarrier.h"
#include "../UltrasonicSensor.h"

#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"
using namespace std;

class MockTransmitter : public UltrasonicSensor::Transmitter
{
public:
    bool isEnabled(void) const {return enabled;}
    void enable(void)
    {
        mock().actualCall("Enable").onObject(this);
        enabled = true;
    }
    void disable(void) {
        mock().actualCall("Disable").onObject(this);
        enabled = false;
    }
private:
    bool enabled = false;
};

class MockReceiver : public UltrasonicSensor::Receiver
{
public:
    bool isEnabled(void) const {return enabled;}
    void enable(void) {
        mock().actualCall("Enable").onObject(this);
        enabled = true;
    }
    void disable(void)
    {
        mock().actualCall("Disable").onObject(this);
        enabled = false;
    }
    void handleEcho(void)
    {
        uint32_t elapsedTime = MockTime::GetTimeInMicroseconds() - initialTime;
        if (listener) listener->handleEcho(elapsedTime);
    }
    void handlePing(void)
    {
        initialTime = MockTime::GetTimeInMicroseconds();
    }
    void registerListener(UltrasonicSensor::EchoListener* toRegister) {listener = toRegister;}
    // make the listener public for the purposes of tests. This way we can
    // easily simulate the arrival of an echo or a timeout because one doesn't
    // arrive when it should do.
    UltrasonicSensor::EchoListener* GetEchoListener(void) {return listener;}
private:
    UltrasonicSensor::EchoListener* listener = NULL;
    uint32_t initialTime = 0;
    bool enabled = false;
};

class MockDistanceListener : public SonarBarrier::DistanceListener
{
public:
    double gotDistance = 0.0;
    void handleDistanceNotification(double distance)
    {
        gotDistance = distance;
    }
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
    UltrasonicSensor::Sensor sensor = UltrasonicSensor::Sensor(rx, tx);
    mock().checkExpectations();
}

TEST(SensorSetupTestGroup, EnableSensorEnablesPeripherals)
{
    mock().disable();
    MockReceiver rx = MockReceiver();
    MockTransmitter tx = MockTransmitter();
    UltrasonicSensor::Sensor sensor = UltrasonicSensor::Sensor(rx, tx);
    mock().enable();
    mock().expectOneCall("Enable").onObject(&tx);
    mock().expectOneCall("Enable").onObject(&rx);
    sensor.enable();
    mock().checkExpectations();
}

TEST_GROUP(SensorOperationTestGroup)
{
    UltrasonicSensor::Sensor* sensor;
    MockReceiver* receiver;
    MockTransmitter* transmitter;
    void setup()
    {
        MockTime::SetTimeInMicroseconds(0U);
        transmitter = new MockTransmitter();
        receiver = new MockReceiver();
        mock().disable();
        sensor = new UltrasonicSensor::Sensor(*receiver, *transmitter);
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
    DOUBLES_EQUAL(0.0, sensor->getDistanceCm(), 1.0);
    receiver->handlePing();
    //say the sensor detects a surface 1m away. Sound traveling at 344m/s will
    //take 2907us to reach it -> 5814us round trip. Therefore, the we should
    //expect the receiver to get an echo after this...
    MockTime::SetTimeInMicroseconds(5814U);
    receiver->handleEcho();
    DOUBLES_EQUAL(100.0, sensor->getDistanceCm(), 1.0);
}

TEST(SensorOperationTestGroup, NoEchoReceivedShouldReturnMaxDistance)
{
    DOUBLES_EQUAL(0.0, sensor->getDistanceCm(), 1.0);
    receiver->handlePing();
    //maximum range of the sensor is 4m (8m round trip) so the time between
    //pings can never be more than this. This will give the maximum distance.
    MockTime::SetTimeInMicroseconds(23256U);
    receiver->handleEcho();
    DOUBLES_EQUAL(400.0, sensor->getDistanceCm(), 1.0);
}

TEST(SensorOperationTestGroup, ShouldCallCallbackWithDistance)
{
    MockDistanceListener listener;
    sensor->registerListener(&listener);
    receiver->handlePing();
    MockTime::SetTimeInMicroseconds(23256U);
    receiver->handleEcho();
    DOUBLES_EQUAL(400.0, listener.gotDistance, 1.0);
}
