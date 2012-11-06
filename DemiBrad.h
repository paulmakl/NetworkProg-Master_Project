/*
 *
 * this will contain out DemiBrad class.
 * DemiBrad does lots of useful things like
 * store data in queues and make threads.
 *
 */
 
#include "circularfifo.h"
#include "packet.h"
#ifndef DemiBrad_H
#define DemiBrad_H
using namespace std;

class DemiBrad{

	public:
		//DemiBrad();
		int dot11_init(short MACaddr, ostream *streamy);
		int dot11_command(int cmd, int val);
		int status();
		int dot11_recv(short *srcAddr, short *destAddr, char *buf, int bufSize);
		int dot11_send(short destAddr, char *buf, int bufSize);
		//void *create_sender_thread(void *cnt);
		//void *create_Receiver_Thread(void *cnt);
		void receiveRun();
	private:
		short MACaddr; // users mac address
		ostream *streamy; // provided ostream
		bool ack_Received; // flag for acknowledgment received
		short MACACK; // the address that is associated with the most recent Acknowledgement
		CircularFifo<Packet*, 10> send_Queue;
		CircularFifo<Packet*, 10> receive_Queue;
		int counts[2];
		//RF* RFLayer;
		//ncoming_Queue queue <short, char, int> // a queue for incoming data
		//outgoing_Queue queue <short, char, int> // a queue for outgoing data 


};

#endif