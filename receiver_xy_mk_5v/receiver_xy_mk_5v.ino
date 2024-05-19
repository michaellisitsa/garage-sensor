// ask_receiver.pde
// -*- mode: C++ -*-
// Simple example of how to use RadioHead to receive messages
// with a simple ASK transmitter in a very simple way.
// Implements a simplex (one-way) receiver with an Rx-B1 module

#include <RH_ASK.h>
#include <SPI.h> // Not actualy used but needed to compile

unsigned int speed = 2000;
uint8_t rxPin = 10;
uint8_t txPin = 6;  // Any ways to avoid setting txPin?
uint8_t pttPin = 7; // Any ways to avoid setting pttPin?
bool pttInverse = false;
RH_ASK driver(speed, rxPin, txPin, pttPin, pttInverse);
uint8_t redLedPin = 8;
uint8_t yellowLedPin = 9;

void setup()
{
  Serial.begin(9600); // Debugging only
  if (!driver.init())
    Serial.println("init failed");
  ;
  pinMode(9, INPUT);  // Ensure high impedance at D9
  pinMode(10, INPUT); // Ensure high impedance at D10
  pinMode(redLedPin, OUTPUT);
  pinMode(yellowLedPin, OUTPUT);
}

void loop()
{
  uint8_t buf[RH_ASK_MAX_MESSAGE_LEN];
  uint8_t buflen = sizeof(buf);

  if (driver.recv(buf, &buflen)) // Non-blocking
  {
    int i;
    // Message with a good checksum received, dump it.
    driver.printBuffer("Got:", buf, buflen);
    // Cast the received bytes back to a float
    uint8_t message = *((uint8_t *)buf);
    Serial.println(message);
    if (message == 11)
    {
      digitalWrite(redLedPin, HIGH);
      digitalWrite(yellowLedPin, HIGH);
    }
    else if (message == 10)
    {
      digitalWrite(redLedPin, HIGH);
      digitalWrite(yellowLedPin, LOW);
    }
    else if (message == 1)
    {
      digitalWrite(redLedPin, LOW);
      digitalWrite(yellowLedPin, HIGH);
    }
    else
    {
      digitalWrite(redLedPin, LOW);
      digitalWrite(yellowLedPin, LOW);
    }
  }
}