#include <iostream>
#include <pthread.h>
#include "DemiBrad.h"


using namespace std;

short MACaddr_demibrad; // users mac address
ostream* streamy_demibrad; // provided ostream
bool ack_Received_demibrad; // flag for acknowledgment received
short MACACK_demibrad; // the address that is associated with the most recent Acknowledgement
//incoming_Queue queue <short, char, int> // a queue for incoming data
//outgoing_Queue queue <short, char, int> // a queue for outgoing data
unsigned short test; 
RF* RFLayer_demibrad;
CircularFifo<Packet*, 10> send_Queue_demibrad;
CircularFifo<Packet*, 10> receive_Queue_demibrad;
Packet memory_buffer_demibrad[500];
int memory_buffer_number_count_demibrad;

void *create_sender_thread(void *cnt){
	RFLayer_demibrad->attachThread();
	wcerr << "sender thread";
	Sender sendy(RFLayer_demibrad, &send_Queue_demibrad, &test, &ack_Received_demibrad, MACACK_demibrad);
	return (void *)0;
}
void *create_and_run_receiver_thread(void *cnt){
	RFLayer_demibrad->attachThread();
	wcerr << "receiver thread";
	bool hello = true;
	Listener listen(RFLayer_demibrad, &receive_Queue_demibrad, &test, &ack_Received_demibrad, MACaddr_demibrad);
	listen.UltraListen();
	return (void *)0;
}

int DemiBrad::dot11_init(short MACadr, ostream* stremy){
	memory_buffer_number_count_demibrad = 0;
	MACaddr_demibrad = MACadr;
	streamy_demibrad = stremy;
	RFLayer_demibrad = new RF();
	test = 803;
	pthread_t ids[3];
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);  
    pthread_setconcurrency(3);
    int counts[3];
    RFLayer_demibrad->attachThread();
    pthread_create(&(ids[0]), &attr, create_sender_thread, &(counts[0]));
    pthread_create(&(ids[1]), &attr, create_and_run_receiver_thread, &(counts[1]));
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
	receive_Queue_demibrad.pop(temp_pointer);
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
	Packet temp;
	memory_buffer_demibrad[memory_buffer_number_count_demibrad] = temp;
	memory_buffer_demibrad[memory_buffer_number_count_demibrad].initPacket(0,false,0,destAddr,MACaddr_demibrad,buf,0,bufSize);
	Packet * temp_pointer = &memory_buffer_demibrad[memory_buffer_number_count_demibrad];
	send_Queue_demibrad.push(temp_pointer);
	if (memory_buffer_number_count_demibrad > 499)
	{
		memory_buffer_number_count_demibrad = 0;
	}
	else{
		memory_buffer_number_count_demibrad++;
	}
	return 1;
}



