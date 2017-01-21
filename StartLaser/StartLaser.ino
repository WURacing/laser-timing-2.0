#include <Wire.h>
#include "Adafruit_SI1145.h"

//UV Sensor:
Adafruit_SI1145 uv = Adafruit_SI1145();

int UVLight = 0;
int visLight = 0;
//  THESE ARE THE THRESHOLD VALUES; IF NEEDED, CHANGE TO SUIT WEATHER
const int normalIR = 5000;
const int normalUV = 18;

//Pin Constants:
const int ResetSignal = 3;
const int StartRaceSignal = 5;
const int EndRaceSignal = 6;
const int RedLED = 11;
const int GreenLED = 12;
  
//Testing/Timing:
unsigned long startTime = 0;
unsigned long endTime = 0;
long Times[100];
float timeElapsed = 0;
int runcount = 1;

//Delta Timing:
unsigned long currentTime = 0;
unsigned long previousLEDTime = 0;
const unsigned long deltaTime = 1000;
int RedLEDState = LOW;

//Debounce Buttons/Timing:s
int lastStartButtonState = LOW;
int resetButtonState = 1;
int lastResetButtonState = LOW;
unsigned long previousStartTime = 0;
unsigned long previousResetTime = 0;
const unsigned long debounceTime = 50;

//Signaling:

enum State {
  StartReady,
  RaceStart,
  RaceIsOn,
  RaceComplete,
  Reset,
};

State raceState = StartReady;

void setup() {
  Serial.begin(9600);
  
// Laser Startup
  Serial.println("Adafruit SI1145 test");
  if (! uv.begin()) {
    Serial.println("Didn't find Si1145");
    while (1);
  }

  Serial.println("OK!");
  
// Initialize pins
  pinMode(ResetSignal,INPUT_PULLUP);
  pinMode(StartRaceSignal,INPUT_PULLUP);
  pinMode(EndRaceSignal,INPUT_PULLUP);
  pinMode(RedLED,OUTPUT);
  pinMode(GreenLED,OUTPUT);

// All set up!
  digitalWrite(GreenLED,HIGH);
  Serial.println("Ready to time");

// Print first test number
  Serial.println("------");
  Serial.print("Run ");
  Serial.println(runcount);
  Serial.println();
}

void loop() {
// Time Tracking
  currentTime = millis();

// Main Functions
  startLaser(raceState);
  
// For Debugging Threshold Values
//  Serial.println(uv.readUV());
}

void startLaser(State s) {
  switch(s) {

//  before the laser is armed...
    case StartReady:
//    ...check the start button
      StartButtonCheck();
      break;

//  after the laser is armed...
    case RaceStart:
//    ..if the laser is tripped...
      if (uv.readUV() < normalUV) {
//      ...send "start tripped" message
        Serial.println("Start Laser was tripped!");
//      ...record the Start Time
        startTime = currentTime;
//      ...turn on red 
        digitalWrite(RedLED,HIGH);
        RedLEDState = HIGH;
//      ...start LED delta-timing
        previousLEDTime = currentTime;
//      ...change state
        raceState = RaceIsOn;
//      ...clear SerialIn
        while(Serial.available() > 0) {
          Serial.read();
        }
      }

//    ...check the reset button
      ResetButtonCheck();
      break;

//  while the race is happening...
    case RaceIsOn:
//    ...flash LED
      if (currentTime - previousLEDTime > deltaTime) {
        RedLEDState = !RedLEDState;
        digitalWrite(RedLED,RedLEDState);
        previousLEDTime += deltaTime;
      }
    
//    ...when the End Signal is recieved...
      if (digitalRead(EndRaceSignal) == HIGH) {
//      ...send "end tripped" message
        Serial.println("End Laser was tripped!");
//      ...record the End Time
        endTime = currentTime;
//      ...calculate Time Elapsed
        Times[(runcount - 1) % 100] = endTime - startTime;
        timeElapsed = (float) Times[runcount - 1] / 1000.0;
//      ...print the time elapsed
        Serial.println();
        Serial.print("Time elapsed: ");
        Serial.print(timeElapsed);
        Serial.println(" seconds");
        for (int i = 0; i < 100; i++) {
          Serial.print(Times[i]);
          Serial.print(",");
        }
        
        Serial.println();
//      ...turn LED off
        digitalWrite(RedLED,LOW);
//      ...change state
        raceState = RaceComplete;
      }

//    ...check the reset button
      ResetButtonCheck();
      break;

//  when the race is complete...
    case RaceComplete:
//    ...wait until the reset button is pushed
      ResetButtonCheck();
      break;

//  if the reset button is pressed...
    case Reset:
//    ...reset the timer longs and the reset boolean:
      previousLEDTime = currentTime;
//    ...reset the LEDs
      digitalWrite(RedLED,LOW);
      digitalWrite(GreenLED,HIGH);
//    ...prepare to restart the race
      raceState = StartReady;
      break;
  }

}

void StartButtonCheck() {
// Take a signal reading
  int startReading = digitalRead(StartRaceSignal);
  
// If the start signal is recieved...
  if (startReading != lastStartButtonState) {
//  ...start debounce check
    previousStartTime = currentTime;
  }

// If the start debounce check passes...
  if (currentTime - previousStartTime > debounceTime && !startReading) {
//  ...notify that the laser is armed
    Serial.println("Laser is armed!");
//  ...start the race
    raceState = RaceStart;
  }

// Record the current reading for the next check
  lastStartButtonState = startReading;
}

void ResetButtonCheck() {
// Take a signal reading
  int resetReading = digitalRead(ResetSignal);

//  If the reset button changes...
  if (resetReading != lastResetButtonState) {
//    ...start debounce check
    previousResetTime = currentTime;
  }

// If the reset debounce check passes...
  if (currentTime - previousResetTime > debounceTime && !resetReading) {
//  ...notify that reset procedure is initiated
    Serial.println("Resetting...");
//  ...if the button is pushed down...
//      ...initiate Reset procedure
    raceState = Reset;
//  ...change test index
    runcount += 1;
//  ...print test number
    Serial.println("------");
    Serial.print("Run ");
    Serial.println(runcount);
    Serial.println();
  }

// Record the current reading for the next check
  lastResetButtonState = resetReading;
}

