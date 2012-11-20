#include <iostream>
#include <stdlib.h>
#pragma once
#ifndef packet_H
#define packet_H
using namespace std;

class Packet {
	public:
	// Initializer list
	Packet(): frametype(0), resend(false), sequence_number(0), sender(0), CRC(0), 
	resTransAttempts(0), bytes_to_send(0), frame_size(0) {}
	// Constructor for sender
	Packet(short dest, char* dta, int size);
	// Constructor for Receiver
    Packet(char *pac, int byts);
    // Creates a packet for an acknowledgement
    Packet(short destaddr, short seqnum);
    // Creates a resend packet
	int make_resend();
	// builds a packet into a byte array
	int buildByteArray(char *buffer);
	// takes the data out of a physical array and coppies it into the packet class
	void pointer_data_to_physical(char* data);

	// Fields
    static const short MAXDATASIZE = 2038;  //The max amount of bytes
                                            //that can be held in the 
                                            //data field of a packet
    // The type of frame being sent (between 1 and 4)
    short frametype;
    // checked off if this packet is a resend
    bool resend;
    // the sequence number
	short sequence_number;
	// destination addres
	short destination;
	// the address of the sender
	short sender;
	int CRC;
	// the amount of data to be sent
	int bytes_to_send;
	// the physical data to be put in the packet
	char physical_data_array[MAXDATASIZE];
	// specifies the size of the frame, which will always bet 10 greater that the number of bytes to be sent
	int resTransAttempts;
    int frame_size; 
	private:
};

#endif
