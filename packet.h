#include <iostream>
#include <stdlib.h>
#ifndef packet_H
#define packet_H
using namespace std;

class Packet {
	public:
	//Methods
        void initpacket(unsigned short dest, char* dta, int size);
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
		int CRC;
		int bytes_to_send;
		char physical_data_array[MAXDATASIZE];
        int frame_size; 
	private:
};

#endif
