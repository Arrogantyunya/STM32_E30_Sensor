#include "MAX44009.h"
#include "SoftwareI2C.h"
//#include <math.h>  //use for pow() function

MAX44009 light;

MAX44009::MAX44009() {}

int MAX44009::begin()
{
	MAX44009_Sensor.beginTransmission(MAX_ADDR);
	MAX44009_Sensor.write(0x02);
	MAX44009_Sensor.write(0x00); // changed from 0x40
	
	return MAX44009_Sensor.endTransmission();
}


float MAX44009::get_lux(void)
{
	unsigned int data[2];
	
	MAX44009_Sensor.beginTransmission(MAX_ADDR);
	MAX44009_Sensor.write(0x03); //request high-byte register data
	MAX44009_Sensor.endTransmission();

	// Request 1 byte of data
	MAX44009_Sensor.requestFrom(MAX_ADDR, 1);

	// Read first byte of data
	if (MAX44009_Sensor.available() == 1)
	{
		data[0] = MAX44009_Sensor.read();
	}

    MAX44009_Sensor.beginTransmission(MAX_ADDR);
	MAX44009_Sensor.write(0x04); //request low-byte register data
	MAX44009_Sensor.endTransmission();

	// Request 1 byte of data
	MAX44009_Sensor.requestFrom(MAX_ADDR, 1);

	// Read second byte of data
	if (MAX44009_Sensor.available() == 1)
	{
		data[1] = MAX44009_Sensor.read();
	}
 
	// Convert the data to lux
	int exponent = (data[0] & 0xF0) >> 4;
	int mantissa = ((data[0] & 0x0F) << 4) | (data[1] & 0x0F);
	
	//float luminance = pow(2, exponent) * mantissa * 0.045;
	float luminance = (float)(((0x00000001 << exponent) * (float)mantissa) * 0.045);
	
	Serial.println(String("luminance = ") + luminance);
	return luminance; 
}
