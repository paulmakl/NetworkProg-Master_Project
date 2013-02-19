/*
 *
 * this will contain out DemiBrad class.
 * DemiBrad does lots of useful things like
 * store data in queues and make threads.
 *
 */

// Inlcude everything we need
#pragma once
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
		DemiBrad():MACaddr_demibrad(0), ack_Received_demibrad(false), MACACK_demibrad(0), expected_sequence_number(0) {};
		DemiBrad(short MACaddr, ostream *streamy);
		int dot11_command_DemiBrad(int cmd, int val);
		int status_DemiBrad();
		int dot11_recv_DemiBrad(short *srcAddr, short *destAddr, char *buf, int bufSize);
		/*
		 * send a packet to the address provided, with the
		 * information, starting at the specified buffer
		 * of the specified size.
		 */
		int dot11_send_DemiBrad(short destAddr, char *buf, int bufSize);
		short MACaddr_demibrad; // users mac address
		ostream* streamy_demibrad; // provided ostream
		volatile bool ack_Received_demibrad; // flag for acknowledgment received
		volatile short MACACK_demibrad; // the address that is associated with the next Acknowledgement. Is zero if none need to be sent
		//unsigned short send_flag_demibrad; // flat that lets the sender know when to send
		RF* RFLayer_demibrad; // the RF layer associated with Demibrad
		queue<Packet> send_Queue_demibrad; // the queue of packets to send
		queue<Packet> receive_Queue_demibrad; // the queu of packets received from the receiver class
		volatile short expected_sequence_number;
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
int dot11_send(short destAddr, char *buf, int bufSize);
