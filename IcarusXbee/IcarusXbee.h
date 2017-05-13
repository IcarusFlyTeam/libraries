#ifndef IcarusXbee_h
#define IcarusXbee_h

#include "Arduino.h"

#ifndef __SAM3X8E__
#include <SoftwareSerial.h>
#endif

#include <IcarusIO.h>
#include <XBee.h>

#define MAXIMUMPAYLOAD 54

#define	CROCODILE_SH 0x0013a200
#define CROCODILE_LH 0x40e5eaf4
#define	RHINO_SH 0x0013a200
#define RHINO_LH 0x40e5eaf4
#define	IPPOPOTAMUS_SH 0x0013a200
#define IPPOPOTAMUS_LH 0x40f5289d
#define	EAGLE_SH 0x0013a200
#define EAGLE_LH 0x40f5289d


typedef enum {
	crocodile,
	ippopotamus,
	eagle,
	rhino
} IcarusAddress;

typedef enum {
	Carica,
	scaricaSicura,
	Fuoco,
	avviaCountdown,
	stopEmergenza,
	testConnessione,
	Timeout,
	MissingCountdown,
	numCmdFire
} typeCmdFire;

class IcarusXbee
{
	public:
	IcarusXbee(IcarusIO*, IcarusAddress, Stream*);
	
	XBee xbee = XBee();
	void addToPayload(dataType);
	
	void setReceiverAddress(IcarusAddress);
	void handleXbeeRxMessage(uint8_t*, uint8_t);
	void Send();
	void Read();
	void SendCmdFire(typeCmdFire);
	typeCmdFire readCmdFire();
	uint16_t getCmd();
	
	protected:
	int indice=0;
	byte container[54];
	IcarusIO* xbeeIO;
	XBeeAddress64 addr64;
	ZBTxRequest zbTx;
	ZBTxStatusResponse txStatus;
	
	XBeeResponse response = XBeeResponse();
	// create reusable response objects for responses we expect to handle
	ZBRxResponse rx = ZBRxResponse();
	char cmdFire[numCmdFire][FIRESIZE+1];
	const char cmdCarica[FIRESIZE + 1] = { 0x4c,0x4f,0x41,0x44,0x43,0x4f,0x4e,0x44,0x45,0x4e,0x53,0x41,0x54,0x4f,0x52,0x49,'\0' };
	const char cmdScaricaSicura[FIRESIZE + 1] = { 0x44,0x41,0x56,0x53,0x43,0x41,0x52,0x49,0x43,0x41,0x53,0x49,0x43,0x55,0x52,0x41,'\0' };
	const char cmdCountdown[FIRESIZE + 1] = { 0x43,0x4f,0x4e,0x54,0x4f,0x52,0x4f,0x56,0x45,0x53,0x43,0x49,0x41,0x44,0x41,0x56,'\0' };
	const char cmdFuoco[FIRESIZE + 1] = { 0x44,0x41,0x56,0x45,0x44,0x49,0x43,0x45,0x46,0x55,0x4f,0x43,0x4f,0x4f,0x4f,0x4f,'\0' };
	const char cmdStop[FIRESIZE + 1] = { 0x44,0x41,0x56,0x45,0x44,0x49,0x43,0x45,0x53,0x54,0x4f,0x50,0x4f,0x52,0x41,0x21,'\0' };
	const char cmdTest[FIRESIZE + 1] = { 0x44,0x41,0x56,0x45,0x46,0x49,0x47,0x4f,0x46,0x41,0x55,0x4e,0x54,0x45,0x53,0x54,'\0' };
	const char cmdTimeout[FIRESIZE + 1] = { 0x54,0x45,0x53,0x54,0x54,0x45,0x53,0x54,0x54,0x45,0x53,0x54,0x54,0x45,0x53,0x54,'\0' };
	const char cmdMissingCountdown[FIRESIZE + 1] = { 0x44,0x61,0x76,0x65,0x73,0x69,0x61,0x6e,0x6e,0x6f,0x69,0x61,0x76,0x65,0x72,0x79,'\0' };
	};

#endif