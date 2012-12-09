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
    Packet(short destaddr, short seqnum, short sorcey);
    // Creates a resend packet
	int make_resend();
	// builds a packet into a byte array
	int buildByteArray(char *buffer);
	// takes the data out of a physical array and coppies it into the packet class
	void pointer_data_to_physical(char* data);
	// takes a pointer to an array and filles the first four blocks with the CRC-32
	void build_CRC(char *data, int data_length, int CRC_TT);
	// shifts an entire array of characters over by 1 character
	//void shift_char_array(char *data, int size);
	// this is super complicated i will explain it when its done
	//void xor_crc(char *data, int CRC_TT, int sizenum);
	char get_nth_bit(char dta, int n);
	char get_nth_bit(int dta, int n);
	//char get_nth_bit(int dta, int n);
	void flip_nth_bit(char *dta, int n);
	//void calc_CRC(char *dta, int size, int CRCTT);
	void pointer_data_to_physical_universal(char* data, char *put_data_here, int size);
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
