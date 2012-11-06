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
	CircularFifo<Packet*, 3> queue_one;
	CircularFifo<Packet*, 3> * pointer_to_queue_one = &queue_one;
	(*pointer_to_queue_one).isEmpty();



	const int bytes_to_send = 5;
	char data[bytes_to_send];
	// fill in the data array with dummy data
	data[0] = 'A';
	data[1] = 'B';
	data[2] = 'C';
	data[3] = 'D';
	data[4] = 'E';
	// create a pointer to the data
	char* test = &data[0];
	// create a packet
	Packet packet_one;
	packet_one.initPacket(3,true,5,257,514,test,67372036,bytes_to_send);
	Packet * packpoint = &packet_one;
	(*pointer_to_queue_one).push(packpoint);
	Packet * packet_pointer;
	//packet_pointer.initPacket(3,true,5,257,514,test,3,bytes_to_send);
	(*pointer_to_queue_one).pop(packet_pointer);
	Packet imback = *packet_pointer;
	//cout << packet_pointer.get_crc() << "\n";*/


	short MACaddrr = 3; // users mac address
	ostream* streamyy; // provided ostream

	DemiBrad yar;
	yar.dot11_init(MACaddrr, streamyy);
	wcerr << "DOING IMPORTANT STUFF RIGHT HERE!!!!!";
	int j = 0;
	while(true){
		j++;
	}
    return 0;
}



/* IGNORE THIS
g++ DemiBradTestMain.cpp Demibrad.cpp packet.cpp RF.cpp -o bcast -I/System/Library/Frameworks/JavaVM.framework/Headers -L/System/Library/Frameworks/JavaVM.framework/Libraries -ljvm  -framework JavaVM
*/
