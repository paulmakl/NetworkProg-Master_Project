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
//FROM PAUL: This was removed since we now have
// our own power function
// #include <math.h> //for pwr

//Wait IFS (SIFS + 2*slotTime)
// FROM PAUL: the constant aSIFSTime was EaSIFSTIME. I think it was
// a mistake but I changed it to the name you had in your h file.
#define waitIFS usleep(aSIFSTime + 2*aSlotTime);

//Items used
using std::queue;


//FROM PAUL: impolementing the intPow function
// it simply does powers for ints. The native c++ 
// pow function does not like ints, it uses doubles and floats.
int Sender::intPow(int base, int power){
    int acc = 0;
    while(power > 0){
        acc = acc * base;
        power--;
    }
    return acc;
}

void 
Sender::MasterSend() {
    //FOR TESTING PURPOSES

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
            wcerr << "Got stuff off the queue" << endl;
            buildFrame(0, true, seqTable.getSeqNum(pachyderm.destination), 1111); // FROM PAUL: turned this on and fixed the function call
            //buildFrame(1, false, 0, 1111); //FOR TESTING, uncomment above line for actual

            //Build the frame (char[]) to be send
            char theFrame[pachyderm.frame_size];
            char* pointerToTheFrame = &theFrame[0];
            pachyderm.buildByteArray(pointerToTheFrame); //Fill theFrame
            
            //Transmit
            //FROM PAUL: need to check the result of send. So I store it in a temp variable.
            wcerr << "sending data" << endl;
            int doesItSend = send(pointerToTheFrame, pachyderm.frame_size, false, aCWmin);
            wcerr << "data sent!" << endl;
           //TODO start Timer to check for timeouts
            //CAN ONLY RESEND 5 TIMES AND CONTENTION WINDOWN CAN ONLY GET UP TO 31
            //while(NoAckRecieved && notAtEndofTimer) {
            //          usleep(1000);
            //    }
            //     if (noAckRecieved) {
            //          retransmit
            //       }
            // FROM PAUL: If the frame sent properly, then we want to check for a timeout.
            if (doesItSend != -1)
            {
                // FROM PAUL: If data was sent, then we check for a timeout.
                // we want to wait SIFS (100 milliseconds)
                int currentCW = aCWmin; // stores the current window size
                // FROM PAUL: loop to handle exponential backoff
                // if we actually send a packet, then the very next ack we get
                // should be for our previously transmitted message.
                while(ackReceived && pachyderm.resTransAttempts < 5){
                    usleep(aSIFSTime); // FROM PAUL: wait SIFS
                    if (ackReceived) // FROM PAUL: if we get an ack, then everything is GREAT!
                                     // and we break.
                    {
                        break;
                    }else{
                        // FROM PAUL: if we don't get an ack, then we doulbe the size of the window
                        // only if it is smaller than the maximum window size. Otherwise, 
                        // we set it to the maximum window size.
                        currentCW = currentCW*2;
                        pachyderm.resTransAttempts = pachyderm.resTransAttempts + 1;
                        if(currentCW >= aCWmax){
                            currentCW = aCWmax;
                        }
                        // FROM PAUL: we then try to resend. 
                        resend();
                    }
                }
            }

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
        wcerr << "This is not a retransmission" << endl;
        if (!theRF->inUse()) {
            wcerr << "The RF layer is not in use." << endl;
            waitIFS
            wcerr << "Waiting IFS" << endl;
            if (!theRF->inUse()) {  //Perfect transmittion
                wcerr << "The RF layer is still not in use yay!" << endl;
               if (theRF->transmit(frame, size) != size) {  //Makes the transmission
                wcerr << "Did not send correctly" << endl;
                    return 0; //Did not send all of frame or something failed internally
                } else {
                    wcerr << "sent correctly" << endl;
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
    int waitTime = rand() % intPow(2, cWindow);//Random number in range [0,2^aCWmin) FROM PAUL: forgot a ;
    while (true) {
        while (waitTime>0 && !theRF->inUse()) {     //We havent waited waitTime and 
                                                                            //No one else is transmitting
            usleep(1000);   //Sleep 1 milSec FROM PAUL: the s in usleep was capitalized, I fixed it.
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
    short tempy = 7;
    //FROM PAUL: The third argument to build frame was originally seqTable(pachyderm.destination)
    // I think you meant to call the getSeqNum function from your seqNumManager
    buildFrame(0, true, seqTable.getSeqNum(pachyderm.destination), 1111);
    char theFrame[pachyderm.frame_size];
    char* pointerToTheFrame = &theFrame[0];
    pachyderm.buildByteArray(pointerToTheFrame); //Fill theFrame
    pachyderm.resTransAttempts++;     //Increment the times we have tried to resend FROM PAUL: resTransAttempts, not reTransAttempts
    return send(pointerToTheFrame, pachyderm.frame_size, true, aCWmin + pachyderm.resTransAttempts); 
}














