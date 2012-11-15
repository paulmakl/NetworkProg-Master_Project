/*
 *
 * This was my HW 3. I did well on it so
 * we should be able to use some of it 
 * for our project.
 *
 */

#include <iostream>
#include <stdlib.h>
#include <arpa/inet.h>
#include "RF.h"
#include <pthread.h> 
#include <time.h>

            
int counts[2];
int mac = 77;
RF* RFLayer = new RF();

using namespace std;

//Sends packets
void *send(void *cnt)
{
    //attach the thread
    RFLayer->attachThread();
    //create the buffer
    //  0 :: 0 :: 1 :: 44 :: 3 :: -23 :: 97 :: 98 :: 99 :: 22 :: -94 :: 54 :: -27 :: 
    char buf[13];
    buf[0] = 0;
    buf[1] = 0;
    buf[2] = 1;
    buf[3] = 44;
    buf[4] = 3;
    buf[5] = -23;
    buf[6] = 97;
    buf[7] = 98;
    buf[8] = 99;
    buf[9] = 22;
    buf[10] = -94;
    buf[11] = 54;
    buf[12] = -27;
    //make variables for the number of bytes sent
    //a time checkpoint
    // and a temperary short to manipulate the mac address
    int bytesSent;
    long long checkpointTime = RFLayer->clock();
    unsigned short temp = mac;
    //create a temp value for the upcoming while loop
    // so it can manipulate and store clock values.
    unsigned long long tempNEW;
    //Create the first random wait time
    srand ( time(NULL) );
    //generate a number between 0 and 7
    float waitTimeInt = rand() % 7;
    //generate a fraction to make the wait time more exact
    // than with only an integer. A random numerator and denominator
    // ar created to add the the previous integer which will be the new random
    // number. This is sort of a homebrewed float random number generator. Time is in seconds
    float waitTimeDenominator = (rand() % 11) + 1.0;
    float waitTimeNumerator = (rand() % (int)waitTimeDenominator) + 1.0;
    float tempFraction = waitTimeNumerator / waitTimeDenominator;
    float waitTime = waitTime + tempFraction;
    while(true){
        //If the clock - the last checkpoint time is greater than the wait time, then transmit.
        if ( (RFLayer->clock() - checkpointTime) > waitTime * 3600.0)
        {
            //create a new random wait time
            waitTimeInt = rand() % 7;
            waitTimeDenominator = (rand() % 11) + 1;
            waitTimeNumerator = (rand() % (int)waitTimeDenominator) + 1.0;
            tempFraction = waitTimeNumerator / waitTimeDenominator;
            waitTime = waitTimeInt + tempFraction;
            //send the packet
            bytesSent = RFLayer->transmit(buf, 13);
            //check for errors
            if (bytesSent != 13)
                wcerr << "Only sent " << bytesSent << " bytes of data!" << endl;
            else{
                wcerr << "Yay!  We sent the entire packet!     " << mac << "     ";
                int q = 0;
                while(q < 13){
                    wcerr << buf[q] + 0 << " :: ";
                    q++;
                }
                wcerr << "" << endl;
            }
            //set the checkpoint time to the rf layer clock time
            checkpointTime = RFLayer->clock();
        }
    }
    return (void *)0;
}
//Receives packets
void *getPackets(void *cnt){
    //attach thread
    RFLayer->attachThread();
    //Create a buffer
    char buf[13];
    //create a bytes received integer
    int bytesReceived;
    while(true){
            // get the bytes received
        bytesReceived = RFLayer->receive(buf, 13);
            //print the bytes received and checks for errors
        if (bytesReceived != 13)
            wcerr << "Only Received " << bytesReceived << " bytes of data!" << endl;
        else{
            wcerr << bytesReceived << " bytes Received!     ";
            int q = 0;
            while(q < 13){
                wcerr << buf[q] + 0 << " :: ";
                q++;
            }
            wcerr << "" << endl;
        }
    }
    return (void *)0;
}

int main(int argc, char const *argv[])
{
    // Set up the threads and input the mac address
    mac = atoi(argv[1]);
    if(mac == 0){
        srand ( time(NULL) );
        mac = rand() % 30000;
        cerr << "Random Mac Address Assigned As " << mac;
    }

    pthread_t ids[2];
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);  
    pthread_setconcurrency(2);
    RFLayer->attachThread();

    //Create the Threads
    pthread_create(&(ids[0]), &attr, getPackets, &(counts[0]));
    pthread_create(&(ids[1]), &attr, send, &(counts[1]));
    
    //Wait for the threads to finish
    for (int i=0; i<2; i++){
        pthread_join(ids[i], NULL);
    }


    return 0;
}
/*
g++ RF_Test.cpp RF.cpp -o bcast -I/System/Library/Frameworks/JavaVM.framework/Headers -L/System/Library/Frameworks/JavaVM.framework/Libraries -ljvm  -framework JavaVM
*/
