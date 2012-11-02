/*
 * This class runs in a glorious infinite loop in undying devotion to the high and mighty demiBrad
 * working ceaslessly, once started, as his reciver of messages. With its ear to the ground hearing even 
 * the most lightfooted packets and determining their usefulness to us acting accordingly on information recived.
 * This means either alerting the sender the status of ACK(ours or others), or saving data for our lordly demiBrad.
 */

#include <iostream>;
#include "RF.h";
#include <Packet.h>; 
using namespace std;

class Listener
{
public:

	/*
	 * constructor for the listener class that sets up all our sexy varribles and
	 * starts the thread listening for imcoming messages
	 */
	Listener();

private:

	short *MACaddr; //a pointer to our MAC address
	*ostream streamy; //the given output stream for data to the layer above
	*short MACACK;//a pointer to the MAC address of the most recent sender of data that has not been sent an ACK yet
	// or assuming that none need to be sent a special case of zero should be used to indicate this
	*queue incoming_Queue <short, char, int>;//pointer to the queue for incoming data to be held until the layer above ask for data
	*bool ack_Received;// a pointer to a boolean that indicates whether or not a ACK has been recived 

	/*
	 * the heart of the listener watches activity on the RF layer and blocks until data is recived
	 */
	int listen();

	/*
	 * looks at a packet to check for three things from every packet that comes across the the RF layer
	 * first it makes sure the packet conforms to the 802.11~ specs if not it disregards it
	 * second it looks to see if the packet is addressed to our MAC address if not it disregards it 
	 * finally if the packet is for us it determines if it is an ACK or incoming data responding accordingly
	 */
	 int read_packet();

	 /*
	  * if an incoming packet is for us and has data for us then this method packs the relevent information,
	  * the sender's MAC, an array of characters of the data, and the size of said array into a tupal that
	  * then is put into our incoming_Queue\
	  */
	  int queue_data();
};