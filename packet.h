#include <iostream>
#include <stdlib.h>
#ifndef packet_H
#define packet_H
using namespace std;

class Packet {
	public:
		void initPacket(short frm, bool resen, unsigned short sn, unsigned short dest, unsigned short sendr, unsigned char* dta, int CS, int size);//(short frametype, bool resend, unsigned short sequence_number, unsigned short destination, unsigned short sender, char* data, int CRC, int bytes_to_send);
		//Packet(short frm, bool resen, unsigned short sn, unsigned short dest, unsigned short sendr, char *dta, int CS);
		int make_resend(); // turns the resend bool to true
		int buildByteArray();
		int get_crc();// TEMPORARY
		void pointer_data_to_physical();
		short frametype;
		bool resend;
		unsigned short sequence_number;
		unsigned short destination;
		unsigned short sender;
		unsigned char* data;
		int CRC;
		int bytes_to_send;
		unsigned char physical_data_array[2048];
		unsigned char physical_frame[2048];
        int frame_size; 
	private:
};

#endif
