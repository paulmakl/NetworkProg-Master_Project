#include "DemiBrad.h"

using namespace std;
DemiBrad x;
short MACaddr_demibrad; // users mac address
ostream* streamy_demibrad; // provided ostream
bool ack_Received_demibrad; // flag for acknowledgment received
unsigned short MACACK_demibrad; // the address that is associated with the next Acknowledgement. Is zero if none need to be sent
//unsigned short send_flag_demibrad; // flat that lets the sender know when to send
RF* RFLayer_demibrad; // the RF layer associated with Demibrad
queue<Packet> send_Queue_demibrad; // the queue of packets to send
queue<Packet> receive_Queue_demibrad; // the queu of packets received from the receiver class
unsigned short MACACK_sequence_number;
unsigned short MACACK_expected_sequence_number;
pthread_mutex_t mutex_Demibrad_Receiver = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_Demibrad_Sender = PTHREAD_MUTEX_INITIALIZER;

/*
 * creates a sender thread
 */
void *create_sender_thread(void *cnt){
	RFLayer_demibrad->attachThread();
	//wcerr << "sender thread";

	Sender sendy(RFLayer_demibrad, &send_Queue_demibrad, &MACACK_demibrad, &ack_Received_demibrad, MACaddr_demibrad, &mutex_Demibrad_Sender);
	wcerr << &send_Queue_demibrad << endl;
	sendy.MasterSend();
	wcerr << "This should not appear";
	return (void *)0;
}
 /*
  * create receiver thread
  */
void *create_and_run_receiver_thread(void *cnt){
	RFLayer_demibrad->attachThread();
	//wcerr << "receiver thread";
	bool hello = true;
	Listener listen(RFLayer_demibrad, &receive_Queue_demibrad, &MACACK_demibrad, &ack_Received_demibrad, 103, &mutex_Demibrad_Receiver);
	listen.UltraListen();
	wcerr << "This should not appear again";
	return (void *)0;
}

 /*
  * initialize the demibrad class
  */
int dot11_init(short MACadr, ostream* stremy){
	MACaddr_demibrad = MACadr;
	streamy_demibrad = stremy;
	RFLayer_demibrad = new RF();
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
	Packet temp = receive_Queue_demibrad.front(); // temporary packet
	receive_Queue_demibrad.pop(); // pop of the first pointer to a packet
	int i = 0;
	while(i < bufSize){ // put the data in the buffer into the buffer that is beoing returned
		buf[i] = temp.physical_data_array[i];
		i++;
	}
	return bufSize;
}
 /*
  * send data
  */
int DemiBrad::dot11_send(short destAddr, char *buf, int bufSize){
	Packet temp; // make a temporary packet
	//memory_buffer_demibrad[memory_buffer_number_count_demibrad] = temp; //put the temporary packet in the memory buffer
	temp.initpacket(destAddr,buf,bufSize); // initialize the packet with the information to be sent
	send_Queue_demibrad.push(temp);
	return 1;
}



