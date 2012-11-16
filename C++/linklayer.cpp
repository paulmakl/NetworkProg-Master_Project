/*
 * This is a FAKE 802.11~ "layer".  It exists only as a placeholder for
 * your code, and to demonstrate writing to the output stream provided
 * by the client.
 */

#include <iostream>
#include "linklayer.h"  // Prototypes & status codes for 802.11~
#include "RF.h"         // Physical layer prototypes & constants

using namespace std;

ostream *out;       // The output stream to which we send diagnostic output
RF *theRF;          // A pointer to the physical layer
short theMACaddr;   // Our MAC address


int dot11_init(short MACaddr, ostream *output)
{
	// Store the output stream and MAC address for later use
	out = output;	
	theMACaddr = MACaddr;

	// An example of printing to the stream:
	*out << "dot11_init initializing link layer with MAC address " << MACaddr << endl;

	// Create the physical layer and return
	theRF = new RF();
	return 0;
}


int dot11_send(short destAddr, char *buf, int bufSize)
{
	// Currently just calls the RF layer's broadcast function.
	// Eventually, it'll need to implement the full CSMA/CA mechanism.
	return theRF->transmit(buf, bufSize);
}


int dot11_recv(short *srcAddr, short *destAddr, char *buf, int bufSize)
{
	// Set up a fake source and destination address.  Call the RF
	// layer to receive some data.
	*srcAddr = 444;
	*destAddr = 555;
	return theRF->receive(buf, bufSize);
}


int dot11_status()
{
	return 1;	// Always return 1, for now...
}


int dot11_command(int cmd, int val)
{
	*out << "Fake 802 layer was passed command: " 
		<< cmd << ", " << val << endl;
	return 0;
}
