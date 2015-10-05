#################################################################
#																#
#			Transmitter Receiver Protocol RF 433MHz				#
#																#
#################################################################

Your raspberry will need the library wiringpi 	(https://projects.drogon.net/raspberry-pi/wiringpi/)

This projet is using RC-Switch
- (https://github.com/sui77/rc-switch)
- (https://github.com/r10r/rcswitch-pi)

_Set the content of the 'Arduino(Transmitter)/library' directory in the Arduino's library directory._

#Introduction :
I made this to send a text message from an Arduino Uno to a Raspberry Pi, using only one transmitter and one receiver.
It's made to be without mistake so if there is an error during the transmission, the receiver will wait for the transmitter to start again.

The transmitter is set on the Arduino Uno like below :
- GND is on a GND pin
- VCC is on a 5V pin
- Data is on the pin 10

The receiver is set on the Raspberry Pi like below :
- GND is on a GND pin
- VCC is on a 5V pin
- One of the data is on GPIO 2
	
By default the OID is set on 288. (Maximum is 1023)

The max length of the string is unlimited but the reception is sometime bad so it won't work.

#Documentation :
##Frame (32 bits):
- OID: 10 bits
- N° Packet: 5 bits
- DATA: 16 bits
- Parity Bit: 1bit

##How to use
Find uses in the examples files

##Transmitter (Arduino) :
Sending a string :
- First frame 	: [ OID, 0, 0x0002 ]
- Then 			: [ OID, i, Data[i] + Data[i+1] ] 
- Last frame	: [ OID, 0, i + 0x03 ]

*i from 1 to 31, data is char * if data[i+1] doesn't exist, it'll be set to 0x00*

##Receiver (Raspberry) :
Get the message only when it's completely received.
