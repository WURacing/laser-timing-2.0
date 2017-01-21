# laser-timing
DESCRIPTION:
Code for the start- and end-laser Arduino sensors

REQUIRED HARDWARE:
Laser Post x2
Start Sensor
End Sensor
Laptop (to collect lap data)
9-volt w/ Arduino plug (or other method to power End Sensor)

SETUP:
Place a Laser post and Sensor post on either side of the track to create a gate (make sure the laser hits the UV Sensor on the Sensor posts. To connect the two gates to each other, take the red wire from spool attached to the the End Sensor and insert it into *Digital Pin 6* on the Start Sensor, and insert the black wire into the *Ground* on the Start Sensor. To power the sensors, connect the laptop to the Start Sensor, and power the End Sensor with a 9-volt (or any way that works, honestly).

USE:
The big button on the Start Sensor arms the tripwire (a disruption in the laser will start the timer), and the little button resets the tripwires and starts a new timing run. Additionally, an array will be sent over the Serial stream containing all lap data time (in milliseconds).
