/**
 * This class specifies the methods for the Sender class
 * 
 * Author: Erin Jamroz
 * Date: October 2012
 * Class: CSCI 325 University of Puget Sound
 */
#pragma once

#include <iostream>
#include <pthread.h>
#include "packet.h"
#include "RF.h"
#include <queue>
#include "SeqNumManager.h"
 using std::queue; 

//#ifndef __RF_H_INCLUDED__   // if x.h hasn't been included yet...
//#define __RF_H_INCLUDED__
class Sender {
    public:
        /**
         * Constructor for objects of the Sender class
         * param RFLayer Pointer to the RF layer used when transmitting
         * param theQueue Pointer to the outgoing messages queue
         * param recievedFlag Pointer to the flag marking received Ack's
         * param ourMAC our MAC address
         * param expSeq Pointer to the Seq num to be expected in an ack
         * param mutex Pointer to the mutex to lock the queue
         */
        Sender(RF* RFLayer, queue<Packet> *theQueue,
                volatile bool *receivedFlag, short ourMAC, 
                volatile short *expSeq, pthread_mutex_t *mutex) 
                    :   theRF(RFLayer),  infoToSend(theQueue), ackReceived(receivedFlag), 
                        macAddr_Sender(ourMAC), expSeqNum(expSeq), mutexSender(mutex), 
                        seqTable(MAXSEQNUM) {}
        
        /**
         * Invokes the sender object to do all of its duties
         */
        void MasterSend();

    private:
    //Fields
        RF* theRF; //Pointer to the RF layer
        queue<Packet> *infoToSend; //A queue to check for outgoing data 
        volatile bool* ackReceived; //Pointer to flag for received acks
        short macAddr_Sender; //Our MAC address
        volatile short *expSeqNum; //The expected seq num to see in an incomming ack
        volatile short *macAckSeqNum; //The seqNum for to acknowlege that we have recieved
        pthread_mutex_t *mutexSender; //pointer to the lock for the queue
        //Internal fields  
        Packet pachyderm; //The packet to send
        static const short MAXSEQNUM = 4095;
        static const int SLEEPTIME = 1;    //Wait time (second) to check again if
                                                            //the network is free 
        char* frame; //The byte array to be transmitted on RF
        SeqNumManager seqTable; //Manages all seqNums for all MAC addr's

    //Methods
        /**
         * Checks the sender queue for data to be sent and checks the Ack
         * field for flags indicated acknowledgments needing to be sent
         * return 0 nothing to send
         * return 1 data to send
         */
        int  check_QueueToSend();

        /**
         * Checks for an acknowledgment received
         * return 0 No ack received
         * return Mac address of received ack  
         */
        //TODO How will you tell which message an ack is for using this system?
        short check_ReceivedAck();

        /**
         * Builds a packet object for sending
         * param frm the frame typ
         * param resend Indicates if this packet is part of  a retransmission
         * param seqNum This packets sequence number
         * param CS CRC
         * return 1 if packet was successfully built
         */
        int buildFrame(short frm, bool resend, short seqNum, int CS);
       
        /**
         * Sends a packet
         * param thePacket The packet to send
         * return 1 if the packet was sent correctly
         */
        int send(char* frame, int size);

        /**
         * Resends the current packet
         * return 1 if packet was successfully sent
         */
        int resend();
};
//#endif
