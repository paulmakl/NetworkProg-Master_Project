#include <iostream>
#include <pthread.h>


		short MACaddr // users mac address
		*ostream streamy // provided ostream
		bool ack_Received // flag for acknowledgment received
		short MACACK // the address that is associated with the most recent Acknowledgement
		//incoming_Queue queue <short, char, int> // a queue for incoming data
		//outgoing_Queue queue <short, char, int> // a queue for outgoing data 
using namespace std;

		dot11_init(short MACadr, *ostream stremy){
			MACaddr = MACadr;
			streamy = stremy;
			create_Receiver_Thread();
		}
		int create_Receiver_Thread(){

		}

		int dot11_command(int cmd, int val){
			return 1;
		}
		int status(){
			return 1;
		}
		int dot11_recv(short *srcAddr, short *destAddr, char *buf, int bufSize){
			return 1;
		}
		int dot11_send(short destAddr, char *buf, int bufSize){
			return 1;
		}
		int create_sender_thread(*bool ack_){
			return 1;
		}
		int create_Receiver_Thread(){
			return 1;
		}


