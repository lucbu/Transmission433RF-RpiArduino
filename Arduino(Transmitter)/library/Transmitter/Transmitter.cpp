
#include "Transmitter.h"
#include <stdlib.h>
#include <stdio.h>

// ##### The Frame (32bits) ##### //
#define sizeFrame 32
#define sizeOID 10
#define sizeNumPacket 5
#define sizeData 16
#define sizeDataByte 2
#define sizeChar 8
// The last bit is parity bit

 
Transmitter::Transmitter(){
	this->mySwitch = RCSwitch();
	this->setDelay(10);
	this->setPin(10);
	this->setOID(288);
	this->sending = false;
}

void Transmitter::setDelay(int delay){
	this->DELAY = delay;
}

void Transmitter::setPin(int pin){
	this->TRANSMITTER_PIN = pin;
	this->mySwitch.enableTransmit(pin);
}

void Transmitter::setOID(int oid){
	if(oid<1023){
		this->OID = oid;
	}
}

bool Transmitter::isSending(){
	return this->sending;
}

void Transmitter::sendCommand(char cmd){
	this->sending = true;
	char * toSend = concat(int2bin(1, sizeChar), char2bin(cmd));
	char * frame = createFrame(this->OID, 0, toSend);
	this->send(frame);
	free(frame);free(toSend);
	this->sending = false;
}

void Transmitter::sendString(char *str) {
	this->sending = true;
	uint8_t cnt = 0;
	char * beginString = this->createFrame(this->OID, 0, concat(int2bin(2, sizeChar), int2bin(0, sizeChar)));
	this->send(beginString);
	free(beginString);
	uint8_t i = 0;
	for (; i < strlen(str); i = i+2){
		char * data = (char *) malloc(sizeDataByte);
		char * data1 = int2bin(str[i], sizeChar);
		strcpy(data, data1);
		if(i+1 < strlen(str)){
			char * data2 = int2bin(str[i+1], sizeChar);
			data = this->concat(data, data2);
			free(data2);
		}else{
			data = this->concat(data, "00000000");
		}
		if(cnt < 31)
			cnt++;
		else
			cnt = 1;
		char * frame = this->createFrame(this->OID, cnt, data);
		this->send(frame);
		free(data1);free(data);free(frame);
	}
	char * endString = this->createFrame(this->OID, 0, this->concat(this->int2bin(3, sizeChar), this->int2bin(cnt, sizeChar)));
	this->send(endString);
	free(str);free(endString);free(&cnt);free(&i);
	this->sending = false;
}

void Transmitter::send(char *str){
  this->mySwitch.send(str);
  delay(this->DELAY);
}

char * Transmitter::createFrame(int oid, int numPacket, char * data){
  char * oidBin = this->int2bin(oid, sizeOID);
  char * numPacketBin = this->int2bin(numPacket, sizeNumPacket);
  char * frame = this->concat(oidBin, numPacketBin);
  free(oidBin);free(numPacketBin);
  frame = this->concat(frame, data);
  frame = this->setParityBit(frame);
  return frame;
}
 
char * Transmitter::setParityBit(char * frame){
  uint8_t numberOfOnes = 0;
  char * parityBit = "1";
  uint8_t i = 0;
  for (; i < strlen(frame); i++){
    if (frame[i] == '1')
        numberOfOnes++;
  }
  if (numberOfOnes % 2 == 0)
    parityBit = "0";

  frame = this->concat(frame, parityBit);
  //free(&numberOfOnes);free(parityBit);
  return frame;
}

char * Transmitter::char2bin(char c){
  return this->int2bin((int) c, sizeChar);
}

char * Transmitter::int2bin(int i, int size)
{
    char * str = (char *) malloc(size + 1);
    if(!str) return NULL;
    str[size] = 0;

    unsigned u = *(unsigned *)&i;
    for(; size--; u >>= 1)
      str[size] = u & 1 ? '1' : '0';

    return str;
}

char * Transmitter::concat(char * str1, char * str2){
  char * str3 = (char *) malloc(1 + strlen(str1)+ strlen(str2) );
  strcpy(str3, str1);
  strcat(str3, str2);
  return str3;
}

