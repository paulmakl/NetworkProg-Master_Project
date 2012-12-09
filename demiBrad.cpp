
// includes
#include "DemiBrad.h"
using namespace std;
DemiBrad theDemibrad;

/*
 * creates a sender thread that will send packets
 * on the queue send_Queue_demibrad
 */
void *create_sender_thread(void *cnt){
	usleep(100); // allow time for demibrad's constructor to finish
	theDemibrad.RFLayer_demibrad->attachThread(); // attach this thread to the RF layer
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
		theDemibrad.streamy_demibrad); // init sender
	sendy.MasterSend(); // have sender send
	return (void *)0;
}
 /*
  * create receiver thread that puts incoming packets on
  * the queue receive_Queue_demibrad
  */
void *create_and_run_receiver_thread(void *cnt){
	usleep(1000); // wait for demibrad and sender to finish initing
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
	RFLayer_demibrad = new RF(); // attach RF layer
	streamy_demibrad = stremy;
	//*theDemibrad.streamy_demibrad << "RF layer created!" << endl;
	statusCode = 1;
	
	fudge_factor_Demibrad = 0;
	if (MACadr < 101 || MACadr > 1800) // check for improper mac address
	{
		pthread_mutex_lock(&statusMutex);
		statusCode = 8;
		pthread_mutex_unlock(&statusMutex);
		MACaddr_demibrad = 723; // set to 723 if mac address provided is bad
	}else{
		MACaddr_demibrad = MACadr;
	}
	// set up the mutexs
	pthread_mutexattr_init(&attr);
	pthread_mutex_init(&mutex_Demibrad_Sender, &attr);
	pthread_mutex_init(&mutex_Demibrad_Receiver, &attr);
	pthread_mutex_init(&mutex_Demibrad_ostream, &attr);
	pthread_mutex_init(&mutex_Demibrad_fudge_factor, &attr);
	pthread_mutex_init(&mutex_attach_rflayer, &attr);
	// create the threads
	pthread_t ids[3];     
    pthread_setconcurrency(5);
    // initialize status codes
    int counts[8];
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
    // create the sender and receiver threads
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
		pthread_mutex_lock(&statusMutex);
		statusCode = 9;
		pthread_mutex_unlock(&statusMutex);
		return 9;
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
 * receive data from the RF layer, and return the oldest piece of data.
 * 
 */
int DemiBrad::dot11_recv_DemiBrad(short *srcAddr, short *destAddr, char *buf, int bufSize){
	if (srcAddr == NULL || destAddr == NULL || buf == NULL) // check for null pointers
	{
		pthread_mutex_lock(&statusMutex);
		statusCode = 7;
		pthread_mutex_unlock(&statusMutex);
		return 7;
	}
	if (bufSize < 0) // check if the bufsize is negative
	{
		pthread_mutex_lock(&statusMutex);
		statusCode = 6;
		pthread_mutex_unlock(&statusMutex);
		return 6;
	}
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
	if (buf == NULL) // check if the buffer is null
	{
		pthread_mutex_lock(&statusMutex);
		statusCode = 7;
		pthread_mutex_unlock(&statusMutex);
		return 7;
	}
	if (bufSize < 0) // check if the size is negative
	{
		pthread_mutex_lock(&statusMutex);
		statusCode = 6;
		pthread_mutex_unlock(&statusMutex);
		return 6;
	}
	if (destAddr < 101 || destAddr > 1800) // check for proper mac address
	{
		pthread_mutex_lock(&statusMutex);
		statusCode = 8;
		pthread_mutex_unlock(&statusMutex);
		return 8;
	}
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



