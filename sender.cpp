/**
 * Author: Erin Jamroz
 * Dater: November 2012
 * class: CSCI 325 University of Puget Sound
 */

using namespace std;
#include <iostream>
#include "packet.h"
#include "RF.h"
#include "circularfifo.h"
#include "sender.h"

Sender::sender(RF* RFLayer, queue* theQueue, unsigned short* sendFlag,
                bool* receivedFlag, unsigned short ourMAC) {
    //Initialize fields
    theRF = RFLayer;
    MacAddr = ourMAC;
    infoToSend = theQueue;
    ackReceived = receivedFlag;
    ackToSend = sendFlag;
}

void
Sender::MasterSend() {
    while (true) {
        //Check for Ack to send
        if (ackToSend != 0) {
        //TODO Implement listen for Ack's to send
        }

        //Check for data to send
        if (infoToSend.isEmpty() == true) {

        }
    }
}

int
Sender:: buildPacket(enum frm, bool resend, unsigned short seqNum,
            unsigned short destAddr, unsiged short senderAddr, 
            *char data int CS) {
    

}

int 
Sender::send(Packet thePacket) {

}

int 
Sender::resend() {

}













