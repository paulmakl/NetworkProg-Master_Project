#include <iostream>
#include <pthread.h>
#include "DemiBrad.h"
#include "RF.h"
#include "circularfifo.h"
#include "packet.h"

using namespace std;

short MACaddr; // users mac address
ostream* streamy; // provided ostream
bool ack_Received; // flag for acknowledgment received
short MACACK; // the address that is associated with the most recent Acknowledgement
//incoming_Queue queue <short, char, int> // a queue for incoming data
//outgoing_Queue queue <short, char, int> // a queue for outgoing data 
RF* RFLayer;
CircularFifo<int, 2> send_Queue;
CircularFifo<int, 2> receive_Queue;

int DemiBrad::dot11_init(short MACadr, ostream* stremy){
	MACaddr = MACadr;
	streamy = stremy;
	RFLayer = new RF();
	pthread_t ids[2];
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);  
    pthread_setconcurrency(2);
    //RFLayer->attachThread();

    //Create the Threads
    /*pthread_create(&(ids[0]), &attr, getPackets, &(counts[0]));
    pthread_create(&(ids[1]), &attr, send, &(counts[1]));

    for (int i=0; i<2; i++){
    	pthread_join(ids[i], NULL);
    }*/
	//create_Receiver_Thread();
	//create_sender_thread();
}
int DemiBrad::dot11_command(int cmd, int val){
	return 1;
}
int DemiBrad::status(){
	return 0;
}
int DemiBrad::dot11_recv(short *srcAddr, short *destAddr, char *buf, int bufSize){
	return 1;
}
int DemiBrad::dot11_send(short destAddr, char *buf, int bufSize){
	return 1;
}
int DemiBrad::create_sender_thread(bool * ack_){
	return 1;
}
int DemiBrad::create_Receiver_Thread(){
	return 1;
}


