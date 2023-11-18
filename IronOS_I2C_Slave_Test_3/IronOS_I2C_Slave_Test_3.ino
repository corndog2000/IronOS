// Wire Slave Sender
// by Nicholas Zambetti <http://www.zambetti.com>

// Demonstrates use of the Wire library
// Sends data as an I2C/TWI slave device
// Refer to the "Wire Master Reader" example for use with this

// Created 29 March 2006

// This example code is in the public domain.


#include <Wire.h>

#define TwoBytes 1

uint8_t toSend[2] = {};

void setup() {
  if (TwoBytes) {
    toSend[0] = 65;
    toSend[1] = 66;
  }
  else {
    toSend[0] = 65;
  }

  Wire.begin(0x55);                // join I2C bus with address #8
  Wire.onRequest(requestEvent); // register event
  Serial.begin(115200);

  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
}

void loop() {
  //Serial.println("...");
  delay(100);
}

// function that executes whenever data is requested by master
// this function is registered as an event, see setup()
void requestEvent() {
  if (TwoBytes) {
    Wire.write(toSend, 2);
  }
  else {
    Wire.write(toSend, 1);
  }
  Serial.println("!!!!!!!!!! Responded !!!!!!!!!!");
  digitalWrite(13, HIGH);
  delay(25);
  digitalWrite(13, LOW);
  // as expected by master
}
