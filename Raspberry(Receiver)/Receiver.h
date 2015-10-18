
#ifndef _Receiver_h
#define _Receiver_h

#include "RCSwitch.h"
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <string.h>

class Receiver {

	public:
		Receiver();
		char * receiveData();
		void setPin(int pin);
		void setOID(int oid);
		bool getIsCommand();
		bool getIsString();

		
	private:
		//Properties
		RCSwitch mySwitch;
		int PIN;
		bool received;
		int oldNumPacket;
		int OID;
		bool isString;
		bool isCommand;
		char * oldValue;
		char * theCmd;
		char * theStr;
		
		//Function
		void onDataReceived(int oid, int numPacket, char * rawData);
		char * concat(char * str1, char * str2);
		bool isBad(char * frame);
		char * dataFromBinary(char *s);
		int intFromBinary(char *s);
		char * getFromFrame(char * frame, int size, int offset);
		char * int2bin(int i, int size);
};
#endif