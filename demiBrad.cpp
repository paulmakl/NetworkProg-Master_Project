#include <iostream>
#include <pthread.h>
#include "DemiBrad.h"


using namespace std;

short MACaddr_demibrad; // users mac address
ostream* streamy_demibrad; // provided ostream
bool ack_Received_demibrad; // flag for acknowledgment received
short MACACK_demibrad; // the address that is associated with the most recent Acknowledgement
//unsigned short send_flag_demibrad; // flat that lets the sender know when to send
RF* RFLayer_demibrad; // the RF layer associated with Demibrad
CircularFifo<Packet*, 10> send_Queue_demibrad; // the queue of packets to send
CircularFifo<Packet*, 10> receive_Queue_demibrad; // the queu of packets received from the receiver class
Packet memory_buffer_demibrad[500]; // used to hold packets until they are used up
int memory_buffer_number_count_demibrad; // current position of the memory buffer


/*
 * creates a sender thread
 */
void *create_sender_thread(void *cnt){
	RFLayer_demibrad->attachThread();
	wcerr << "sender thread";

	Sender sendy(RFLayer_demibrad, &send_Queue_demibrad, &MACACK_demibrad, &ack_Received_demibrad, MACACK_demibrad);
	return (void *)0;
}
 /*
  * create receiver thread
  */
void *create_and_run_receiver_thread(void *cnt){
	RFLayer_demibrad->attachThread();
	wcerr << "receiver thread";
	bool hello = true;
	Listener listen(RFLayer_demibrad, &receive_Queue_demibrad, &test, &ack_Received_demibrad, MACaddr_demibrad);
	listen.UltraListen();
	return (void *)0;
}

 /*
  * initialize the demibrad class
  */
int DemiBrad::dot11_init(short MACadr, ostream* stremy){
	memory_buffer_number_count_demibrad = 0;
	MACaddr_demibrad = MACadr;
	streamy_demibrad = stremy;
	RFLayer_demibrad = new RF();
	test = 803;
	// create the threads
	pthread_t ids[3];
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);  
    pthread_setconcurrency(3);
    int counts[3];
    RFLayer_demibrad->attachThread();
    // create the sender and receiver threads
    pthread_create(&(ids[0]), &attr, create_sender_thread, &(counts[0]));
    pthread_create(&(ids[1]), &attr, create_and_run_receiver_thread, &(counts[1]));
    return 1;
}
 /*
  * unimplemented
  */
int DemiBrad::dot11_command(int cmd, int val){
	return 1;
}
 /*
  * unimplemented
  */
int DemiBrad::status(){
	return 0;
}
/*
 * receive data
 */
int DemiBrad::dot11_recv(short *srcAddr, short *destAddr, char *buf, int bufSize){
	Packet * temp_pointer; // temporary pointer to a packet
	Packet temp; // temporary packet
	receive_Queue_demibrad.pop(temp_pointer); // pop of the first pointer to a packet
	temp = *temp_pointer; // fill the temporary packet with the packet from the pointer
	*srcAddr = temp.sender; // access the senders mac address and put it in the returned source address
	*destAddr = temp.destination; // take the destination address out and put it in the destination address
	int i = 0;
	while(i < bufSize){ // put the data in the buffer into the buffer that is beoing returned
		buf[i] = temp.physical_data_array[i];
	}
	return bufSize;
}
 /*
  * send data
  */
int DemiBrad::dot11_send(short destAddr, char *buf, int bufSize){
	Packet temp; // make a temporary packet
	memory_buffer_demibrad[memory_buffer_number_count_demibrad] = temp; //put the temporary packet in the memory buffer
	memory_buffer_demibrad[memory_buffer_number_count_demibrad].initPacket(0,false,0,destAddr,MACaddr_demibrad,buf,0,bufSize); // initialize the packet with the information to be sent
	Packet * temp_pointer = &memory_buffer_demibrad[memory_buffer_number_count_demibrad]; // create a temporary pointer to the packet
	send_Queue_demibrad.push(temp_pointer); //push the temporary pointer onto the queue
	send_flag_demibrad = true; // flags that something should be sent
	// potentially reset the memory buffer number
	if (memory_buffer_number_count_demibrad > 499)
	{
		memory_buffer_number_count_demibrad = 0;
	}
	else{
		memory_buffer_number_count_demibrad++;
	}
	return 1;
}



