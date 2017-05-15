#include <IcarusCelle.h>
#include "Arduino.h"

IcarusCelle::IcarusCelle(byte AnalogPin, celle cella){
	/* Calibrates the Wheatstone bridge linear interpolation.
	inputMin:	Minimum expected value of raw ADC input
	inputMax:	Maximum expected value of raw ADC output
	outputMin:	First (lower) calibration point with a known force, usually 0.
	outputMax:	Second (higher) calibration point with a known force, usually near the maximum force measurable by the load cell used.
	*/
	iMin = 672;
	switch(cella){
	case _10kg:	iMin =
				iMax = 346;
				oMin = 0;
				oMax = 1000;
				break;
	case _500kg022:	iMin = 672;
					iMax = 220;
					oMin = 0;
					oMax = 1000;
					break;
	case _500kg128:	iMin = 703;
					iMax = 250;
					oMin = 0;
					oMax = 1000;
					break;
	default: break;
	}
	iPin=AnalogPin;
}

uint16_t IcarusCelle::measureForce()
{
	// Obtain ADC raw measurement
	uint16_t lastForceADC = analogRead(iPin);
	
	// 
	//int16_t lastForce = map(lastForceADC, iMin, iMax, oMin, oMax) + oMin;
	//Serial.println(lastForce);
	// Return value
	return (lastForceADC);
}


