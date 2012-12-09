/*
 *
 * this will contain our DemiBrad class.
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
#include <time.h>

using namespace std;

class DemiBrad{

	public:
		DemiBrad():MACaddr_demibrad(0), ack_Received_demibrad(false),  expected_sequence_number(0), fudge_factor_Demibrad(0) {};
		DemiBrad(short MACaddr, ostream *streamy);

		/*
		 * sets the value of the commands availible to the user
		 * command o returns a list of avalible commands 
		 */
		int dot11_command_DemiBrad(int cmd, int val);

		/*
		 * prints the most recent status code to the the output stream
		 */
		int status_DemiBrad();

		/*
		 * returns any data queued up for delivery to the layer above along with the sequence number as well as the size of the incoming data
		 */
		int dot11_recv_DemiBrad(short *srcAddr, short *destAddr, char *buf, int bufSize);

		/*
		 * send a packet to the address provided, with the
		 * information, starting at the specified buffer
		 * of the specified size.
		 */
		int dot11_send_DemiBrad(short destAddr, char *buf, int bufSize);
		ostream* streamy_demibrad; // ostream for printing output to the layer above
		RF* RFLayer_demibrad; // the RF layer associated with Demibrad
		queue<Packet> send_Queue_demibrad; // the queue of packets to send
		queue<Packet> receive_Queue_demibrad; // the queue of packets received from the receiver class
		pthread_mutex_t mutex_Demibrad_Receiver;// listener's mutex for the incoming packet queue
		pthread_mutex_t mutex_Demibrad_Sender;// sender's mutex for outgoing packet queue
		pthread_mutex_t mutex_Demibrad_ostream;//a mutex for all threads when printing to the ostream
		pthread_mutex_t mutex_Demibrad_fudge_factor;//a mutex for the timestamp adjustment
		pthread_mutex_t mutex_attach_rflayer;// a mutex for when demibrad attaches the treads to the RF layer
		pthread_mutex_t statusMutex; // a mutex for adjusting the current status code
        pthread_mutexattr_t attr;//struct for intiting the mutex atributes
		volatile long long fudge_factor_Demibrad;//the offset for adjusting the RF layers clock  to network standard time
		volatile short expected_sequence_number;// a flag sender sets to alert listener to what sequence number to expect for incoming acks
		volatile int statusCode; //the current status for the must recent error/ect.
        volatile int cmdCode[4];//an array holding the command values
        volatile bool ack_Received_demibrad; // flag for acknowledgment received
        short MACaddr_demibrad; // users mac address
};


// outside methods, these are the methods that are available to the layer above that then call the corresponding
// method that is located in demibrad.
int dot11_init(short MACadr, ostream* stremy);
int dot11_send(short destAddr, char *buf, int bufSize);
int dot11_recv(short *srcAddr, short *destAddr, char *buf, int bufSize);
int dot11_command(int cmd, int val);
int dot11_status();
