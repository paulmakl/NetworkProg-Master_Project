#include <iostream>
#include <arpa/inet.h>
#include <pthread.h> 
#include <time.h>
#include "packet.h"
using namespace std;

// Basic constructor. CS is the value for CRC this will eventually be taken out
Packet::Packet(short dest, char* dta, int size){
	frametype = 3; // test value
	resend = 5; // test value
	sequence_number = 30; // test value
	destination = dest;
	sender = 1001; // test value
	CRC = 46869594; // test value
	bytes_to_send = size;
	frame_size = size + 10;
	resend = false; // test value
    pointer_data_to_physical(dta); //Make the physical copy
}

//Weston's Additions: overwritten "constructor" to unpack data from listener going to demi brad
Packet::Packet(char *pac, int byts)
{
    int size = byts-10;//total size of incoming data minus 10 bytes of header and CRC
    bytes_to_send = size;
    wcerr << "*******************************************" << bytes_to_send << endl;
    char dataIn[size];//a new char array for just the incoming data
    short dataSource = pac[4] + 0;//extract the source address
    dataSource = dataSource << 8;
    dataSource = dataSource + pac[5];
    sender = dataSource;
    for (int i = 6; i < byts-4; ++i)//strip the headers and put just the data in our packet
    {
        dataIn[i-6] = pac[i];//the offset of six is the front header being skipped in our buf and the four less is the CRC
    }
    char* pointerToData = &dataIn[0];
    pointer_data_to_physical(pointerToData); //Make the physical copy
}
// Packet for acknowledgement
Packet::Packet(short destaddr, short seqnum){
	sequence_number = seqnum;
	destination = destaddr;
	bytes_to_send = 0;
	frame_size = 10;
	frametype = 1;
	resend = false;
	CRC = 92929;
}

// takes a pointer to an array of data and copies it into the phyical data array in the packet class
void Packet::pointer_data_to_physical(char* data){
	int i = 0;
	while(i < bytes_to_send){
		physical_data_array[i] = data[i];
		i++;
	}
}
// This is really confusing, carefully read each line and each comment.
int Packet::buildByteArray(char *buffer){
	// we are passed a buffer that is equal to frame size.
	// this is because this char array will be the final
	// packet that will be sent across the network.

	//These next few lines of code breaks the CRC into
	//four bytes. and then puts those bytes in the
	// last four slots of the packet.
	wcerr << "Checkpoint" << endl;
	int i = 0; // create a counter variable
	int temp_int = 0; // make a temporary integer.
	while(i < 4){
		temp_int = CRC << 24 - 8*i; // shift the ith byte of the integer all the way over to the left.
		temp_int = temp_int >> 24; // shift that byte back to its original position.
		buffer[frame_size-1-i] = temp_int; // add that byte to the appropriate section of the buffer.
		wcerr << buffer[frame_size-1-i]+0 << "::";
		i++;
	}
	wcerr << "Checkpoint" << endl;
	//Now we need to put the destination and sender addresses in their
	//proper positions
	short temp_short = 0; // create a temporary short variable.
	temp_short = sender << 8; // shift the first byte of sender the the leftmost position of the short
	temp_short = temp_short >> 8;// shift the previously mentioned byte back to its original position.
								 // this removes all the unnesessary zeros.
	buffer[5] = temp_short; // store this byte in the fifth position in the buffer
	temp_short = sender >> 8; // shift the second byte of the sender variable over to be the first byte.
	buffer[4] = temp_short; // store the second byte in the sixth position.
	// do the same process as above.
	temp_short = destination << 8;
	temp_short = temp_short >> 8;
	buffer[3] = temp_short;
	temp_short = destination >> 8;
	buffer[2] = temp_short;

	// this is kind of complicated.
	// the first two bytes of information need to store three variables.
	// they need to store the frame type, the retry bit and the sequence number.
	// The frame type takes up three bits, the retry takes up one, and the sequence number takes up 12.
	// EXAMPLE
	// 1   0   0   1   0   1   1   0      1   0   1   0   0   0   1   0
	// ---frame- retry -----------------sequence number----------------
	// when I stored destination address and sender address, I broke a short (2 bytes) up into two 1 byte
	// segments that could easily be put into the buffer. I take a similar approach here. I merge the three
	// variables into a short and then split it into two bytes to be put into the buffer.
	short temp_short_two = 0; // I needed a second temporary variable.
	temp_short = 0; // i zeroed out the first temporary variable, just to be safe.
	temp_short = frametype << 13; // the frame type is stored in a short, so I shift it over 13 bits so that only the
								  // last three bits will be in the leftmost position of the number
	if (resend == true) // if resend is true, then we need to flip the 13th bit. Otherwise we leave it at 0.
	{
		temp_short = temp_short + 4096; // 4096 is the number where only the 13th bit is flipped. 
	}
	//sequence number is stored in a short. We only want the first 12 bits. So we remove the last 4
	//with bitshift operations.
	temp_short_two = sequence_number << 4;
	temp_short_two = temp_short_two >> 4;
	temp_short = temp_short + temp_short_two; // temp_short now holds the short that we need to break up.
	// Now we break up the short into two packets. 
	temp_short_two = temp_short << 8;
	temp_short_two = temp_short_two >> 8;
	buffer[1] = temp_short_two;
	temp_short_two = temp_short >> 8;
	buffer[0] = temp_short_two;
	//at this point, our frame is completely build.
	// now we must fill our packet with data.
	i = 0; // reset our counter variable
	// the data we want is stored in the char array called 'data'
	// we want to put it in the buffer, but we want to put the data
	// after the header. we go through the data in 'data' and 
	// put it in the buffer.
	wcerr << "Checkpoint" << endl;
	while(i < bytes_to_send){
		buffer[i+6] = physical_data_array[i];
		wcerr << " ::" << i << physical_data_array[i];
		i++;
	}
	wcerr << "FIN" << endl;
}

