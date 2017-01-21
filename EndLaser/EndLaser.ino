#include <Wire.h>
#include "Adafruit_SI1145.h"

//UV Sensor:
Adafruit_SI1145 uv = Adafruit_SI1145();

int UVLight = 0;
int visLight = 0;
const int normalIR = 5000;
const int normalUV = 10;

//Pin Constants:
const int SignalOut = 6;
const int RedLED = 8;

void setup() {
  Serial.begin(9600);
  // Initialize pins
  pinMode(SignalOut,OUTPUT);
  pinMode(RedLED,OUTPUT);

// Laser Startup
  if (! uv.begin()) {
    digitalWrite(SignalOut,HIGH);
    digitalWrite(RedLED,LOW);
    while (1);
  }

  Serial.println("OK");
}

void loop() {
// If laser is tripped...
  if (uv.readUV() < normalUV) {
    digitalWrite(SignalOut,HIGH);
    digitalWrite(RedLED,HIGH);
  }
  else {
    digitalWrite(SignalOut,LOW);
    digitalWrite(RedLED,LOW);
  }
  Serial.println(uv.readUV());
  
}
