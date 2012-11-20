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
            
            //buildFrame(0, true, seqTable(pachyderm.destination), 1111)
            buildFrame(1, false, 0, 1111); //FOR TESTING, uncomment above line for actual

            //TODO add in seq numbering

            //Build the frame (char[]) to be send
            char theFrame[pachyderm.frame_size];
            char* pointerToTheFrame = &theFrame[0];
            pachyderm.buildByteArray(pointerToTheFrame); //Fill theFrame
            
            //Transmit
            send(pointerToTheFrame, pachyderm.frame_size, false, aCWmin);
           //TODO start Timer to check for timeouts
            //CAN ONLY RESEND 5 TIMES AND CONTENTION WINDOWN CAN ONLY GET UP TO 31
            //while(NoAckRecieved && notAtEndofTimer) {
            //          usleep(1000);
            //    }
            //     if (noAckRecieved) {
            //          retransmit
            //       }
        }
    }
             //Free memory because this is c++
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
Sender::send(char* frame, int size, bool reSend, int cWparam) {
    if (!reSend) {  //This is not a retransmittion 
        //Wait for current channel to be idle
        if (!theRF->inUse()) {
            waitIFS
            if (!theRF->inUse()) {  //Perfect transmittion
               if (theRF->transmit(frame, size) != size) {  //Makes the transmission 
                    return 0; //Did not send all of frame or something failed internally
                } else {
                    return 1; //Frame transmitted properly 
                }
            }
        } 
           //Channel was busy
            bool idleFlag = false;
            //Wait for channel to open
            while (!idleFlag) { 
                while (theRF->inUse()) {    //They are sending
                    usleep(1000); //Sleep 1 milSec
                }
                waitIFS
                //Check is channel is idle
                if (!theRF->inUse()) {
                    idleFlag = true;    //Break
                }
            }
    } //Possibility for this to be a resend
    //Exponential backoff!
    int cWindow = cWparam;
    int waitTime = rand() % pow(2, cWindow)//Random number in range [0,2^aCWmin)
    while (true) {
        while (waitTime>0 && !theRF->inUse()) {     //We havent waited waitTime and 
                                                                            //No one else is transmitting
            uSleep(1000);   //Sleep 1 milSec
            waitTime--;
        }   //StopClock
        if (waitTime>0) {   //We havent finished waiting, but someone had transmitted
                while (theRF->inUse()) {    //They are transmitting
                    usleep(1000);   //Sleep 1 milSec
                }
                waitIFS
        } else {
            if (theRF->transmit(frame, size) != size) {  //Makes the transmission 
                return 0; //Did not send all of frame or something failed internally
            } else {
                return 1; //Frame transmitted properly 
            }
        }
    }   //End Exponential Backoff
}

int 
Sender::resend() {
    buildFrame(0, true, seqTable(pachyderm.destination), 1111);
    char theFrame[pachyderm.frame_size];
    char* pointerToTheFrame = &theFrame[0];
    pachyderm.buildByteArray(pointerToTheFrame); //Fill theFrame
    pachyderm.reTransAttemps++;     //Increment the times we have tried to resend
    return send(pointerToTheFrame, pachyderm.frame_size, true, aCWmin + pachyderm.reTransAttemps); 
}














