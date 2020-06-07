/*
 * Only tested on an Arduino Uno
 * Requires modified interrupt definition!
 */
#include "ArduinoTransponder.h"
#include "UltrasonicSensor.h"

#define ECHO_PIN    2  // must be 2 or 3 on an Uno
#define TRIGGER_PIN 10 // only works on this pin right now

ArduinoReceiver receiver = ArduinoReceiver(ECHO_PIN);
ArduinoTransmitter transmitter = ArduinoTransmitter(TRIGGER_PIN);
UltrasonicSensor sensor = UltrasonicSensor(receiver, transmitter);

void setup(void)
{
    Serial.begin(9600);
    Serial.println("Setting up...");
    sensor.Enable();
    Serial.println("Setup done.");
}

void loop(void)
{
    Serial.println(sensor.GetDistanceCm());
}
