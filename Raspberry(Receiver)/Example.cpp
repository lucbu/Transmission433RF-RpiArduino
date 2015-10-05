#include "Receiver.h"
#include <string.h>
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
		std::string example = myReceiver.receiveData();
		std::cout << "Message: " << example << std::endl;
	}
	
	exit(0);
}