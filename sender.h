/**
 * This class specifies the methods for the Sender class
 * 
 * Author: Erin Jamroz
 * Date: October 2012
 * Class: CSCI 325 University of Puget Sound
 */

using namespace std;
#include <iostream>
#include <pthread.h>
#include "packet.h"
#include "RF.h"
#include <queue>
#include "SeqNumManager.h"

//#ifndef __RF_H_INCLUDED__   // if x.h hasn't been included yet...
//#define __RF_H_INCLUDED__
class Sender : public SeqNumManager {
    public:
        /**
         * Constructor for objects of the Sender class
         * @param RFLayer Pointer to the RF layer used when transmitting
         * @param theQueue Pointer to the outgoing messages queue
         * @param recievedFlag Pointer to the flag marking received Ack's
         * @param ourMAC our MAC address
         * param sendFlag Pointer to flag marking the destination to send an Ack to
         */
        Sender(RF* RFLayer, queue<Packet> * theQueue, unsigned short* sendFlag,
                bool* receivedFlag, unsigned short ourMAC, 
                unsigned short *expSeq, pthread_mutex_t * mutex, 
                unsigned short *macAckSeq) 
                    : SeqNumManager(MAXSEQNUM) {};  //BRAD: here is where I attempt to 
                                                    //specify the SeqNumManager
                                                    //constructor to use
                        
                        /*
                         * Ignore this, was trying to implicitly instantiate 
                         * the sender class, but it failed
                         *
                         * theRf(RFLayer), macAddr_Sender(ourMac),
                    infoToSend(theQueue), expSeqNum(expSeq),
                    macAckSender(macAckSeq), ackRecieved(recievedFlag), 
                    ackToSend(sendFlag), mutexSender(mutex),
                    seqTable(SeqNumManager(MAXSEQNUM)) {};//TODO Figure out inisializer lists

                    */
        /**
         * Invokes the sender object to do all of its duties
         */
        void MasterSend();

    private:
    //Fields
        RF* theRF; //Pointer to the RF layer
        short macAddr_Sender; //Our MAC address
        ostream* dataStream; //ostream provided to us
        queue<Packet> *infoToSend; //A queue to check for outgoing data 
        unsigned short *expSeqNum; //The expected seq num to see in an incomming ack
        unsigned short *macAckSeqNum; //The seqNum for to acknowlege that we have recieved
        //queue<short,char,int>* outgoing_Queue:  //pointer to outgoing message queue
        bool* ackReceived; //Pointer to flag for received acks
        unsigned short* ackToSend; //Pointer to destination addr to send Ack
        Packet pachyderm; //The packet to send
        unsigned short seqNum; //The sequence number for transmitted packets
        static const unsigned short MAXSEQNUM = 4095;
        static const unsigned int SLEEPTIME = 1;  //Wait time (second) to check again if
                                                    //the network is free 
        char* frame; //The byte array to be transmitted on RF
        pthread_mutex_t *mutexSender; //pointer to the lock for the queue
        SeqNumManager seqTable; //Manages all seqNums for all MAC addr's

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
         * @param frm the frame typ
         * @param resend Indicates if this packet is part of  a retransmission
         * @param seqNum This packets sequence number
         * @param CS CRC
         * @return 1 if packet was successfully built
         */
        int buildFrame(short frm, bool resend, unsigned short seqNum, int CS);
       
        /**
         * Sends a packet
         * @param thePacket The packet to send
         * @return 1 if the packet was sent correctly
         */
        int send(char* frame, int size);

        /**
         * Increments the sequence number up to 4095 then wraps around to 0
         */
        void incrementSeqNum(); 

        /**
         * Resends the current packet
         * @return 1 if packet was successfully sent
         */
        int resend();
};
//#endif
