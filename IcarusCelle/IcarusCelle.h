#ifndef IcarusCelle_h
#define IcarusCelle_h

#include "Arduino.h"


typedef enum{
	_10kg,
	_500kg022,
	_500kg128
} celle;

class IcarusCelle{
	protected:
	byte iPin = A0;			// Defaults to "Strain 1"
	// Calibration
	int iMin = 0;
	int iMax = 1023;
	int oMin = 0;
	int oMax = 65535;
	public:
	IcarusCelle(byte, celle);
	uint16_t measureForce();
};
#endif