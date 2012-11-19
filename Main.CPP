/* 
 *
 * THIS IS AN EXAMPLE OF USING PACKETS
 * - Paul
 */

#include <stdlib.h>
#include <iostream>
#include "Demibrad.h"

using namespace std;

int main(int argc, char const *argv[])
{
	ostream* stream;
	//DemiBrad jesus;
	dot11_init(103, stream);
	int bufSize = 3;
	char buf[bufSize];
	buf[0] = 'a';
	buf[1] = 'b';
	buf[2] = 'c';
	dot11_send(101, &buf[0], bufSize);
	while(true){

	}
}



/* IGNORE THIS
g++ Main.cpp Demibrad.cpp packet.cpp sender.cpp listener.cpp RF.cpp -o bcast -I/System/Library/Frameworks/JavaVM.framework/Headers -L/System/Library/Frameworks/JavaVM.framework/Libraries -ljvm  -framework JavaVM

g++ Demibrad.cpp packet.cpp sender.cpp listener.cpp RF.cpp -o bcast -I/System/Library/Frameworks/JavaVM.framework/Headers -L/System/Library/Frameworks/JavaVM.framework/Libraries -ljvm  -framework JavaVM

g++ Demibrad.cpp Main.cpp packet.cpp sender.cpp listener.cpp RF.cpp -o bcast -I/System/Library/Frameworks/JavaVM.framework/Headers -L/System/Library/Frameworks/JavaVM.framework/Libraries -ljvm  -framework JavaVM
g++ RF_test.cpp RF.cpp -o bcast -I/System/Library/Frameworks/JavaVM.framework/Headers -L/System/Library/Frameworks/JavaVM.framework/Libraries -ljvm  -framework JavaVM
*/
