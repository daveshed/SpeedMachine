/*
 * Only tested on an Arduino Uno
 */
#include <DebugLog.h>

#include "ArduinoLed.h"
#include "ArduinoTransponder.h"
#include "LedMatrixDisplay.h"
#include "SonarBarrier.h"
#include "SpeedMachine.h"
#include "UltrasonicSensor.h"

#define ECHO_PINA    2U // must be 2 or 3 on an Uno
#define ECHO_PINB    3U // must be 2 or 3 on an Uno
#define TRIGGER_PIN 10U // only works on this pin right now

const uint8_t LEDMATRIX_CS_PIN = 7;
const int LEDMATRIX_SEGMENTS = 4;
const int LEDMATRIX_WIDTH = LEDMATRIX_SEGMENTS * 8;

// SENSORS...
ArduinoReceiver receiverA = ArduinoReceiver(ECHO_PINA);
ArduinoReceiver receiverB = ArduinoReceiver(ECHO_PINB);
ArduinoTransmitter transmitter = ArduinoTransmitter(TRIGGER_PIN);
UltrasonicSensor::Sensor sensorA = UltrasonicSensor::Sensor(receiverA, transmitter);
UltrasonicSensor::Sensor sensorB = UltrasonicSensor::Sensor(receiverB, transmitter);

// INDICATORS...
ArduinoLedIndicator indicatorA = ArduinoLedIndicator(4U);
ArduinoLedIndicator indicatorB = ArduinoLedIndicator(5U);
SonarBarrier::Barrier barrierA = SonarBarrier::Barrier(sensorA, indicatorA);
SonarBarrier::Barrier barrierB = SonarBarrier::Barrier(sensorB, indicatorB);
LedMatrixDisplay display(LEDMATRIX_SEGMENTS, LEDMATRIX_CS_PIN);

// CONTROLLER...
SpeedMachine::Controller controller(&barrierA, &barrierB, &display);

void HandleInterruptA(void)
{
    bool risingEdge = digitalRead(ECHO_PINA);
    if (risingEdge) receiverA.handlePing();
    else receiverA.handleEcho();
}

void HandleInterruptB(void)
{
    bool risingEdge = digitalRead(ECHO_PINB);
    if (risingEdge) receiverB.handlePing();
    else receiverB.handleEcho();
}

void setup(void)
{
    Serial.begin(9600);
    LOG_VERBOSE("Setting up...");
    pinMode(ECHO_PINA, INPUT);
    pinMode(ECHO_PINB, INPUT);
    display.setup();
    attachInterrupt(
      digitalPinToInterrupt(ECHO_PINA),
      HandleInterruptA,
      CHANGE);
    attachInterrupt(
      digitalPinToInterrupt(ECHO_PINB),
      HandleInterruptB,
      CHANGE);
    controller.setBarrierDistanceM(1.16);
    controller.start();
    LOG_VERBOSE("Setup done.");
}

void loop(void)
{
    // PRINTLN(sensor.getDistanceCm());
}
