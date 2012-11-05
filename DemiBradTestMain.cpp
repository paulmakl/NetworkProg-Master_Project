/* 
 *
 * THIS IS AN EXAMPLE OF USING PACKETS
 * - Paul
 */

#include <stdlib.h>
#include <iostream>
#include "DemiBrad.h"
#include "packet.h"
#include "CircularFifo.h"

using namespace std;

int main(int argc, char const *argv[])
{
	CircularFifo<Packet, 3> infoToSend;
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
	Packet packy;
	packy.initPacket(3,true,5,257,514,test,67372036,bytes_to_send);
	infoToSend.push(packy);
	Packet rawr;
	rawr.initPacket(3,true,5,257,514,test,3,bytes_to_send);
	infoToSend.pop(rawr);
	cout << rawr.get_crc() << "\n";
	


	/*int a = 1;
	int b = 2;
	int c = 3;
	int * e = &a;
	int * f = &b;
	int * g = &c;
	infoToSend.push(e);
	infoToSend.push(f);
	infoToSend.push(g);
	int i = 0;
	while(i < 3){
		int x = 0;
		i++;
	}*/
	//int * y = &x[0];


	short MACaddrr = 3; // users mac address
	ostream* streamyy; // provided ostream

	DemiBrad yar;
	yar.dot11_init(MACaddrr, streamyy);
    return 0;
}



/* IGNORE THIS
g++ DemiBradTestMain.cpp Demibrad.cpp packet.cpp RF.cpp -o bcast -I/System/Library/Frameworks/JavaVM.framework/Headers -L/System/Library/Frameworks/JavaVM.framework/Libraries -ljvm  -framework JavaVM
*/
