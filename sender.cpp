/* 
 * Author: Erin Jamroz
 * Dater: November 2012
 * class: CSCI 325 University of Puget Sound
 */

using namespace std;
#include <iostream>
#include "sender.h"
//#include "DemiBrad.h"
#include <unistd.h>

Sender::Sender(RF* RFLayer, CircularFifo<Packet*,10>* theQueue, unsigned short* sendFlag,
                bool* receivedFlag, unsigned short ourMAC) {
    //Initialize fields
    theRF = RFLayer;
    macAddr_Sender = ourMAC;
    infoToSend = theQueue;
    ackReceived = receivedFlag;
    ackToSend = sendFlag;
    seqNum = 0; //Initialize sequnece number to 0
}

void Sender::MasterSend() {
    //FOR TESTING PURPOSES
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
            Packet* temp;
            infoToSend->pop(temp); 
            //buildPacket
            //build('1', false, seqNum, 111, test, 1111, 100); //FOR TESTING 
            send(*temp);
            //start Timer to chech for timeouts
            delete temp; //Free memory because this is c++
        }

        //TODO Handle case of timeout and resend
    }
}

int
Sender:: buildFrame(short frm, bool resend, unsigned short seqNum,
            unsigned short destAddr, char* data, int CS, int size) {
    pachyderm.initPacket(frm, resend, seqNum, destAddr, macAddr_Sender, data, CS, size);
}

int 
Sender::send(Packet theFrame) {
    //Listen to see if channel is open
    if (!theRF->inUse()) { //The channel is clear
        if (theRF->transmit(theFrame.physical_frame, theFrame.frame_size) !=
                theFrame.frame_size) {    //Makes the transmission 
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















