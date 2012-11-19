/* 
 * Author: Erin Jamroz
 * Dater: November 2012
 * class: CSCI 325 University of Puget Sound
 */

#include <iostream>
#include "sender.h"
#include <unistd.h>
using std::queue;

void 
Sender::MasterSend() {
    //FOR TESTING PURPOSES
    wcerr << infoToSend << endl;
    while (true) {
        //Lock mutex, block until you can
        pthread_mutex_lock(mutexSender);

        //Check for data to send
        if (infoToSend->empty()) {
            //wcerr << "QUEUE IS EMPTY" << endl;
            pthread_mutex_unlock(mutexSender); //Unlock because the queue is not ready
            sleep(SLEEPTIME);
        }
        else {
            //wcerr << " pop should happen" << endl;
           
            //Get incomplete packet to send
            pachyderm = infoToSend->front();
            infoToSend->pop(); 
            pthread_mutex_unlock(mutexSender); //Unlock bc we are done with queue 
            
            //buildFrame(frameType, false,seqNum, CRC);
            buildFrame(1, false, 0, 1111); //FOR TESTING 

            //TODO add in seq numbering

            //Build the frame (char[]) to be send
            char theFrame[pachyderm.frame_size];
            char* pointerToTheFrame = &theFrame[0];
            pachyderm.buildByteArray(pointerToTheFrame); //Fill theFrame
            
            //Transmit
            send(pointerToTheFrame, pachyderm.frame_size);

            //TODO start Timer to check for timeouts

             //Free memory because this is c++
        }

        //TODO Handle case of timeout and resend
    }
}

int
Sender::buildFrame(short frm, bool resend,  short seqNum, int CS) {
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














