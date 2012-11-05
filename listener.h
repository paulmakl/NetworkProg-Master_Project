/*
 * This class runs in a glorious infinite loop in undying devotion to the high and mighty demiBrad
 * working ceaslessly, once started, as his reciver of messages. With its ear to the ground hearing even
 * the most lightfooted packets and determining their usefulness to us acting accordingly on information recived.
 * This means either alerting the sender the status of ACKs(ours or others), or saving data for our lordly demiBrad.
 * Weston Thornburg
 */

#include <iostream>
#include <tuple>
#include "RF.h"
#include <Packet.h>
#include "CircularFifo.h"
#include <tuple>
using namespace std;


class Listener
{
public:

    /*
     * constructor for the listener class that sets up all our sexy varribles and
     * starts the thread listening for imcoming messages
     */
    Listener(RF* RFLayer, CircularFifo<int,10>* incomingQueue, unsigned short* sendFlag, bool* receivedFlag, unsigned short myMAC);

private:


    unsigned short* MACaddr; //a pointer to our MAC address
    ostream *streamy; //the given output stream for data to the layer above
    unsigned short* MACACK;//a pointer to the MAC address of the most recent sender of data that has not been sent an ACK yet
    // or assuming that none need to be sent a special case of zero should be used to indicate this
    bool* ack_Received;// a pointer to a boolean that indicates whether or not a ACK has been recived
    static const int MAXPACKETSIZE = 2048; //size guarenteed to hold all properly formated packets
    char buf[MAXPACKETSIZE];// buffer for the incoming packets
    CircularFifo<int,2> daLoopLine;
    RF* daRF;//the reference to the RF layer
    int bytesReceived;// bytes from the last packet


    /*
     * the heart of the listener watches activity on the RF layer and blocks until a packet is recived
     */
    int UltraListen();

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
