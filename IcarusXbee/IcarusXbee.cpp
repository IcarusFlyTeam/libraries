#include "String.h"
#include "IcarusXbee.h"

IcarusXbee::IcarusXbee(IcarusIO* xbeeIO, IcarusAddress address, Stream *port ){
	this->xbeeIO=xbeeIO;
	xbee.setSerial(*port);
	switch(address){
		case ippopotamus:
		case rhino:
		case crocodile: setReceiverAddress(eagle);	
						break;
		case eagle: setReceiverAddress(crocodile);
					break;
		default: break;
	}
	zbTx = ZBTxRequest(addr64, container, sizeof(container));
	txStatus = ZBTxStatusResponse();
	strcpy(cmdFire[Carica], cmdCarica);
	strcpy(cmdFire[scaricaSicura], cmdScaricaSicura);
	strcpy(cmdFire[Fuoco], cmdFuoco);
	strcpy(cmdFire[avviaCountdown], cmdCountdown);
	strcpy(cmdFire[stopEmergenza], cmdStop);
	strcpy(cmdFire[testConnessione], cmdTest);
	strcpy(cmdFire[Carica], cmdCarica);
	strcpy(cmdFire[Timeout], cmdTimeout);
	strcpy(cmdFire[MissingCountdown], cmdMissingCountdown);
}

void IcarusXbee::addToPayload(dataType tipo){
	if(tipo==Tempo||tipo==Latitudine||tipo==Longitudine
		||tipo==Roll||tipo==Pitch||tipo==Yaw||tipo==Altitudine
		||tipo==gpsSpeed||tipo==Pressione||tipo==Temperatura) 
		{if (indice+sizeof(uint32_t)+1>MAXIMUMPAYLOAD){
			xbeeIO->myDebugSerial->println("Payload overflow!");
			return;}}
	if(tipo==valoreServo){if (indice+sizeof(uint16_t)*NUM_SERVI+1>MAXIMUMPAYLOAD){
							xbeeIO->myDebugSerial->println("Payload overflow!");
						return;}}
	if(tipo==Celle){if (indice+sizeof(uint16_t)*NUM_CELLE+1>MAXIMUMPAYLOAD){
							xbeeIO->myDebugSerial->println("Payload overflow!");
						return;}}
	if(tipo==rawAcc_z){if (indice+sizeof(uint16_t)+1>MAXIMUMPAYLOAD){
							xbeeIO->myDebugSerial->println("Payload overflow!");
						return;}}
	if(tipo==Pito||tipo==Comando){if (indice+sizeof(uint16_t)+1>MAXIMUMPAYLOAD){
							xbeeIO->myDebugSerial->println("Payload overflow!");
						return;}}
	if (tipo==ComandoLancio){if (indice+FIRESIZE+1>MAXIMUMPAYLOAD){
							xbeeIO->myDebugSerial->println("Payload overflow!");
						return;}}
  switch(tipo){
    case Tempo: container[indice++]='T';
				for (int i=0;i<sizeof(uint32_t);i++)
					container[indice++]=*((uint8_t*)&xbeeIO->Time+i);
                break;
    case Latitudine: 	container[indice++]='N';
						for (int i=0;i<sizeof(uint32_t);i++)
							container[indice++]=*((uint8_t*)&xbeeIO->Latitude+i);
						break;
    case Longitudine: 	container[indice++]='E';
						for (int i=0;i<sizeof(uint32_t);i++)
							container[indice++]=*((uint8_t*)&xbeeIO->Longitude+i);
						break;
						
    case Celle: container[indice++]='L';
					for (int i=0;i<NUM_SERVI*sizeof(uint16_t);i++)
							container[indice++]=*((uint8_t*)&xbeeIO->Load+i);
                       break;
    case valoreServo: 	container[indice++]='D';
						for (int i=0;i<(NUM_SERVI*sizeof(uint16_t));i++)
							container[indice++]=*((uint8_t*)&xbeeIO->valueServi+i);
						break;
    case rawAcc_z: container[indice++]='A';
					for (int i=0;i<sizeof(uint16_t);i++)
                        container[indice++]=*((uint8_t*)&xbeeIO->rawAcc+i);
                    break;
    case Roll:  container[indice++]='R';
                for (int i=0;i<sizeof(uint32_t);i++)
                    container[indice++]=*((uint8_t*)&xbeeIO->angleRoll+i);
                break;
    case Pitch: container[indice++]='P';
                for (int i=0;i<sizeof(uint32_t);i++)
                    container[indice++]=*((uint8_t*)&xbeeIO->anglePitch+i);
                break;
    case Yaw: container[indice++]='Y';
                for (int i=0;i<sizeof(uint32_t);i++)
                    container[indice++]=*((uint8_t*)&xbeeIO->angleYaw+i);
                break;
    case Altitudine: 	container[indice++]='M';
						for (int i=0;i<sizeof(uint32_t);i++)
							container[indice++]=*((uint8_t*)&xbeeIO->Altitude+i);
						break;
    case gpsSpeed: container[indice++]='V';
                for (int i=0;i<sizeof(uint32_t);i++)
                    container[indice++]=*((uint8_t*)&xbeeIO->speedGps+i);
                break;
    case Pressione: container[indice++]='H';
                for (int i=0;i<sizeof(uint32_t);i++)
                    container[indice++]=*((uint8_t*)&xbeeIO->Pressure+i);
                break;
    case Temperatura: container[indice++]='K';
                for (int i=0;i<sizeof(uint32_t);i++)
                    container[indice++]=*((uint8_t*)&xbeeIO->Temperature+i);
                break;
    case Pito: 	container[indice++]='S';
                for (int i=0;i<sizeof(uint16_t);i++)
                    container[indice++]=*((uint8_t*)&xbeeIO->airSpeed+i);
                break;
    case Comando: 	container[indice++]='C';
					for (int i=0;i<sizeof(uint16_t);i++)
						container[indice++]=*((uint8_t*)&xbeeIO->Cmd+i);
					break;
	case ComandoLancio: container[indice++]='F';
					 for (int i = 0; i<FIRESIZE*sizeof(uint8_t);i++)
					 container[indice++]=*((uint8_t*)&xbeeIO->cmdFire+i);
					break;
    default: break;
    }
  return;
  }
  
  void IcarusXbee::Send(){
	  zbTx = ZBTxRequest(addr64, container, indice);
	  xbee.send(zbTx);
	  indice=0;
  }
  
