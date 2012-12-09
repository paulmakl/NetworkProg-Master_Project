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
 
 // The SIFS inter-frame spacing time, in milliseconds 
#define aSIFSTime    100

// The slot time, in milliseconds 
#define aSlotTime    200

// The minimum size of the collision window 
#define aCWmin       3

// The maximum size of the collision window 
#define aCWmax       31

// The maximum nubmer of retransmission attempts 
#define dot11RetryLimit    5

// The maximum number of bytes allowed in an RF packet 
#define aMPDUMaximumLength    2048

 using std::queue; 

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
        Sender(RF* RFLayer, 
                    queue<Packet> *theQueue,
                    volatile bool *receivedFlag, 
                    short ourMAC, 
                    volatile short *expSeq, 
                    pthread_mutex_t *mutex, 
                    volatile int *statusCode, 
                    volatile int *cmdCode, 
                    pthread_mutex_t *statusMutex, 
                    pthread_mutex_t *mutexSenderOstreamInput, 
                    pthread_mutex_t *mtxDemibradFdgFctr, 
                    volatile long long *fdgFctrDemibrad,
                    ostream *output)
                    :   theRF(RFLayer),  
                        infoToSend(theQueue), 
                        ackReceived(receivedFlag), 
                        macAddr_Sender(ourMAC), 
                        expSeqNum(expSeq), 
                        mutexSender(mutex), 
                        statCode(statusCode),
                        cmdVals(cmdCode),
                        mutexStatus(statusMutex),
                        mutexSenderOstream(mutexSenderOstreamInput),
                        mutexFudgeFactor(mtxDemibradFdgFctr),
                        fudgeFactor(fdgFctrDemibrad),
                        outputBuff(output),
                        seqTable(MAXSEQNUM),
                        cmd0(0), cmd1(0), cmd2(0), cmd3(0),
                        waitTime(0) {}
        
        /**
         * Invokes the sender object to do all of its duties
         */
        void MasterSend();

    private:
    //Fields
        //From paramaters (in order)
        RF* theRF; //Pointer to the RF layer
        queue<Packet> *infoToSend; //A queue to check for outgoing data 
        volatile bool* ackReceived; //Pointer to flag for received acks
        short macAddr_Sender; //Our MAC address
        volatile short *expSeqNum; //The expected seq num to see in an incomming ack
        pthread_mutex_t *mutexSender; //pointer to the lock for the queue
        volatile int *statCode; //Pointer to the  current status code
        volatile int *cmdVals;  //Pointer to 1st item in cmd value array
        pthread_mutex_t *mutexStatus; //Pointer to the mutex protecting statuses 
        pthread_mutex_t *mutexSenderOstream;    //Lock for the ostream
        pthread_mutex_t *mutexFudgeFactor;  //lock for accessing the fudge factor
        volatile long long *fudgeFactor;    //pointer to the fudge factor to align our clock 
                                                              //with the RF layer clock
        ostream *outputBuff; //Pointer to the diagnostic ostream
        //Internal fields  
        Packet pachyderm; //The packet to send
        static const short MAXSEQNUM = 4095;
        static const int WAITTIME = 10000000;    //Wait time (milsec)between ack's 
        //char* frame; //The byte array to be transmitted on RF
        SeqNumManager seqTable; //Manages all seqNums for all MAC addr's
        static const long long TRANSTIME = 0;    //The amount of time it takes to build      TODO: ALTER THIS VALUE TO ONE GATHERED EMPERICALLY 
                                                                               //and send a frame, used for beacons
        int cmd0;   //Gets cmd 0 value 
        int cmd1;   //Gets cmd 1 value 
        int cmd2;   //Gets cmd 2 value 
        int cmd3;   //Gets cmd 3 value 
        int waitTime; //Gets collision windo choice

        /**
        * Builds a beacon frame
        * param timeParam The RF time plus our fudge factor for adjustment
        * param beaconTime The time it takes to build a beacon
        */
        void buildBeacon(long long timeParam, const long long beaconTime);

        /**
        * Pulls out a particular bytes from a long long
        * param number the long long to pull a byte out of
        * param index the byte to pull out of number
        * return The byte pulled out 
        */
        unsigned char pullByte(long long number, int index);

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
        int send(char* frame, int size, bool reSend, int cWparam);

        /**
         * Resends the current packet
         * return 1 if packet was successfully sent
         */
        int resend();
};
