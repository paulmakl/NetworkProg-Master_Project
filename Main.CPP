/* 
 *
 * THIS IS AN EXAMPLE OF USING PACKETS
 * - Paul
 */

#include <stdlib.h>
#include <iostream>
#include "Demibrad.h"
#include <time.h>

using namespace std;

int main(int argc, char const *argv[])
{
	ostream* stream;
	//DemiBrad jesus;
	short dest = 103;
	short myMAC = 101;
	dot11_init(myMAC, stream);
	wcerr << "Creating a buffer" << endl;
	int bufSize = 3;
	char buf[bufSize];
	buf[0] = 'a';
	buf[1] = 'b';
	buf[2] = 'c';
	wcerr << "Testing..." << endl;
	while(true){
		/*wcerr << "Sending 'abc'..." << endl;
		dot11_send(dest, &buf[0], bufSize);
		wcerr << "Message sent!" << endl;*/
		wcerr << "Creating temporary Variables ..." << endl;
		short srcAddr,destAddr = -1;
		char buf[2048];
		wcerr << "Created Variables!" << endl;
		wcerr << "Checking incoming data queue..." << endl;
		int incDataResult = dot11_recv(&srcAddr, &destAddr, &buf[0], 2048);
		if (incDataResult == -1)
		{
			wcerr << "No Data in queue" << endl;
		}else{
			wcerr << "Data in queue detected! Printing..." << endl;
			int i = 0;
			while(i < incDataResult){
				wcerr << buf[i] << " :: ";
				i++;
			}
			wcerr << "Printing done!" << endl;
		}
		wcerr << "Sleeping." << endl << "---------------------" << endl << endl;
		sleep(3);
	}
}



/* IGNORE THIS
g++ Main.cpp Demibrad.cpp packet.cpp sender.cpp listener.cpp RF.cpp -o bcast -I/System/Library/Frameworks/JavaVM.framework/Headers -L/System/Library/Frameworks/JavaVM.framework/Libraries -ljvm  -framework JavaVM

g++ Demibrad.cpp packet.cpp sender.cpp listener.cpp RF.cpp -o bcast -I/System/Library/Frameworks/JavaVM.framework/Headers -L/System/Library/Frameworks/JavaVM.framework/Libraries -ljvm  -framework JavaVM

g++ Demibrad.cpp Main.cpp packet.cpp sender.cpp listener.cpp RF.cpp -o bcast -I/System/Library/Frameworks/JavaVM.framework/Headers -L/System/Library/Frameworks/JavaVM.framework/Libraries -ljvm  -framework JavaVM
g++ RF_test.cpp RF.cpp -o bcast -I/System/Library/Frameworks/JavaVM.framework/Headers -L/System/Library/Frameworks/JavaVM.framework/Libraries -ljvm  -framework JavaVM
*/
