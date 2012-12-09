#include "DemiBrad.h"

using namespace std;
DemiBrad theDemibrad;

// NONMEMBER FUNCTIONS

/*
 * creates a sender thread and calls its infinite loop method
 */
void *create_sender_thread(void *cnt){
	usleep(100);
	theDemibrad.RFLayer_demibrad->attachThread();
	Sender sendy(theDemibrad.RFLayer_demibrad, 
		&theDemibrad.send_Queue_demibrad, 
		&theDemibrad.ack_Received_demibrad, 
		theDemibrad.MACaddr_demibrad, 
		&theDemibrad.expected_sequence_number, 
		&theDemibrad.mutex_Demibrad_Sender, 
		&theDemibrad.statusCode, 
		&theDemibrad.cmdCode[0], 
		&theDemibrad.statusMutex, 
		&theDemibrad.mutex_Demibrad_ostream,
		&theDemibrad.mutex_Demibrad_fudge_factor, 
		&theDemibrad.fudge_factor_Demibrad, 
		theDemibrad.streamy_demibrad);
	sendy.MasterSend();
	wcerr << "This should not appear";
	return (void *)0;
}
 /*
  * create receiver thread and calls its infinite loop method
  */
void *create_and_run_receiver_thread(void *cnt){
	usleep(1000);
	theDemibrad.RFLayer_demibrad->attachThread();
	Listener listen(theDemibrad.RFLayer_demibrad, 
		&theDemibrad.receive_Queue_demibrad, 
		&theDemibrad.ack_Received_demibrad, 
		theDemibrad.MACaddr_demibrad, 
		&theDemibrad.mutex_Demibrad_Receiver, 
		&theDemibrad.expected_sequence_number, 
		&theDemibrad.statusMutex, 
		&theDemibrad.mutex_Demibrad_ostream, 
		&theDemibrad.mutex_Demibrad_fudge_factor, 
		&theDemibrad.fudge_factor_Demibrad, 
		&theDemibrad.cmdCode[0], 
		&theDemibrad.statusCode,
		theDemibrad.streamy_demibrad);
	listen.UltraListen();
	wcerr << "This should not appear again";
	return (void *)0;
}

DemiBrad::DemiBrad(short MACadr, ostream* stremy){
	RFLayer_demibrad = new RF();
	MACaddr_demibrad = MACadr;

	streamy_demibrad = stremy;
	
	fudge_factor_Demibrad = 0;
	pthread_mutexattr_init(&attr);
	pthread_mutex_init(&mutex_Demibrad_Sender, &attr);
	pthread_mutex_init(&mutex_Demibrad_Receiver, &attr);
	pthread_mutex_init(&mutex_Demibrad_ostream, &attr);
	pthread_mutex_init(&mutex_Demibrad_fudge_factor, &attr);
	pthread_mutex_init(&mutex_attach_rflayer, &attr);
	// create the threads
	pthread_t ids[3];     
    pthread_setconcurrency(5);
    int counts[8];
    // create the sender and receiver threads
	statusCode = 0; 
    cmdCode[0] = 0;
    cmdCode[1] = 0;
    cmdCode[2] = 0;
    cmdCode[3] = 30;
    pthread_mutex_init(&statusMutex, &attr);
    pthread_mutex_init(&mutex_Demibrad_ostream, &attr);
    pthread_mutex_init(&mutex_Demibrad_fudge_factor, &attr);
    fudge_factor_Demibrad = 5; // CHANGE THIS
    pthread_attr_t attrr;
    pthread_attr_init(&attrr);
    pthread_attr_setscope(&attrr, PTHREAD_SCOPE_SYSTEM);
    pthread_create(&(ids[0]), &attrr, create_sender_thread, &(counts[0]));
    
    pthread_create(&(ids[1]), &attrr, create_and_run_receiver_thread, &(counts[1]));
}

/*
 * intializes this layer 
 */
int dot11_init(short MACadr, ostream* stremy){
	DemiBrad temp(MACadr, stremy);
	theDemibrad = temp;
	return 1;
}

/*
 * recives data
 */
int dot11_recv(short *srcAddr, short *destAddr, char *buf, int bufSize){
	return theDemibrad.dot11_recv_DemiBrad(srcAddr, destAddr, buf, bufSize); //BRAD LOOK HERE: jump to line 96
}

/*
 * adjust the current command levels
 */
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
  * takes data from the layer above and hands it off to the sender thread
  */
int dot11_send(short destAddr, char *buf, int bufSize){
	return theDemibrad.dot11_send_DemiBrad(destAddr, buf, bufSize);
}
//MEMBER FUNCTIONS

 /*
  * sets the command levels that are used by the listener and sender threads
  */
int DemiBrad::dot11_command_DemiBrad(int cmd, int val){
	if (cmd > 3 || cmd < 0)
	{

		return -1;
	}else{
		cmdCode[cmd] = val;
		return 1;
	}
}
 /*
  * returns the most recent status/error code 
  */
int DemiBrad::status_DemiBrad(){
	return statusCode;
}
/*
 * receive data
 * from the incoming data queue that is filled by listner
 */
int DemiBrad::dot11_recv_DemiBrad(short *srcAddr, short *destAddr, char *buf, int bufSize){
	pthread_mutex_lock(&mutex_Demibrad_Receiver); // Lock the receiver's mutex so the receiver cannot access data while we do.
	if(!receive_Queue_demibrad.empty()){// check if the queue containing packets addressed to us is empty
		//if not
		Packet temp = receive_Queue_demibrad.front(); // make a temporary packet
		receive_Queue_demibrad.pop(); // pop of the front to a packet
		
		int size = temp.frame_size;
		int i = 0;
		while(i < temp.bytes_to_send){ // put the data in the buffer into the buffer that is being returned
			buf[i] = temp.physical_data_array[i];
			wcerr << buf[i] << " :: ";
			i++;
		}
		*destAddr = temp.destination;
		*srcAddr = temp.sender;
		wcerr << temp.sender << " is the sender address " << temp.destination << " is the destination address. " << endl;
		pthread_mutex_unlock(&mutex_Demibrad_Receiver);
		return temp.bytes_to_send;
	}else{
		//if it is empty, unlock the mutex and return -1
		pthread_mutex_unlock(&mutex_Demibrad_Receiver);
		return -1;
	}
}
 /*
  * sends data by putting it in a packet and putting that in a queue for sender
  */
int DemiBrad::dot11_send_DemiBrad(short destAddr, char *buf, int bufSize){
	Packet temp(destAddr,buf,bufSize); // make a temporary packet
	pthread_mutex_lock(&mutex_Demibrad_Sender);
	if (send_Queue_demibrad.size() > 4)
	{
		pthread_mutex_unlock(&mutex_Demibrad_Sender);
		return -1;
	}else{
		send_Queue_demibrad.push(temp);
		pthread_mutex_unlock(&mutex_Demibrad_Sender);
		return bufSize;
	}
}



