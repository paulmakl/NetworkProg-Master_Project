/**
 * Weston Thornburg
 * University of Puget Sound CS 325
 * October 2012
 */
#include "listener.h"

/*Listener::Listener(RF* RFLayer, queue<Packet>* incomingQueue, bool* receivedFlag, short myMAC, pthread_mutex_t * mutexListenr, short* exSN)
{
    daRF = RFLayer;//our reference to the RF layer
    ackReceivedL = receivedFlag;//flag for telling the sending an ACK has come in
    ackReceivedL = false;// indicates no ACKs recived 
    MACaddrList = myMAC;// our current mac address
    daLoopLine = incomingQueue;//where incoming data will be sent via pointers to tuples
    mutexListener = mutexListenr;//the mutex for locking the queue
    prints = true;//if true print statments are on
    expectedSN = exSN;//flag for determining the validity of incoming ACKS
    seqNumMang(MAXSEQNUM);//construct a SeqNumManager with roll over point of the maxium size 
}*/

int
Listener::read_Packet ()
{
    int status;//will be returned with different status code to help ultra listen react
    short packetDest = buf[2];//bitwise terribleness
    unsigned short temp_dest = packetDest;
    temp_dest = temp_dest << 8;
    unsigned short temp = buf[3];
    temp = temp << 8;
    temp = temp >> 8;
    //wcerr << temp << endl;
    packetDest = temp_dest + temp;
    unsigned char frameType = buf[0];

    frameType = frameType >> 5;
    switch (frameType)//compare the frame type of the packet given to known types to figure out what kind of packet it is
    {
        case 0:
            if (prints) wcerr << "Data packet received." << endl;
            //if (commands[0] == 1) streamy << "Data packet received." << endl;
            if (packetDest != MACaddrList && packetDest != -1)//compare the destination of this packet to our MAC address and to the broadcast address
            {
                status = 0;//this packet isn't for us
                if (prints) wcerr << "Packet not addressed to current MAC address. Our Mac address: " << MACaddrList << ".  Destination of incoming packet: " << packetDest << "MAC ADDRESS: " << endl;
                //if (commands[0] == 1) streamy << "Packet not addressed to current MAC address. Our Mac address: " << MACaddrList << ".  Destination of incoming packet: " << packetDest << endl;
                return status;
            }
            status = 1;
            break;

        case 1:
            if (prints) wcerr << "ACK Received." << endl;
            //if (commands[0] == 1) streamy << "ACK Received." << endl;
            if (packetDest != MACaddrList)//compare the destination of this packet to our MAC address and to the broadcast address
            {
                status = 0;//this packet isn't for us
                if (prints) wcerr << "Packet not addressed to current MAC address. Our Mac address: " << MACaddrList << ".  Destination of incoming packet: " << packetDest << endl;
                //if (commands[0] == 1) streamy << "Packet not addressed to current MAC address. Our Mac address: " << MACaddrList << ".  Destination of incoming packet: " << packetDest << endl;
                return status;
            }
            status = 2;
            break;

        case 2:
            if (prints) wcerr << "Beacon Received." << endl;
            //if (commands[0] == 1) streamy << "Beacon Received." << endl;
            status = 3;
            break;

        default:
            if (prints) wcerr << "Unknown packet type received." << endl;
            //if (commands[0] == 1) streamy << "Unknown packet type received." << endl;
            status = 4;
            break;
    }
    return status;
}

