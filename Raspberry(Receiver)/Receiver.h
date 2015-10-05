
#ifndef _Receiver_h
#define _Receiver_h

#include "RCSwitch.h"
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <limits.h>
#include <string.h>

class Receiver {

	public:
		Receiver();
		std::string receiveData();
		void setPin(int pin);
		void setOID(int oid);

		
	private:
		//Properties
		RCSwitch mySwitch;
		int PIN;
		bool received;
		int oldNumPacket;
		int OID;
		bool isString;
		char * oldValue;
		std::string theStr;
		
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