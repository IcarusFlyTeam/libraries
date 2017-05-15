#ifndef IcarusIO_h
#define IcarusIO_h
#include "IcarusHWCfg.h"
#include "IcarusServi.h"
#include "IcarusCelle.h"
#include <Arduino.h>
#define FIRESIZE 16



enum dataType {
  Tempo,
  Latitudine,
  Longitudine,
  Celle,
  valoreServo,
  rawAcc_z,
  Roll,
  Pitch,
  Yaw,
  Altitudine,
  gpsSpeed,
  Pressione,
  Temperatura,
  Pito,
  Comando,
  ComandoLancio
};

class IcarusIO{
	protected:

	public:
	IcarusIO(Stream*);
	Stream* myDebugSerial;
	uint16_t valueServi[NUM_SERVI];
	uint16_t Load[NUM_CELLE];
	uint32_t Time, Latitude, Longitude;
	uint16_t rawAcc,airSpeed,Cmd;
	char cmdFire[FIRESIZE];
	float angleRoll,anglePitch,angleYaw,Altitude,speedGps,Pressure,Temperature;
	bool cmdAvailable;
	bool cmdFireAvailable;
	uint32_t msgAvailable;

	void printIncomingMsg();
};
#endif
