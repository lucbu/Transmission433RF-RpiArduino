#include "Receiver.h"

const int lengthFrame = 32;
const int lengthOID = 10;
const int lengthNumPacket = 5;
const int lengthData = 16;
const int lengthChar = 8;
//Last Bit is for parity bit


Receiver::Receiver(){
	this->PIN = 2;
	this->received = false;
	this->oldNumPacket = 0;
	this->isString = false;
	this->isCommand = false;
	this->oldValue = (char *) malloc(lengthFrame);
	this->theStr = (char *) malloc(2);
	this->theCmd = (char *) malloc(lengthChar);;
    if(wiringPiSetup() == -1)
		printf("Error in initializing wiringpi\n");
    this->mySwitch = RCSwitch();
    this->setPin(Receiver::PIN);
	this->setOID(288);
}

void Receiver::setPin(int pin){
	this->PIN = pin;
    this->mySwitch.enableReceive(pin);
}

void Receiver::setOID(int oid){
    this->OID = oid;
}
 
char * Receiver::int2bin(int i, int size){
    char * str = (char *) malloc(size + 1);
    if(!str) return NULL;
    str[size] = 0;

    // type punning because signed shift is implementation-defined
    unsigned u = *(unsigned *)&i;
    for(; size--; u >>= 1)
      str[size] = u & 1 ? '1' : '0';

    return str;
} 

char * Receiver::getFromFrame(char * frame, int size, int offset){
	char * raw = (char *) malloc(size);
	for (int i=0;i<size;i++) {
		if (i < size) {
			raw[i] = frame[offset + i];
		}
	}
	return raw;
}

int Receiver::intFromBinary(char *s){
  return (int) strtol(s, NULL, 2);
}

char * Receiver::dataFromBinary(char *s){
	char * data1 = getFromFrame(s, lengthChar, 0);
	char * data2 = getFromFrame(s, lengthChar, lengthChar);
	char * data = (char *) malloc(2) ;
	data[0] = (char) intFromBinary(data1);
	data[1] = (char) intFromBinary(data2);
	return data;
}

 
bool Receiver::isBad(char * frame){
	int numberOfOnes = 0;
	char parityBit = frame[strlen(frame)-1];
	for (int i = 0; i < strlen(frame)-1; i++){
		if (frame[i] == '1')
		numberOfOnes++;
	}
	if((numberOfOnes % 2 == 0 && parityBit == '1') || (numberOfOnes % 2 == 1 && parityBit == '0'))
		return true;
	return false;
}

char * Receiver::concat(char * str1, char * str2){
  char * str3 = (char *) malloc(1 + strlen(str1)+ strlen(str2) );
  strcpy(str3, str1);
  strcat(str3, str2);
  return str3;
}

bool Receiver::getIsString(){
	return this->isString;
}

bool Receiver::getIsCommand(){
	return this->isCommand;
}

void Receiver::onDataReceived(int oid, int numPacket, char * rawData){
	if(oid == this->OID){
		if(numPacket == 0){
			// If numPAcket == 0 it's a request
			
			char * rawDataPart1 = getFromFrame(rawData, lengthChar, 0);
			char * rawDataPart2 = getFromFrame(rawData, lengthChar, lengthChar);
			
			int request = intFromBinary(rawDataPart1);
			if(request == 1){
				// Command
				this->received = true;
				this->isCommand = true;
				this->theStr = (char *) malloc(2);
				this->oldNumPacket = 0;
				this->isString = false;
				this->theCmd = dataFromBinary(rawDataPart2);
			}else if (request == 2) { 
				// Start message
				this->theStr = (char *) malloc(2);
				this->oldNumPacket = 0;
				this->isCommand = false;
				this->isString = true;
			}else if(request == 3){
				this->isCommand = false;
				int param = intFromBinary(rawDataPart2);
				if(param == this->oldNumPacket){
					// End message
					this->received = true;
				}else{
					//printf("Error \n");
				}
			}
		}else{
			if(this->isString){
				char * data = dataFromBinary(rawData);
				//printf("%d ", numPacket);
				if(numPacket == this->oldNumPacket+1 || (oldNumPacket == 31 && numPacket == 1) ){
					this->oldNumPacket++;
					this->theStr = this->concat(theStr, data);
				}else{
					//printf("Error in reception \n");
					this->theStr = (char *) malloc(2);
					this->oldNumPacket = 0;
					this->isString = false;
				}
			}
		}
	}
}

char * Receiver::receiveData(){

	// printf("################# \n");
	// printf("#   Receiving   # \n");
	// printf("################# \n");
	this->isString = false;
	this->isCommand = false;
	this->theStr =  (char *) malloc(2);
	this->oldNumPacket = 0;
	strcpy(this->oldValue, "00000000000000000000000000000000");
	do{
		if (this->mySwitch.available()) {
			int value = this->mySwitch.getReceivedValue();
			this->mySwitch.resetAvailable();
			char * frame = int2bin(value, lengthFrame);
			
			if (strcmp(frame, this->oldValue) != 0 && !isBad(frame)) {
				strcpy(this->oldValue, frame);
				//printf("%s \n", frame);
				
				int offset = 0;
				char * rawOID = getFromFrame(frame, lengthOID, offset);
				int oid = intFromBinary(rawOID);
				offset += lengthOID;
				char * rawNumPacket = getFromFrame(frame, lengthNumPacket, offset);
				offset += lengthNumPacket;
				char * rawDATA = getFromFrame(frame, lengthData, offset);
				int numPacket = intFromBinary(rawNumPacket);
				
				this->onDataReceived(oid, numPacket, rawDATA);
			}
		  }
	}while(!this->received);
	this->received = false;
	if(this->isCommand){
		return this->theCmd;
	}else if(this->isString){
		return this->theStr;
	}
}

