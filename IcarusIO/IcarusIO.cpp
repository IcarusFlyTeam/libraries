#include "IcarusIO.h"

		IcarusIO::IcarusIO(Stream* port){
	myDebugSerial=port;
}

void IcarusIO::printIncomingMsg(){
	for (int i=0; i<32;i++){
		if (msgAvailable&(1<<i)) {
			switch (i){
				case Tempo: myDebugSerial->println(Time);
							break;
    case Latitudine: 	myDebugSerial->print(Latitude);
						myDebugSerial->print(";");
							break;
    case Longitudine: 	myDebugSerial->print(Longitude);
						myDebugSerial->print(";");
						break;
	#ifdef IcarusCelle_h
    case Celle: 		for (int j=0;j<NUM_CELLE;j++){
							myDebugSerial->print(Load[j]);
							myDebugSerial->print("|");}
							myDebugSerial->print(";");
							break;
	#endif
	#ifdef IcarusServi_h
    case valoreServo: 	for (int j=0;j<NUM_SERVI;j++){
							myDebugSerial->print(valueServi[j]);
							myDebugSerial->print(";");}
							break;
	#endif
    case rawAcc_z: 		myDebugSerial->print(rawAcc);
						myDebugSerial->print(";");
							break;
    case Roll:  		myDebugSerial->print(angleRoll);
						myDebugSerial->print(";");
							break;
    case Pitch: 		myDebugSerial->print(anglePitch);
						myDebugSerial->print(";");
							break;
    case Yaw: 			myDebugSerial->print(angleYaw);
						myDebugSerial->print(";");
							break;
    case Altitudine: 	myDebugSerial->print(Altitude);
						myDebugSerial->print(";");
							break;
    case gpsSpeed: 		myDebugSerial->print(speedGps);
						myDebugSerial->print(";");
							break;
    case Pressione: 	myDebugSerial->print(Pressure);
						myDebugSerial->print(";");
							break;
    case Temperatura: 	myDebugSerial->print(Temperature);
						myDebugSerial->print(";");
							break;
    case Pito: 			myDebugSerial->print(airSpeed);
						myDebugSerial->print(";");
							break;
    default: break;
			}
			bitClear(msgAvailable,i);
		}
	}
}