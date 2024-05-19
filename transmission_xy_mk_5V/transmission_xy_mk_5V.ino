// ask_transmitter.pde
// -*- mode: C++ -*-
// Simple example of how to use RadioHead to transmit messages
// with a simple ASK transmitter in a very simple way.
// Implements a simplex (one-way) transmitter with an TX-C1 module
#include <RH_ASK.h>
#include <SPI.h> // Not actually used but needed to compile
// #include <inttypes.h>
#include "LowPower.h"

uint16_t speed = 2000;
// Remapping rx and tx pins so you can use Pins 0 and 1 for sending
// via the computer
// https://arduino.stackexchange.com/questions/66708/why-map-arduino-pins-2-and-3-to-rx-and-tx-instead-of-using-the-build-in-rx-and-t
// Library docs.
// https://www.airspayce.com/mikem/arduino/RadioHead/classRH__ASK.html
// Old livewire
// https://quadmeup.com/fs1000a-and-xy-mk-5v-arduino-and-virtualwire/
// Where this code came from:
// https://forum.arduino.cc/t/fs1000a-rf-communication-using-rh_ask/282303

uint8_t lightGarageLilyPin = A5;
uint8_t rxPin = 6; // Any ways to avoid setting rxPin?
uint8_t txPin = 10;
uint8_t pttPin = 7; // Any ways to avoid setting pttPin? Transmitter enable pin.
boolean pttInverse = false;
RH_ASK driver(speed, rxPin, txPin, pttPin, pttInverse);

// Ultrasonic sensor
const int trigPin = 2;
const int echoPin = 3;
float duration, distance;
int lightValue = 0;

void setup()
{
  Serial.begin(9600); // Debugging only
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(lightGarageLilyPin, INPUT);
  if (!driver.init())
    Serial.println("init failed");
}

void loop()
{
  LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);
  // Ultrasonic sensor
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);

  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  float currentDistance = (duration * .0343) / 2;

  // Photoresistor
  lightValue = analogRead(lightGarageLilyPin);

  // Prepare status message
  int message = 0;
  if (currentDistance < 20.0 && lightValue < 700)
  {
    message = 11;
  }
  else if (currentDistance < 20.0)
  {
    message = 10;
  }
  else if (lightValue < 700)
  {
    message = 1;
  }
  else
  {
    message = 0;
  }

  // // Alternative sending as string
  // const char *msg = "hello";
  // driver.send((uint8_t *)msg, strlen(msg));

  uint8_t *bytes = (uint8_t *)&message;
  driver.send(bytes, sizeof(message));

  driver.waitPacketSent();
  delay(200);
}