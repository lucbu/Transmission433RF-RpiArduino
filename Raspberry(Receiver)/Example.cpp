#include "Receiver.h"
#include <stdlib.h>
#include <stdio.h>

Receiver myReceiver;

int main(int argc, char *argv[]) {
	myReceiver = Receiver();
	//You can change the pin if you want
	//myReceiver.setPin(2);
	//You can change the oid if you want
	//myReceiver.setOID(288);
	while(1){
		//This will fulfill 'example' with the received string
		char * example = myReceiver.receiveData();
		printf("Message: %s\n", example);
	}
	
	exit(0);
}