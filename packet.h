#include <iostream>
#include <stdlib.h>
#ifndef packet_H
#define packet_H
using namespace std;

class Packet {
	public:
		void makePacket(short frm, bool resen, unsigned short sn, unsigned short dest, unsigned short sendr, char* dta, int CS, int size);//(short frametype, bool resend, unsigned short sequence_number, unsigned short destination, unsigned short sender, char* data, int CRC, int bytes_to_send);
		//Packet(short frm, bool resen, unsigned short sn, unsigned short dest, unsigned short sendr, char *dta, int CS);
		int make_resend(); // turns the resend bool to true
		void rawr();
		int buildByteArray(unsigned char* packup);
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

short frametype;
bool resend;
unsigned short sequence_number;
unsigned short destination;
unsigned short sender;
char* data;
unsigned int CRC;
int bytes_to_send;
int packup_size;

void Packet::makePacket(short frm, bool resen, unsigned short sn, unsigned short dest, unsigned short sendr, char* dta, int CS, int size){
	frametype = frm;
	resend = resen;
	sequence_number = sn;
	destination = dest;
	sender = sendr;
	data = dta;
	CRC = CS;
	bytes_to_send = size;
	packup_size = size + 10;
}
void Packet::rawr(){
	cout << "k";
}
int Packet::buildByteArray(unsigned char* packup){
	//unsigned char Packet[packet_size];
	// Set the CRC
	int i = 0;
	unsigned int temp_int = 0;
	while(i < 4){
		temp_int = CRC << 24 - 8*i;
		temp_int = temp_int >> 24;
		packup[packup_size-5+i] = temp_int;
		cout << temp_int << "\n";
		i++;
	}

	// put all the data in the packup
	/*i = 0;
	while(i<2038){
		packup[i + 7] = data[i];
		i++;
	}*/
	// put in source address
	unsigned short temp_short = 0;
	temp_short = sender << 8;
	temp_short = temp_short >> 8;
	packup[4] = temp_short;
	cout << ":" << temp_short << "\n";
	temp_short = sender >> 8;
	packup[5] = temp_short;
	cout << ":" << temp_short << "\n";
	// put in dest address
	temp_short = destination << 8;
	temp_short = temp_short >> 8;
	packup[2] = temp_short;
	cout << ":" << temp_short << "\n";
	temp_short = destination >> 8;
	packup[3] = temp_short;
	cout << ":" << temp_short << "\n";
	// deal with control garbage
}
int Packet::make_resend(){
	return 1;
}

#endif