int
Listener::UltraListen()
{
    while(true){
        int PRR;//Packet Read Result
        // wait for data
        bytesReceived = daRF->receive(buf, MAXPACKETSIZE);//block until data comes our way 
        if (prints) wcerr << "FROM: " << extractSourceAddress() << "..." << endl;
        //if (prints) wcerr << "Sequence Number in Packet :: " << extractSequenceNumber() << " :: " << endl;
        //if (prints) wcerr << "Sequence Number in SEQNUMMANG :: " << seqNumMang.getSeqNum(extractSequenceNumber()) + 1 << " :: " << endl;

            //print the bytes received and checks for errors
        if (bytesReceived != MAXPACKETSIZE){
            if (prints) wcerr << "Received  partial Packet with " << bytesReceived << " bytes of data!" << endl;
            //if (commands[0] == 1) streamy << "Received  partial Packet with " << bytesReceived << " bytes of data!" << endl;
        }
        else{
            if (prints) wcerr << bytesReceived << " !Full Packet Received! woo! ";
            //if (commands[0] == 1) streamy << " !Full Packet Received! woo! ";
        }
        //if (buf->size() < 10)
        //{
            //status = 2 
        //}
        PRR = read_Packet();
        short dataSource;
        dataSource = extractSourceAddress();

        short seqNum;
        seqNum = extractSequenceNumber();
        
        if (PRR == 1)//if the packet is relevent to us and is data queue it up
        {
            //wcerr << "SILLYNESS " << seqNumMang.getSeqNum(dataSource) + 1 << " :: " << seqNum << endl;
            if ( seqNumMang.getSeqNum(dataSource) + 1 == seqNum )
            {
                Packet paulLovesPBR( extractSourceAddress(), extractSequenceNumber() );
                char theFrame[paulLovesPBR.frame_size];
                //char* pointerToTheFrame = &theFrame[0];
                paulLovesPBR.buildByteArray(&theFrame[0]);
                if (prints) wcerr << "Paul loaths PBR :: " << paulLovesPBR.frame_size << endl;
                usleep(aSIFSTime * 1000);
                daRF->transmit( &theFrame[0], paulLovesPBR.frame_size );
                seqNumMang.increment(dataSource);
                queue_data();//put data in the queue
            }
            else
            {
                if (prints) wcerr << "Unexpected sequence number for DATA from " << extractSourceAddress() << endl;
                //if (commands[0] == 1) streamy << "Unexpected sequence number" << endl;
            }
        }
        if (PRR == 2)//if the packet is relevent and is an ACK adjust ack recived flag
        {
            if (seqNum == *expectedSN)
            {
                *ackReceivedL = true;
            }
            else
            {
                if (prints) wcerr << "Unexpected sequence number for ACK" << endl;
                //if (commands[0] == 1) streamy << "Unexpected sequence number" << endl;
            }
        }
        if (PRR == 3)//if a beacon comes in
        {
            long long newTimeStamp = extractTimeStamp();//get their timpe stamp from them
            volatile long long ourTmSmp = daRF->clock() + *fudgeFactor;//figure out what time we think it is
            long long diff = newTimeStamp - ourTmSmp;// compute the difference 
            if (diff > 0)//if their clock is running faster than ours go to their time
            {
                fudgeFactor = &diff;// update the fudge factor 
                //TODO figure our our program times for sending and reciving
            }
        }
    }
}

int
Listener::queue_data()
{
    //int queueSize = *daLoopLine.size();
    if ( daLoopLine->size() > 4)
    {
        //status = 10; // report that the queue for incoming data is full
        if (prints) wcerr << "Queue too full to recive incoming Packets" << endl;
        //if (commands[0] == 1) streamy << "Queue too full to recive incoming Packets" << endl;
    }
    else
    {
        if (prints) wcerr << "putting data in queue" << endl;
        //if (commands[0] == 1) streamy << "putting data in queue" << endl;
        Packet toDemiBrad(&buf[0], bytesReceived);//create a packet
        /*
         * testing shiz to make sure the right data is being sent out
         toDemiBrad.init_Packet(&buf[0], bytesReceived);//construct the guts of the packet using the buffer and total byets recived
        int i = 5;
        while(i < bytesReceived - 4){
            wcerr << buf[i] << " :: ";
            i++;
        }
        wcerr << "\n";*/
        pthread_mutex_lock(mutexListener);//lock the queue off
        daLoopLine->push(toDemiBrad);//send the pointer to the packet up to demibrad
        pthread_mutex_unlock(mutexListener); //unlock the queue so demibrad can have shot at it
        /*  
         * this is the code that has moved over to packet now 
        int size = bytesReceived-10;//total size of incoming data minus 10 bytes of header and CRC
        char dataIn[size];//a new char array for just the incoming data
        short dataSource = buf[4];//extract the source address
        dataSource << 8;
        dataSource = dataSource + buf[5];
        for (int i = 6; i < bytesReceived-4; ++i)//strip the headers and put just the data in our tuple
        {
            dataIn[i-6] = buf[i];//the offset of six is the front header being skipped in our buf and the four less is the CRC
        }
        */ 
    }   
}

short
Listener::extractSequenceNumber()
{
    unsigned short SN = buf[0];//extract the sequence number 
    SN = SN << 12;
    SN = SN >> 4;
    unsigned short temp = buf[1];
    temp = temp << 8;
    temp = temp >> 8;
    SN = SN + temp;
    //SN = SN << 4;//shift other data off the sequence number
    //SN = SN >> 4;
    return SN; 
}

short
Listener::extractSourceAddress()
{
    unsigned short DS = buf[4];//extract the source address
    DS = DS << 8;
    unsigned short temp = buf[5];
    temp = temp << 8;
    temp = temp >> 8;
    DS = DS + temp;
    return DS;
}

long long
Listener::extractTimeStamp()
{
    int size = bytesReceived-10;//total size of incoming data minus 10 bytes of header and CRC
    char timeStamp[size];//a new char array for just the incoming data
    for (int i = 6; i < bytesReceived-4; ++i)//strip the headers and put just the data in our tuple
    {
        timeStamp[i-6] = buf[i];//the offset of six is the front header being skipped in our buf and the four less is the CRC
    }
    long long tmStmp = 0;
    for (int i = 0; i < size-1; ++i)//build the long long that will hold the time stamp
    {
        tmStmp = tmStmp + timeStamp[i];
        tmStmp = tmStmp << 8;
    }
    tmStmp = tmStmp + timeStamp[size-1];//the last byte needs to happens outside the loop to prevent over shifting  the data
    return tmStmp;
}



