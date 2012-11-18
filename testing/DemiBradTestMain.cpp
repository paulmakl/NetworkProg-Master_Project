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