void IcarusXbee::Read(){
    xbee.readPacket();  //non blocking
	 if (xbee.getResponse().isAvailable()) {
		//xbeeIO->myDebugSerial->print("Checking Packet");
      if (xbee.getResponse().getApiId() == ZB_RX_RESPONSE) {
        xbee.getResponse().getZBRxResponse(rx);
        //xbeeIO->myDebugSerial->print("data payload length is ");
        //xbeeIO->myDebugSerial->println(rx.getDataLength(),DEC);
        // So, for example, you could do something like this:
        handleXbeeRxMessage(rx.getData(), rx.getDataLength());
        //Serial.println();
      }
      else if (xbee.getResponse().isError()) {
      // some kind of error happened, I put the stars in so
      // it could easily be found
      xbeeIO->myDebugSerial->print("************************************* error code:");
      xbeeIO->myDebugSerial->println(xbee.getResponse().getErrorCode(),DEC);
    }
    }
}

void IcarusXbee::handleXbeeRxMessage(uint8_t *data, uint8_t length){
	  int indice=0;
  while(indice<length){
  switch (data[indice++]) {
	  case 'T': xbeeIO->Time&=0x0;
	            for (int i = 0; i<sizeof(uint32_t); i++)
				        xbeeIO->Time|=data[indice+i]<<(8*i);
				      indice+=sizeof(uint32_t);
				xbeeIO->msgAvailable|=1<<Tempo;	  
				break;
	  case 'N': xbeeIO->Latitude&=0x0;
	            for (int i = 0; i<sizeof(uint32_t); i++)
				     xbeeIO->Latitude|=data[indice+i]<<(8*i);
				indice+=sizeof(uint32_t);
				xbeeIO->msgAvailable|=1<<Latitudine;
				break;
	  case 'E': xbeeIO->Longitude&=0x0;
	            for (int i = 0; i<sizeof(uint32_t); i++)
				    xbeeIO->Longitude|=data[indice+i]<<(8*i);
				indice+=sizeof(uint32_t);
				xbeeIO->msgAvailable|=1<<Longitudine;
				break;
	  case 'L': for (int i = 0; i<NUM_CELLE*sizeof(uint16_t); i++)
				    *((uint8_t*)&xbeeIO->Load+i)=data[indice+i];
				indice+=(NUM_CELLE*sizeof(uint16_t));
				xbeeIO->msgAvailable|=1<<Celle;
				break;	
	  case 'D': for (int i = 0; i<NUM_SERVI*sizeof(uint16_t); i++)
				    *((uint8_t*)&xbeeIO->valueServi+i)=data[indice+i];
				indice+=(NUM_SERVI*sizeof(uint16_t));
				xbeeIO->msgAvailable|=1<<valoreServo;
				break;
	  case 'A': xbeeIO->rawAcc&=0x0;
	            for (int i = 0; i<sizeof(uint16_t); i++)
				    xbeeIO->rawAcc|=data[indice+i]<<(8*i);
				indice+=sizeof(uint16_t);
				xbeeIO->msgAvailable|=1<<rawAcc_z;
				break;
	case 'R':   for (int i = 0; i<sizeof(float); i++)
					*((uint8_t*)&xbeeIO->angleRoll+i)=data[indice+i];
				indice+=sizeof(float);
				xbeeIO->msgAvailable|=1<<Roll;
				break;
    case 'P':   for (int i = 0; i<sizeof(float); i++)
					*((uint8_t*)&xbeeIO->anglePitch+i)=data[indice+i];
				indice+=sizeof(float);
				xbeeIO->msgAvailable|=1<<Pitch;
				break;
    case 'Y':   for (int i = 0; i<sizeof(float); i++)
					*((uint8_t*)&xbeeIO->angleYaw+i)=data[indice+i];
				indice+=sizeof(float);
				xbeeIO->msgAvailable|=1<<Yaw;
				break;
    case 'M':   for (int i = 0; i<sizeof(float); i++)
					*((uint8_t*)&xbeeIO->Altitude+i)=data[indice+i];
				indice+=sizeof(float);
				xbeeIO->msgAvailable|=1<<Altitudine;
				break;
    case 'V': for (int i = 0; i<sizeof(float); i++)
                *((uint8_t*)&xbeeIO->speedGps+i)=data[indice+i];
              indice+=sizeof(float);
			  xbeeIO->msgAvailable|=1<<gpsSpeed;
              break;
    case 'H': for (int i = 0; i<sizeof(float); i++)
                *((uint8_t*)&xbeeIO->Pressure+i)=data[indice+i];
              indice+=sizeof(float);
              xbeeIO->msgAvailable|=1<<Pressione;
			  break;
    case 'K': for (int i = 0; i<sizeof(float); i++)
                *((uint8_t*)&xbeeIO->Temperature+i)=data[indice+i];
              indice+=sizeof(float);
			  xbeeIO->msgAvailable|=1<<Temperatura;
              break;
    case 'S': xbeeIO->airSpeed&=0x0;
              for (int i = 0; i<sizeof(uint16_t); i++)
                xbeeIO->airSpeed|=data[indice+i]<<(8*i);
              indice+=sizeof(uint16_t);
			  xbeeIO->msgAvailable|=1<<Pito;
              break;
    case 'C': xbeeIO->Cmd&=0x0;
              for (int i = 0; i<sizeof(uint16_t); i++)
                xbeeIO->Cmd|=data[indice+i]<<(8*i);
              indice+=sizeof(uint16_t);
			  xbeeIO->cmdAvailable=true;
              break;
	case 'F': for (int i = 0; i<FIRESIZE*sizeof(uint8_t); i++)
                *((uint8_t*)&xbeeIO->cmdFire+i)=data[indice+i];
              indice+=FIRESIZE*sizeof(uint16_t);
			  xbeeIO->cmdFireAvailable=true;
              break;
	
    default: break;
    }          
  }
}

