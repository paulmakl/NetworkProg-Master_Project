/* 
 *
 * THIS IS AN EXAMPLE OF USING PACKETS
 * - Paul
 */

#include <stdlib.h>
#include <iostream>
#include "packet.h"

using namespace std;

int main(int argc, char const *argv[])
{
	// to run this, just enter into terminal 'g++ main.cpp packet.cpp'
	// this is the number of bytes the user wants to send
	// does not include the frame.
	const int bytes_to_send = 5;
	unsigned char data[bytes_to_send];
	// fill in the data array with dummy data
	data[0] = 'A';
	data[1] = 'B';
	data[2] = 'C';
	data[3] = 'D';
	data[4] = 'E';
	// create a pointer to the data
	unsigned char* test = &data[0];
	// create a packet
	Packet packy(3,true,5,257,514,test,67372036,bytes_to_send);
	// 
	unsigned char data_with_frame[bytes_to_send + 10];
	unsigned char* pointer_two = &data_with_frame[0];
	packy.buildByteArray(pointer_two);
	int i = 0;
	while(i< (bytes_to_send+10)){
		cout << ": " << data_with_frame[i] + 0 << " :";
		i++;
	}
	//Packet billy;
	//billy.make_resend();//billy.makePacket();//(4,true,4,3,2,test,4,55);
    return 0;
}



/* IGNORE THIS
g++ sender.cpp RF.cpp packet.cpp -o bcast -I/System/Library/Frameworks/JavaVM.framework/Headers -L/System/Library/Frameworks/JavaVM.framework/Libraries -ljvm  -framework JavaVM
*/
