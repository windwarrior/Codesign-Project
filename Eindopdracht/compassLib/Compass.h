#ifndef _COMPASS_H_
#define _COMPASS_H_
#include "Arduino.h"
#include "Wire.h"

class Compass{
	public:
		Compass();
		int getHeading();
	private:
		Compass _compass;
		int HMC6352Address;
		int slaveAddress;
		byte headingData[2];
		int i, headingValue;
};

#endif