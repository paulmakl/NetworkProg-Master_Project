#include <iostream>
#include <stdlib.h>
#ifndef packet_H
#define packet_H
using namespace std;

class Packet {
	public:
	//Methods
		Packet(): frametype(0), resend(false), sequence_number(0), sender(0), CRC(0), bytes_to_send(0), frame_size(0) {};
        void initpacket(unsigned short dest, char* dta, int size);//(short frametype, bool resend, unsigned short sequence_number, unsigned short destination, unsigned short sender, char* data, int CRC, int bytes_to_send);
		//Packet(short frm, bool resen, unsigned short sn, unsigned short dest, unsigned short sendr, char *dta, int CS);
        void init_Packet(char *pac, int byts);
		int make_resend(); // turns the resend bool to true
		int buildByteArray(char *buffer); // builds a byte array of the packet attached. 
		int get_crc();// TEMPORARY
		void pointer_data_to_physical(char* data);
		

    //Fields
        static const short MAXDATASIZE = 2038;  //The max amount of bytes
                                                //that can be held in the 
                                                //data field of a packet
	    short frametype;
        bool resend;
		unsigned short sequence_number;
		unsigned short destination;
		unsigned short sender;
	//	char* data; REMOVED THIS SO THAT DEFAULT COPY CONSTRUCTOR WILL WORK
		int CRC;
		int bytes_to_send;
		char physical_data_array[MAXDATASIZE];
        int frame_size; 
	private:
};

#endif
