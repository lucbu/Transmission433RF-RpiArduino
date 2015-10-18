
#ifndef Transmitter_h
#define Transmitter_h

#include "RCSwitch.h"
#include <stdlib.h>
#include <stdio.h>

class Transmitter{

	public :
		void sendCommand(char cmd);
		void sendString(char *str);
		Transmitter();
		void setDelay(int delay);
		void setPin(int pin);
		void setOID(int oid);
		bool isSending();

	private:
		RCSwitch mySwitch;
		int DELAY;
		int TRANSMITTER_PIN;
		int OID;
		bool sending;
		void send(char *str);
		char * createFrame(int oid, int numPacket, char * data);
		char * setParityBit(char * frame);
		char * char2bin(char c);
		char * int2bin(int i, int size);
		char * concat(char * str1, char * str2);
};

#endif