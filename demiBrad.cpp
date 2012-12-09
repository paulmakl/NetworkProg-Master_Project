#include "DemiBrad.h"

using namespace std;
DemiBrad theDemibrad;

// NONMEMBER FUNCTIONS

/*
 * creates a sender thread
 */
void *create_sender_thread(void *cnt){
	//wcerr << "S" << endl;
	//pthread_mutex_lock(&theDemibrad.mutex_attach_rflayer);
	//wcerr << "LISTENER HAS ACCESS TO THE RF LAYER." << endl;
	usleep(100);
	//wcerr << "S" << endl;
	theDemibrad.RFLayer_demibrad->attachThread();
	//wcerr << "T" << endl;
	//pthread_mutex_unlock(&theDemibrad.mutex_attach_rflayer);
	//wcerr << "T" << endl; rawr
	//wcerr << "LISTENER HAS LET GO OF THE RF LAYER." << endl;
	//wcerr << "sender thread";
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
		&theDemibrad.mtxDemibradFdgFctr, 
		&theDemibrad.fdgFctrDemibrad, 
		theDemibrad.streamy_demibrad);
	//wcerr << "D" << endl;
	sendy.MasterSend();
	wcerr << "This should not appear";
	return (void *)0;
}
 /*
  * create receiver thread
  */
void *create_and_run_receiver_thread(void *cnt){
	//wcerr << "L" << endl;
	//pthread_mutex_lock(&theDemibrad.mutex_attach_rflayer);
	usleep(1000);
	//wcerr << "L" << endl;
	theDemibrad.RFLayer_demibrad->attachThread();
	//wcerr << "M" << endl;
	//pthread_mutex_unlock(&theDemibrad.mutex_attach_rflayer);
	//wcerr << "M" << endl;
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
		&theDemibrad.status);
	//wcerr << "E" << endl;
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
    //RFLayer_demibrad->attachThread();
    // create the sender and receiver threads
	statusCode = 0; 
    cmdCode[0] = 0;
    cmdCode[1] = 0;
    cmdCode[2] = 0;
    cmdCode[3] = 30;
    pthread_mutex_init(&statusMutex, &attr);
    pthread_mutex_init(&mutex_Demibrad_ostream, &attr);
    pthread_mutex_init(&mtxDemibradFdgFctr, &attr);
    fdgFctrDemibrad = 5; // CHANGE THIS
    pthread_attr_t attrr;
    pthread_attr_init(&attrr);
    pthread_attr_setscope(&attrr, PTHREAD_SCOPE_SYSTEM);
    pthread_create(&(ids[0]), &attrr, create_sender_thread, &(counts[0]));
    
    pthread_create(&(ids[1]), &attrr, create_and_run_receiver_thread, &(counts[1]));
    //wcerr << "D" << endl;
}

int dot11_init(short MACadr, ostream* stremy){
	DemiBrad temp(MACadr, stremy);
	theDemibrad = temp;
	return 1;
}


int dot11_recv(short *srcAddr, short *destAddr, char *buf, int bufSize){
	return theDemibrad.dot11_recv_DemiBrad(srcAddr, destAddr, buf, bufSize); //BRAD LOOK HERE: jump to line 96
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
	if (cmd > 3 || cmd < 0)
	{
		return -1;
	}else{
		cmdCode[cmd] = val;
		return 1;
	}
}
 /*
  * unimplemented
  */
int DemiBrad::status_DemiBrad(){
	return statusCode;
}
/*
 * receive data
 * BRAD LOOK HERE: This is the function that receives data! 
 */
int DemiBrad::dot11_recv_DemiBrad(short *srcAddr, short *destAddr, char *buf, int bufSize){
	pthread_mutex_lock(&mutex_Demibrad_Receiver); // Lock the receiver's mutex so the receiver cannot access data while we do.
	if(!receive_Queue_demibrad.empty()){// check if the queue containing packets addressed to us is empty
		//if not
		Packet temp = receive_Queue_demibrad.front(); // make a temporary packet
		receive_Queue_demibrad.pop(); // pop of the front to a packet
		
		int size = temp.frame_size;
		//wcerr << "************ destination Address:" << size << endl;
		//char * tempBufP = &tempBuf[0];
		//wcerr << "Entering troubled function ..." << endl;
		//wcerr << "Exiting troubled function ..." << endl; // BRAD LOOK HERE
		int i = 0;
		//char tempBuf[temp.frame_size]; // BRAD LOOK HERE: I have created a temporary buffer
		while(i < temp.bytes_to_send){ // put the data in the buffer into the buffer that is being returned
			buf[i] = temp.physical_data_array[i];
			wcerr << buf[i] << " :: ";
			i++;
		}
		*destAddr = temp.destination;
		*srcAddr = temp.sender;
		pthread_mutex_unlock(&mutex_Demibrad_Receiver);
		//wcerr << endl << "Bytes Sent :: " << temp.bytes_to_send << endl;
		return temp.bytes_to_send;
	}else{
		//if it is empty, unlock the mutex and return -1
		pthread_mutex_unlock(&mutex_Demibrad_Receiver);
		return -1;
	}
}
 /*
  * send data
  */
int DemiBrad::dot11_send_DemiBrad(short destAddr, char *buf, int bufSize){
	//char test[] = "RAWR MY NAME IS PAUL RAWR RAWR\n";
	//char * pointer = &test[0];
	//theDemibrad.*streamy_demibrad.write(pointer, 31);
	Packet temp(destAddr,buf,bufSize); // make a temporary packet
	//memory_buffer_demibrad[memory_buffer_number_count_demibrad] = temp; //put the temporary packet in the memory buffer
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



