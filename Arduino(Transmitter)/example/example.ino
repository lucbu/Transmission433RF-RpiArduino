#include <Transmitter.h>

Transmitter myTransmitter;

// The char to transmit
char * strToSend = "Hello";

void setup() {
  Serial.begin(9600);
  myTransmitter = Transmitter();
  myTransmitter.setDelay(0);
  /*
  You can modify these properties
  myTransmitter.setPin(10);
  myTransmitter.setOID(288);
  */

}

void loop() {
  //Send a string just like that:
  myTransmitter.sendString(strToSend);
  Serial.println("les");
}
