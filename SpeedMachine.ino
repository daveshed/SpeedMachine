#include "ArduinoChannel.h"
#include "UltrasonicSensor.h"

// ---------------------------------------------------------------- //
// Arduino Ultrasoninc Sensor HC-SR04
// Using HC-SR04 Module
// ---------------------------------------------------------------- //

#define ECHO_PIN    2 
#define TRIGGER_PIN 3

ArduinoInputChannel receive = ArduinoInputChannel(ECHO_PIN);
ArduinoOutputChannel transmit = ArduinoOutputChannel(TRIGGER_PIN);
UltrasonicSensor sensor = UltrasonicSensor(receive, transmit);

void setup() {
  //pinMode(trigPin, OUTPUT); // Sets the trigPin as an OUTPUT
  //pinMode(echoPin, INPUT); // Sets the echoPin as an INPUT
  Serial.begin(9600); // // Serial Communication is starting with 9600 of baudrate speed
  Serial.println("Ultrasonic Sensor HC-SR04 Test"); // print some text in Serial Monitor
  Serial.println("with Arduino UNO R3");
}
void loop() {
  sensor.Refresh();
  Serial.println(sensor.ReadDistanceCm());
//  transmit.Activate();
//  Serial.println(transmit.IsActive());
//  delay(10);
//  transmit.Deactivate();
//  Serial.println(micros());
//  delay(10);
}
