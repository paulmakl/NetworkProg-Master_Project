#include <iostream>
#include <stdlib.h>
#ifndef packet_H
#define packet_H
using namespace std;

class Packet {
	public:
		void initPacket(short frm, bool resen, unsigned short sn, unsigned short dest, unsigned short sendr, char* dta, int CS, int size);//(short frametype, bool resend, unsigned short sequence_number, unsigned short destination, unsigned short sender, char* data, int CRC, int bytes_to_send);
		//Packet(short frm, bool resen, unsigned short sn, unsigned short dest, unsigned short sendr, char *dta, int CS);
        void initPacket(char *pac, int byts);
		int make_resend(); // turns the resend bool to true
		int buildByteArray(); // builds a byte array of the packet attached. 
		int get_crc();// TEMPORARY
		void pointer_data_to_physical();
		short frametype;
		bool resend;
		unsigned short sequence_number;
		unsigned short destination;
		unsigned short sender;
		char* data;
		int CRC;
		int bytes_to_send;
		char physical_data_array[2048];
		char physical_frame[2048];
        int frame_size; 
	private:
};

#endif
