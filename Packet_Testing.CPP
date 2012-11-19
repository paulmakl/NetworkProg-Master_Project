/** 
 * THIS IS AN EXAMPLE OF USING PACKETS
 * - Paul
 */

#include <stdlib.h>
#include <iostream>
#include "packet.h"

using namespace std;

int main(int argc, char const *argv[])
{
	int data_size = 1;
	char dta[data_size];
	dta[0] = 'H';
	dta[1] = 'E';
	dta[2] = 'L';
	dta[0] = 'L';
	dta[1] = 'O';
	dta[2] = ' ';
	dta[0] = 'W';
	dta[1] = 'O';
	dta[2] = 'R';
	dta[0] = 'L';
	dta[1] = 'D';
	Packet john(101, &dta[0], data_size);
	char Byte_Array[data_size+10];
	int q = john.buildByteArray(&Byte_Array[0]);
	int i = 0;
	while(i < (data_size+10)){
		cout << Byte_Array[i]<< " :: ";
		i++;
	}
	cout << "\n";
}



/* IGNORE THIS
g++ Main.cpp Demibrad.cpp packet.cpp sender.cpp listener.cpp RF.cpp -o bcast -I/System/Library/Frameworks/JavaVM.framework/Headers -L/System/Library/Frameworks/JavaVM.framework/Libraries -ljvm  -framework JavaVM

g++ Demibrad.cpp Main.cpp packet.cpp sender.cpp listener.cpp RF.cpp -o bcast -I/System/Library/Frameworks/JavaVM.framework/Headers -L/System/Library/Frameworks/JavaVM.framework/Libraries -ljvm  -framework JavaVM
g++ RF_test.cpp RF.cpp -o bcast -I/System/Library/Frameworks/JavaVM.framework/Headers -L/System/Library/Frameworks/JavaVM.framework/Libraries -ljvm  -framework JavaVM
*/
