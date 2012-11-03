/**
 * This class specifies the methods for the Sender class
 * 
 * Author: Erin Jamroz
 * Date: October 2012
 * Class: CSCI 325 University of Puget Sound
 */

using namespace std;
#include <iostream>
#include "packet.h"
#include "RF.h"
#include "circularfifo.h"

class Sender {
    public:
        /**
         * Constructor for objects of the Sender class
         * @param RFLayer Pointer to the RF layer used when transmitting
         * @param theQueue Pointer to the outgoing messages queue
         * @param recievedFlag Pointer to the flag marking received Ack's
         * @param ourMAC our MAC address
         * param sendFlag Pointer to flag marking the destination to send an Ack to
         */
        sender(RF* RFLayer, queue* theQueue, unsigned short* sendFlag,
                bool* receivedFlag, unsigned short ourMAC);

        /**
         * Invokes the sender object to do all of its duties
         */
        void MasterSend();

    private:
    //Fields
        RF* theRF; //Pointer to the RF layer
        short* MacAddr; //Our MAC address
        ostream* dataStream; //ostream provided to us
        CircularFifoi<<short,char,int>*>* infoToSend //TODO is this syntactically correct? 
        //queue<short,char,int>* outgoing_Queue:  //pointer to outgoing message queue
        bool*  ackReceived; //Pointer to flag for received acks
        unsigned short* ackToSend; //Pointer to destination addr to send Ack
        Packet pachyderm; //The packet to send

    //Methods
        
        //TODO Do you actuall need these top three methods because you are just checking fields?
        /**
         * Checks the sender queue for data to be sent and checks the Ack
         * field for flags indicated acknowledgments needing to be sent
         * @return 0 nothing to send
         * @return 1 data to send
         */
        int  check_QueueToSend();

        /**
         * Checks for an acknowledgment received
         * @return 0 No ack received
         * @return Mac address of received ack  
         */
        //TODO How will you tell which message an ack is for using this system?
        unsigned short check_ReceivedAck();

        /**
         * Checks to see if an ack needs to be sent
         * @return 0 No ack needs to be sent
         * @return The MAC address to send and ack to 
         */
        unsigned short check_SendAck();

        /**
         * Builds a packet object for sending
         * @param frm
         * @param resend Indicates if this packet is part of  a retransmission
         * @param seqNum This packets sequence number
         * @param destAddr The destination's MAC address
         * @param sendAddr The sender's MAC address
         * @param data The data to transmit
         * @param CS
         //TODO What are CS and frm?
         * @return 1 if packet was successfully built
         */
        int buildPacket(enum frm, bool resend, unsigned short seqNum, 
                unsigned short destAddr, unsigned short senderAddr,
                *char data, int CS);
       
        /**
         * Sends a packet
         * @param thePacket The packet to send
         * @return 1 if the packet was sent correctly
         */
        int send(Packet thePacket);

        /**
         * Resends the current packet
         * @return 1 if packet was successfully sent
         */
        int resend();
}

















