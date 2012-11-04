/**
 * Author: Erin Jamroz
 * Dater: November 2012
 * class: CSCI 325 University of Puget Sound
 */

using namespace std;
#include <iostream>
#include "sender.h"

Sender::Sender(RF* RFLayer, CircularFifo<int,2>* theQueue, unsigned short* sendFlag,
                bool* receivedFlag, unsigned short ourMAC) {
    //Initialize fields
    theRF = RFLayer;
    MacAddr = ourMAC;
    infoToSend = theQueue;
    ackReceived = receivedFlag;
    ackToSend = sendFlag;
    seqNum = 0; //Initialize sequnece number to 0
}

void
Sender::MasterSend() {
    //FOR TESTING PURPOSES
    char b = 'a';
    char* test = &b;

    while (true) {
        //Check for Ack to send
        if (ackToSend != 0) {
        //TODO Implement listen for Ack's to send
        }

        //Check for data to send
        if (infoToSend->isEmpty() == true) {
            //TODO Sleep
        }
        else {
            //Follow pointer
            //buildPacket
            buildPacket('1', 0, seqNum, 111, test, 1111); 
            //Send()
            //start Timer
        }

        //TODO Handle case of timeout and resend
    }
}

int
Sender:: buildPacket(char frm, bool resend, unsigned short seqNum,
            unsigned short destAddr, char* data, int CS) {
    pachyderm.frametype = frm;
    pachyderm.resend = 0;
    pachyderm.sequence_number = seqNum;
    pachyderm.destination = destAddr;
    pachyderm.sender = MacAddr;
    pachyderm.data = data;
    pachyderm.CRC = CS; 
}

int 
Sender::send(Packet thePacket) {
    //Listen to see if channel is open
    if (!theRF->inUse()) {
        //Transmit        
    }
    else {
        //sleep
    }
}

int 
Sender::resend() {

}

void
Sender::incrementSeqNum() {
   if (seqNum < MAXSEQNUM) {
       seqNum++;
   }
   else {
       seqNum = 0;
   }
}















