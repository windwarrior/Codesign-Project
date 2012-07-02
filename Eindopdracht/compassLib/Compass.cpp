#include "Arduino.h"
#include "Wire.h"
#include "Compass.h"

Compass::Compass(){
	HMC6352Address = 0x42;
	slaveAddress = HMC6352Address >> 1;
	Wire.begin();
}

double Compass::getHeading(){
	// Send a "A" command to the HMC6352
	// This requests the current heading data
	Wire.beginTransmission(slaveAddress);
	Wire.write("A");              // The "Get Data" command
	Wire.endTransmission();
	delay(10);                   // The HMC6352 needs at least a 70us (microsecond) delay
	// after this command.  Using 10ms just makes it safe
	// Read the 2 heading bytes, MSB first
	// The resulting 16bit word is the compass heading in 10th's of a degree
	// For example: a heading of 1345 would be 134.5 degrees
	Wire.requestFrom(slaveAddress, 2);        // Request the 2 byte heading (MSB comes first)
	i = 0;
	while(Wire.available() && i < 2)
	{ 
	headingData[i] = Wire.read();
	i++;
	}
	headingValue = headingData[0]*256 + headingData[1];  // Put the MSB and LSB together
	return (double (headingValue) / 10.0);
}