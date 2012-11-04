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

Sender::Sender(RF* RFLayer, CircularFifo<int,2>* theQueue, unsigned short* sendFlag,
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
            //TODO Sleep
        }
        else {
            
        }
    }
}

int
Sender:: buildPacket(char frm, bool resend, unsigned short seqNum,
            unsigned short destAddr, unsigned short senderAddr, 
            char* data, int CS) {
    pachyderm.frameType = frm;
    pachyderm.resend = 0;
    pachyderm.sequence_number = seqNum;
    pachyderm.destination = destAddr;
    pachyderm.sender = MacAddr;
    pachyderm.data = data;
    pachyderm.CRC = CS; 
}

int 
Sender::send(Packet thePacket) {

}

int 
Sender::resend() {

}













