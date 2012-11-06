/* 
 * Author: Erin Jamroz
 * Dater: November 2012
 * class: CSCI 325 University of Puget Sound
 */

using namespace std;
#include <iostream>
#include "sender.h"
#include <unistd.h>

Sender::Sender(RF* RFLayer, CircularFifo<int,2>* theQueue, unsigned short* sendFlag,
                bool* receivedFlag, unsigned short ourMAC) {
    //Initialize fields
    theRF = RFLayer;
    macAddr = ourMAC;
    infoToSend = theQueue;
    ackReceived = receivedFlag;
    ackToSend = sendFlag;
    seqNum = 0; //Initialize sequnece number to 0
}

void Sender::MasterSend() {
    //FOR TESTING PURPOSES
    unsigned char b = 'a';
    unsigned char* test = &b;

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
            buildPacket('1', false, seqNum, 111, test, 1111, 100); //FOR TESTING 
            //Send(frame);
            //start Timer to chech for timeouts
        }

        //TODO Handle case of timeout and resend
    }
}

int
Sender:: buildPacket(short frm, bool resend, unsigned short seqNum,
            unsigned short destAddr, unsigned char* data, int CS, int size) {
    Packet temp = initPacket(frm, resend, seqNum, destAddr, macAddr, data, CS, size);
    pachyderm = &temp; 
    pachyderm->buildByteArray(); //Frame gets the byte array to transmit
}
/*
int 
Sender::send(unsigned char* theFrame) {
    //Listen to see if channel is open
    if (!theRF->inUse()) { //The channel is clear
        if (theRF->transmit(*theFrame, pachyderm->) != pachyderm->frame_size) {
            return 0; //Did not send all of frame or
           }
        else {
            return 1; //Frame transmitted properly 
        }
    }
    else { //The channel is busy
        sleep(SLEEPTIME); //sleep
    }
}
*/
int 
Sender::resend() {
    //Build Packet with a 1 for resend
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















