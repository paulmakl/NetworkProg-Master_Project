#include <iostream>
#include <stdlib.h>
using namespace std;

class Packet {
	public:
		int makePacket();
		//Packet(short frm, bool resen, unsigned short sn, unsigned short dest, unsigned short sendr, char *dta, int CS);
		int buildPacket();
		int make_resend(); // turns the resend bool to true
	
		short frametype;
		bool resend;
		unsigned short sequence_number;
		unsigned short destination;
		unsigned short sender;
		char *data;
		int CRC;
		int bytes_to_send;
	private:
};