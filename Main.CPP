/* 
 *
 * This file will be used to try out all
 * RF layer goodies that we make
 * - Paul
 */

#include <stdlib.h>
#include <arpa/inet.h>
//#include "RF.h"
//#include "DemiBrad.h"
#include <pthread.h> 
#include <time.h>
#include "packet.h"
#include "MyClass.h"

using namespace std;

int main(int argc, char const *argv[])
{
	//char data[100];
	//char * test = &data[0];
	MyClass a;
	a.foo();
	Packet packy;
	packy.rawr();
	//Packet billy;
	//billy.make_resend();//billy.makePacket();//(4,true,4,3,2,test,4,55);
    return 0;
}