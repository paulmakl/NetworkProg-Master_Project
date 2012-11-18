/*
 *
 * this will contain out DemiBrad class.
 * DemiBrad does lots of useful things like
 * store data in queues and make threads.
 *
 */

// Inlcude everything we need
#include "circularfifo.h"
#include "packet.h"
#include "RF.h"
#include "sender.h"
#include "listener.h"

using namespace std;

class DemiBrad{

	public:
		/* 
		 * Initialize the Demibrad Class.
		 * Starts the RF layer, a sender thread, and a receiver thread
		 */
		int dot11_init(short MACaddr, ostream *streamy);
		/*
		 * Not implemented yet...
		 */
		int dot11_command(int cmd, int val);
		/*
		 * Not implemented yet...
		 */
		int status();
		/*
		 * Receive the first item on the outgoing data queue.
		 */
		int dot11_recv(short *srcAddr, short *destAddr, char *buf, int bufSize);
		/*
		 * send a packet to the address provided, with the
		 * information, starting at the specified buffer
		 * of the specified size.
		 */
		int dot11_send(short destAddr, char *buf, int bufSize);
		//void *create_and_run_sender_thread(void *cnt);
		//void *create_and_run_receiver_thread(void *cnt);
	private:
		short MACaddr_demibrad; // users mac address
		ostream *streamy_demibrad; // provided ostream
		bool ack_Received_demibrad; // flag for acknowledgment received
		short MACACK_demibrad; // the address that is associated with the most recent Acknowledgement
		  CircularFifo<Packet*, 10> send_Queue_demibrad; // a queue containing the packets for the sender thread to send
		  CircularFifo<Packet*, 10> receive_Queue_demibrad; // a queue containing the received sender packets.
		int counts_demibrad[3]; //
		int memory_buffer_number_count_demibrad;
		Packet memory_buffer_demibrad[500];
		//RF* RFLayer;
		//ncoming_Queue queue <short, char, int> // a queue for incoming data
		//outgoing_Queue queue <short, char, int> // a queue for outgoing data 


};
