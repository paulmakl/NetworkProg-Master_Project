/*
 *
 * this will contain out DemiBrad class.
 * DemiBrad does lots of useful things like
 * store data in queues and make threads.
 *
 */
 

#include <iostream>
#include <pthread.h>
using namespace std;

class DemiBrad{

	public:

		DemiBrad(short MACaddr, ostream *streamy);
		int dot11_command(int cmd, int val);
		int status();
		int dot11_recv(short *srcAddr, short *destAddr, char *buf, int bufSize);
		int dot11_send(short destAddr, char *buf, int bufSize);
		int create_sender_thread(bool *ack_);
		int create_Receiver_Thread();
		void receiveRun();
	private:
		short MACaddr; // users mac address
		ostream *streamy; // provided ostream
		bool ack_Received; // flag for acknowledgment received
		short MACACK; // the address that is associated with the most recent Acknowledgement
		//ncoming_Queue queue <short, char, int> // a queue for incoming data
		//outgoing_Queue queue <short, char, int> // a queue for outgoing data 


};