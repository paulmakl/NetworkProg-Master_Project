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
CircularFifo<Packet*, 10> send_Queue;
CircularFifo<Packet*, 10> receive_Queue;

void *create_sender_thread(void *cnt){
	RFLayer->attachThread();
	wcerr << "sender thread";
	int i = 0;
	while(true){
		i++;
	}
	return (void *)0;
}
void *create_Receiver_Thread(void *cnt){
	RFLayer->attachThread();
	wcerr << "receiver thread";
	int i = 0;
	while(true){
		i++;
	}
	return (void *)0;
}

int DemiBrad::dot11_init(short MACadr, ostream* stremy){
	MACaddr = MACadr;
	streamy = stremy;
	RFLayer = new RF();

	pthread_t ids[3];
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);  
    pthread_setconcurrency(3);
    int counts[3];
    RFLayer->attachThread();
    pthread_create(&(ids[0]), &attr, create_sender_thread, &(counts[0]));
    pthread_create(&(ids[1]), &attr, create_Receiver_Thread, &(counts[1]));
    //pthread_join(ids[0], NULL);
    //pthread_join(ids[1], NULL);
    return 1;
}
int DemiBrad::dot11_command(int cmd, int val){
	return 1;
}
int DemiBrad::status(){
	return 0;
}
int DemiBrad::dot11_recv(short *srcAddr, short *destAddr, char *buf, int bufSize){
	Packet * temp_pointer;
	Packet temp;
	receive_Queue.pop(temp_pointer);
	temp = *temp_pointer;
	*srcAddr = temp.sender;
	*destAddr = temp.destination;
	int i = 0;
	while(i < bufSize){
		buf[i] = temp.physical_data_array[i];
	}
	return bufSize;
}
int DemiBrad::dot11_send(short destAddr, char *buf, int bufSize){
	//Packet temp;
	//temp.initPacket(0,false,0,destAddr,MACaddr,*buf,0,bufSize);
	return 1;
}



