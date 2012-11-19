/*
 *
 * this will contain out DemiBrad class.
 * DemiBrad does lots of useful things like
 * store data in queues and make threads.
 *
 */

// Inlcude everything we need
#include "packet.h"
#include <pthread.h>
#include "RF.h"
#include "sender.h"
#include "listener.h"
#include <queue>
#include <iostream>

using namespace std;

class DemiBrad{

	public:
		DemiBrad():MACaddr_demibrad(0), ack_Received_demibrad(false), MACACK_demibrad(0), MACACK_sequence_number(0), MACACK_expected_sequence_number(0) {};
		DemiBrad(short MACaddr, ostream *streamy);
		/* 
		 * Initialize the Demibrad Class.
		 * Starts the RF layer, a sender thread, and a receiver thread
		 */
		//int dot11_init(short MACaddr, ostream *streamy);
		/*
		 * Not implemented yet...
		 */
		int dot11_command_DemiBrad(int cmd, int val);
		/*
		 * Not implemented yet...
		 */
		int status_DemiBrad();
		/*
		 * Receive the first item on the outgoing data queue.
		 */
		int dot11_recv_DemiBrad(short *srcAddr, short *destAddr, char *buf, int bufSize);
		/*
		 * send a packet to the address provided, with the
		 * information, starting at the specified buffer
		 * of the specified size.
		 */
		int dot11_send_DemiBrad(short destAddr, char *buf, int bufSize);
		//void *create_and_run_sender_thread(void *cnt);
		//void *create_and_run_receiver_thread(void *cnt);
		short MACaddr_demibrad; // users mac address
		ostream* streamy_demibrad; // provided ostream
		bool ack_Received_demibrad; // flag for acknowledgment received
		unsigned short MACACK_demibrad; // the address that is associated with the next Acknowledgement. Is zero if none need to be sent
		//unsigned short send_flag_demibrad; // flat that lets the sender know when to send
		RF* RFLayer_demibrad; // the RF layer associated with Demibrad
		queue<Packet> send_Queue_demibrad; // the queue of packets to send
		queue<Packet> receive_Queue_demibrad; // the queu of packets received from the receiver class
		unsigned short MACACK_sequence_number;
		unsigned short MACACK_expected_sequence_number;
		pthread_mutex_t mutex_Demibrad_Receiver;// = PTHREAD_MUTEX_INITIALIZER;
		pthread_mutex_t mutex_Demibrad_Sender;// = PTHREAD_MUTEX_INITIALIZER;
		pthread_mutexattr_t attr;
	//private:

		/*short MACaddr_demibrad; // users mac address
		ostream *streamy_demibrad; // provided ostream
		bool ack_Received_demibrad; // flag for acknowledgment received
		short MACACK_demibrad; // the address that is associated with the most recent Acknowledgement
		queue<Packet> send_Queue_demibrad; // a queue containing the packets for the sender thread to send
		queue<Packet> receive_Queue_demibrad; // a queue containing the received sender packets.
		//RF* RFLayer;
		//ncoming_Queue queue <short, char, int> // a queue for incoming data
		//outgoing_Queue queue <short, char, int> // a queue for outgoing data */


};

int dot11_init(short MACadr, ostream* stremy);