void IcarusXbee::setReceiverAddress(IcarusAddress destinatario){
	switch(destinatario){
		case rhino:
		case crocodile:		addr64 = XBeeAddress64(CROCODILE_SH, CROCODILE_LH);
							break;
		case ippopotamus:	addr64 = XBeeAddress64(IPPOPOTAMUS_SH, IPPOPOTAMUS_LH);
							break;
		case eagle:			addr64 = XBeeAddress64(EAGLE_SH, EAGLE_LH);
							break;
		default: break;
	}
}

void IcarusXbee::SendCmdFire(typeCmdFire comando){
	xbeeIO->cmdFire[FIRESIZE]='\0';
	memcpy(xbeeIO->cmdFire, cmdFire[comando], FIRESIZE + 1);
	addToPayload(ComandoLancio);
	Send();
}

typeCmdFire IcarusXbee::readCmdFire(){
	xbeeIO->cmdFireAvailable=false;
	for (int i = 0; i < numCmdFire; i++) 
	{
		if (strcmp(xbeeIO->cmdFire, cmdFire[i]) == 0){
		//xbeeIO->myDebugSerial->println(xbeeIO->cmdFire);
		return  static_cast<typeCmdFire>(i);}
	}
	return  static_cast<typeCmdFire>(numCmdFire+1);
}

uint16_t IcarusXbee::getCmd(){
	xbeeIO->cmdAvailable=false;
	return xbeeIO->Cmd;
}

bool IcarusXbee::CheckAcknCmdFire(typeCmdFire cmd, unsigned long int timeout) {
	unsigned long int startWait = millis();
	while (millis()-startWait<=timeout) {
		Read();
		if (xbeeIO->cmdFireAvailable) {
			if (readCmdFire() == cmd)
				return true;
		}
	}
	return false;
}
