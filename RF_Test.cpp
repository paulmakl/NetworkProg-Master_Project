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
    char buf[10];

    //make variables for the number of bytes sent
    //a time checkpoint
    // and a temperary short to manipulate the mac address
    int bytesSent;
    long long checkpointTime = RFLayer->clock();
    unsigned short temp = mac;
    //store mac address in buffer
    buf[0] = mac >> 8;
    temp = mac << 8;
    buf[1] = temp >> 8;
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
            //create a packet to be sent
            for (int i = 0; i < 8; ++i)
            {
                tempNEW = RFLayer->clock(); //start;
                tempNEW = tempNEW << 56 - (8*i);
                tempNEW = tempNEW >> 56;
                buf[9-i] = tempNEW;
            }
            //create a new random wait time
            waitTimeInt = rand() % 7;
            waitTimeDenominator = (rand() % 11) + 1;
            waitTimeNumerator = (rand() % (int)waitTimeDenominator) + 1.0;
            tempFraction = waitTimeNumerator / waitTimeDenominator;
            waitTime = waitTimeInt + tempFraction;
            //send the packet
            bytesSent = RFLayer->transmit(buf, 10);
            //check for errors
            if (bytesSent != 10)
                wcerr << "Only sent " << bytesSent << " bytes of data!" << endl;
            else{
                wcerr << "Yay!  We sent the entire packet!     " << mac << "     " << RFLayer->clock() << endl;
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
    char buf[10];
    //create a bytes received integer
    int bytesReceived;
    while(true){
            // get the bytes received
        bytesReceived = RFLayer->receive(buf, 10);
            //print the bytes received and checks for errors
        if (bytesReceived != 10)
            wcerr << "Only Received " << bytesReceived << " bytes of data!" << endl;
        else{
            wcerr << bytesReceived << " bytes Received!     ";
        }
        //start printing out the buffer
        int x = 0;
        wcerr << "[ ";
        while(x<10){
            unsigned char temp = buf[x];
            wcerr << temp << ", ";
            x++;
        }
        wcerr << " ]" << endl;
        
        //add all the bytes together to exctract the time from the packet received
        unsigned long long receivedTime = 0;
        unsigned long long temp;
        unsigned char charTemp;
        for (int i = 0; i < 8; ++i)
        {
             temp = buf[i+2] - 0;
             //temp = charTemp;
             temp = temp << 56 - (8 * i);
             receivedTime = receivedTime + temp;
        }

        //extract the host name
        unsigned short hostName;
        unsigned short tempShort;
        hostName = buf[1];
        tempShort = buf[0] << 8;
        hostName = hostName + tempShort;
        //print host name and packet time sent stuff, time for bed
        wcerr << "Host " << hostName << " says the time is " << receivedTime << endl;   
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
