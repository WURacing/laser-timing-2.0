// This file is for testing to see if the UV sensors are working

#include <Wire.h>
#include "Adafruit_SI1145.h"

// IR Sensors
Adafruit_SI1145 uv = Adafruit_SI1145();

int visLight = 0;
int IRlight = 0;


void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
Serial.println("Adafruit SI1145 test");

Serial.println("hey");
}

void loop() {
  // put your main code here, to run repeatedly:

}
