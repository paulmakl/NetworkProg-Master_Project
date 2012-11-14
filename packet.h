#include <iostream>
#include <stdlib.h>
#ifndef packet_H
#define packet_H
using namespace std;

class Packet {
	public:
		void initpacket(unsigned short dest, char* dta, int size, unsigned short madder);//(short frametype, bool resend, unsigned short sequence_number, unsigned short destination, unsigned short sender, char* data, int CRC, int bytes_to_send);
		//Packet(short frm, bool resen, unsigned short sn, unsigned short dest, unsigned short sendr, char *dta, int CS);
        void init_Packet(char *pac, int byts);
		int make_resend(); // turns the resend bool to true
		int buildByteArray(char *buffer); // builds a byte array of the packet attached. 
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
		char physical_data_array[2038];
        int frame_size; 
	private:
};

#endif
