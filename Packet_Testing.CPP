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
	int data_size = 3;
	char dta[data_size];
	dta[0] = 'd';
	dta[1] = 'b';
	dta[2] = 'c';
	Packet john;
	john.initpacket(44, &dta[0], data_size, 1001);
	char Byte_Array[data_size+10];
	int q = john.buildByteArray(&Byte_Array[0]);
	int i = 0;
	while(i < (data_size+10)){
		cout << Byte_Array[i] + 0 << " :: ";
		i++;
	}
	cout << "\n";
}



/* IGNORE THIS
g++ Main.cpp Demibrad.cpp packet.cpp sender.cpp listener.cpp RF.cpp -o bcast -I/System/Library/Frameworks/JavaVM.framework/Headers -L/System/Library/Frameworks/JavaVM.framework/Libraries -ljvm  -framework JavaVM

g++ Demibrad.cpp Main.cpp packet.cpp sender.cpp listener.cpp RF.cpp -o bcast -I/System/Library/Frameworks/JavaVM.framework/Headers -L/System/Library/Frameworks/JavaVM.framework/Libraries -ljvm  -framework JavaVM
g++ RF_test.cpp RF.cpp -o bcast -I/System/Library/Frameworks/JavaVM.framework/Headers -L/System/Library/Frameworks/JavaVM.framework/Libraries -ljvm  -framework JavaVM
*/
