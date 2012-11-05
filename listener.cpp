#include <iostream>
#include <tuple>
#include "RF.h"
#include <Packet.h>
#include "CircularFifo.h"
#include "listener.h"

using namespace std;

short MACACK; //global varribles
bool ack_Received;
unsigned short MACaddr;
static const int MAXPACKETSIZE = 2048; //size guarenteed to hold all properly formated packets
char buf[MAXPACKETSIZE];// buffer for the incoming packets
CircularFifo<int,10> daLoopLine;
RF* daRF;

Listener::Listener(RF* RFLayer, CircularFifo<int,10>* incomingQueue, unsigned short* sendFlag, bool* receivedFlag, unsigned short myMAC)
{
    daRF = RFLayer;//our reference to the RF layer
    MACACK = sendFlag;
    MACACK = 0;//special case no need to send an ACK
    ack_Received = receivedFlag;
    ack_Received = false;// indicates if an ack comes in
    MACaddr = myMAC;
    daLoopLine = incomingQueue;
}

int
Listener::UltraListen()
{
    while(true){
        int bytesReceived;// bytes from the last packet
        int PRR;//Packet Read Result
        // wait for data
        bytesReceived = daRF->receive(buf, MAXPACKETSIZE);//block until data comes our way
            //print the bytes received and checks for errors
        if (bytesReceived != MAXPACKETSIZE){
            wcerr << "Received  partial Packet with " << bytesReceived << " bytes of data!" << endl;
        }
        else{
            wcerr << bytesReceived << " !Full Packet Received! woo! ";
        }
        PRR = read_Packet();
        if (PRR == 1)//if the packet is relevent to us and is data queue it up
        {
            short dataSource = buf[4];//extract the source address
            dataSource << 8;
            dataSource = dataSource + buf[5];
            MACACK = dataSource;//let the sender know to send an ACK for this data
            queue_data();//put data in the cirfifo
        }
        if (PRR == 2)//if the packet is relevent and is an ACK adjust ack recived flag
        {
            ack_Received = true;
        }
    }
}

int
Listener::read_Packet ()
{
    int status;//will be returned with different status code to help ultra listen react
    short packetDest = buf[2];//bitwise terribleness
    packetDest << 8;
    packetDest = packetDest + buf[3];
    if (packetDest != MACaddr)//compare the destination of this packet to our MAC address
    {
        status = 0;//this packet isn't for us
        wcerr << "Packet not addressed to current MAC address." << endl;
        return status;
    }
    char frameType = buf[0];
    frameType >> 5;
    switch (frameType)//compare the frame type of the packet given to known types to figure out what kind of packet it is
    {
        case 0:
            wcerr << "Data packet received." << endl;
            status = 1;
            break;

        case 1:
            wcerr << "ACK Received." << endl;
            status = 2;
            break;

        case 2:
            wcerr << "Beacon Received." << endl;
            status = 3;
            break;

        default:
            wcerr << "Unknown packet type received." << endl;
            status = 3;
            break;
    }
    return status;
}
