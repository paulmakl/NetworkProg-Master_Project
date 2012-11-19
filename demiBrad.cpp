#include "DemiBrad.h"

using namespace std;
DemiBrad theDemibrad;

// NONMEMBER FUNCTIONS

/*
 * creates a sender thread
 */
void *create_sender_thread(void *cnt){
	theDemibrad.RFLayer_demibrad->attachThread();
	//wcerr << "sender thread";

	Sender sendy(theDemibrad.RFLayer_demibrad, &theDemibrad.receive_Queue_demibrad, &theDemibrad.ack_Received_demibrad, 103, &theDemibrad.expected_sequence_number, &theDemibrad.mutex_Demibrad_Receiver);
	sendy.MasterSend();
	wcerr << "This should not appear";
	return (void *)0;
}
 /*
  * create receiver thread
  */
void *create_and_run_receiver_thread(void *cnt){
	theDemibrad.RFLayer_demibrad->attachThread();
	//wcerr << "receiver thread";
	Listener listen(theDemibrad.RFLayer_demibrad, &theDemibrad.receive_Queue_demibrad, &theDemibrad.ack_Received_demibrad, 103, &theDemibrad.mutex_Demibrad_Receiver, &theDemibrad.expected_sequence_number);
	listen.UltraListen();
	wcerr << "This should not appear again";
	return (void *)0;
}

DemiBrad::DemiBrad(short MACadr, ostream* stremy){
	MACaddr_demibrad = MACadr;
	streamy_demibrad = stremy;
	RFLayer_demibrad = new RF();
	pthread_mutexattr_init(&attr);
	pthread_mutex_init(&mutex_Demibrad_Sender, &attr);
	pthread_mutex_init(&mutex_Demibrad_Receiver, &attr);
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
}

int dot11_init(short MACadr, ostream* stremy){
	DemiBrad temp(MACadr, stremy);
	theDemibrad = temp;
	return 1;
}


int dot11_recv(short *srcAddr, short *destAddr, char *buf, int bufSize){
	return theDemibrad.dot11_recv_DemiBrad(srcAddr, destAddr, buf, bufSize);
}

int dot11_command(int cmd, int val){
	return theDemibrad.dot11_command_DemiBrad(cmd,val);
}
 /*
  * unimplemented
  */
int status(){
	return theDemibrad.status_DemiBrad();
}

 /*
  * send data
  */
int dot11_send(short destAddr, char *buf, int bufSize){
	return theDemibrad.dot11_send_DemiBrad(destAddr, buf, bufSize);
}
//MEMBER FUNCTIONS

 /*
  * unimplemented
  */
int DemiBrad::dot11_command_DemiBrad(int cmd, int val){
	return 1;
}
 /*
  * unimplemented
  */
int DemiBrad::status_DemiBrad(){
	return 0;
}
/*
 * receive data
 */
int DemiBrad::dot11_recv_DemiBrad(short *srcAddr, short *destAddr, char *buf, int bufSize){
	Packet temp = receive_Queue_demibrad.front(); // temporary packet
	receive_Queue_demibrad.pop(); // pop of the first pointer to a packet
	char tempBuf[bufSize];
	char * tempBufP = &tempBuf[0];
	temp.buildByteArray(tempBufP);
	int i = 0;
	while(i < bufSize){ // put the data in the buffer into the buffer that is beoing returned
		buf[i] = tempBuf[i + bufSize];
		i++;
	}
	return bufSize;
}
 /*
  * send data
  */
int DemiBrad::dot11_send_DemiBrad(short destAddr, char *buf, int bufSize){
	Packet temp(destAddr,buf,bufSize); // make a temporary packet
	//memory_buffer_demibrad[memory_buffer_number_count_demibrad] = temp; //put the temporary packet in the memory buffer
	send_Queue_demibrad.push(temp);
	return 1;
}



