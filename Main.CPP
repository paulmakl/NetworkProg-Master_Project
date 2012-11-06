/* 
 *
 * THIS IS AN EXAMPLE OF USING PACKETS
 * - Paul
 */

#include <stdlib.h>
#include <iostream>
#include "DemiBrad.h"

using namespace std;

int main(int argc, char const *argv[])
{
	short MACaddrr = 3; // users mac address
	ostream* streamyy; // provided ostream

	DemiBrad yar;
	yar.dot11_init(MACaddrr, streamyy);
	wcerr << "DOING IMPORTANT STUFF RIGHT HERE!!!!!";
	char data[5];
	data[0] = 'A';
	data[1] = 'B';
	data[2] = 'C';
	data[3] = 'D';
	data[4] = 'E';
	yar.dot11_send(809, &data[0], 5);

    return 0;
}



/* IGNORE THIS
g++ Main.cpp Demibrad.cpp packet.cpp sender.cpp listener.cpp RF.cpp -o bcast -I/System/Library/Frameworks/JavaVM.framework/Headers -L/System/Library/Frameworks/JavaVM.framework/Libraries -ljvm  -framework JavaVM

g++ Demibrad.cpp Main.cpp packet.cpp sender.cpp listener.cpp RF.cpp -o bcast -I/System/Library/Frameworks/JavaVM.framework/Headers -L/System/Library/Frameworks/JavaVM.framework/Libraries -ljvm  -framework JavaVM
*/
