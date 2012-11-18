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

queue<Packet> * infoToSend;
pthread_mutex_t * mutexSender;

Sender::Sender(RF* RFLayer, queue<Packet> * theQueue, unsigned short* sendFlag, bool* receivedFlag, unsigned short ourMAC, pthread_mutex_t * mutexSendr)
{
    //Initialize fields
    theRF = RFLayer;
    macAddr_Sender = ourMAC;
    infoToSend = theQueue;
    ackReceived = receivedFlag;
    ackToSend = sendFlag;
    mutexSender = mutexSendr;
    seqNum = 0; //Initialize sequnece number to 0
}

void Sender::MasterSend() {
    //FOR TESTING PURPOSES
    wcerr << infoToSend << endl;
    while (true) {
        //Check for Ack to send
        if (ackToSend != 0) {
        //TODO Implement listen for Ack's to send
        }

        //Check for data to send
        if (infoToSend->empty()) {
            //TODO Sleep
            wcerr << "QUEUE IS EMPTY" << endl;
            sleep(SLEEPTIME);
        }
        else {
            wcerr << " pop should happen" << endl;
            //Get incomplete packet to send
            pachyderm = infoToSend->front();
            infoToSend->pop(); 
             //Dereference temp to save a local copy of it
            
            //buildFrame(frameType, false,seqNum, CRC);
            buildFrame(1, false, seqNum, 1111); //FOR TESTING 
            
            //Build the frame (char[]) to be send
            char theFrame[pachyderm.frame_size];
            char* pointerToTheFrame = &theFrame[0];
            pachyderm.buildByteArray(pointerToTheFrame); //Fill theFrame
            
            //Transmit
            send(pointerToTheFrame, pachyderm.frame_size);

            //start Timer to check for timeouts
            //delete temp; //Free memory because this is c++
            delete pointerToTheFrame;
            //TODO Memory leak: theFrame is not dealt with
        }

        //TODO Handle case of timeout and resend
    }
}

int
Sender::buildFrame(short frm, bool resend, unsigned short seqNum, int CS) {
    pachyderm.frametype = frm;
    pachyderm.resend = resend;
    pachyderm.sequence_number = seqNum;
    pachyderm.sender = macAddr_Sender;
    pachyderm.CRC = CS;
}

int 
Sender::send(char* frame, int size) {
    //Listen to see if channel is open
    if (!theRF->inUse()) { //The channel is clear
        if (theRF->transmit(frame, size) != size) {  //Makes the transmission 
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
   return 0;//nuild Packet with a 1 for resend
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















