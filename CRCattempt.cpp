/**
* This is an attempt at a 32-bit CRC with remainder of 4-Bytes for variable length
* messages.
*/

public main(int argc, char* argv[]) {

}

public int computeCRC(char* message, int len) {
	char temp[len+32];	//Stores bit pattern for message & is padded for CRC
	unsigned int poly = 0x04c11db7;	//bit stream for 802.11 32-bit polynomial
	unsigned int i = 0;

	temp = message; 	//Get bit pattern
	while (i < len - 32) {	//while we are not at the end of the message
		if (temp[i] & 1) { 	//if the char we are at is a 1

		}
	}



}