/* 
 * Author: Erin Jamroz
 * Dater: November 2012
 * class: CSCI 325 University of Puget Sound
 */

//files included
#include <iostream> //For cout & endl
#include "sender.h"
#include <unistd.h> //For sleep()
#include <stdlib.h> //For rand()
#include <math.h> //for pwr

//Wait IFS (SIFS + 2*slotTime)
#define waitIFS sleep(EaSIFSTIME + 2*aSlotTime);

//Items used
using std::queue;


void 
Sender::MasterSend() {
    //FOR TESTING PURPOSES
   // wcerr << infoToSend << endl;
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
    //TODO Change to while loop

    //Wait for current channel to be idle
    if (!theRF->inUse()) {
        waitIFS
        if (!theRF->inUse()) {  //Perfect transmittion
           if (theRF->transmit(frame, size) != size) {  //Makes the transmission 
                return 0; //Did not send all of frame or
            } else {
                return 1; //Frame transmitted properly 
            }
        }
    } else {    //Channel was busy
        //Wait for channel to open
        bool idleFlag = false;
        while (!idleFlag) {
            while (theRF->inUse()) {
                sleep(SLEEPTIME); //TODO Lower SLEEPTIME
            }
            waitIFS
            //Check is channel is idle
            if (!theRF->inUse()) {
                idleFlag = true;    //Break
            }
        }
        //Exponential backoff!
        int cWindow = aCWmin;
        while (cWindow <= aCWmax) {
            int waitTime // = 2^ and shit of that sort 

            //Checking if someone has sent while we are waiting
            if (theRF->getIdleTime() < ) {

            }
        }

    }


    //Channel still idle?
    if (!theRF->inUse()) { //The channel is clear
        //TODO Wait IFS
        //  Check if medium still idle
        if (theRF->transmit(frame, size) != size) {  //Makes the transmission 
            return 0; //Did not send all of frame or
           }
        else {
            return 1; //Frame transmitted properly 
        }
        //Busy:
        //Wait till transmition ends
        //EXPONENTIAL BACKOFF 
        //Send
    }
    else { //The channel is busy
        sleep(SLEEPTIME); //sleep
    }
}

int 
Sender::resend() {
   return 0;//nuild Packet with a 1 for resend
}














