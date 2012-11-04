#include <iostream>;
#include <tuple>
#include "RF.h";
#include <Packet.h>;
using namespace std;

// taking a blind swing at some of things we will want in this bad boy not sure I have all my shit strait
//I have no idea how to properly do a constructor in relation to the whole .h stuff I figure this is shit we can work out
int main(int argc, char const *argv[])
{
	daRF->attachThread();// attach the thread
	MACACK = 0;//special case no need to send an ACK
	ack_Received = false;// indicates if an ack comes in

}

int ultraListen()
{
    while(true){
            // get the bytes received
        bytesReceived = daRF->receive(buf, MAXPACKETSIZE);
            //print the bytes received and checks for errors
        if (bytesReceived != MAXPACKETSIZE){
            wcerr << "Received  partial Packet with " << bytesReceived << " bytes of data!" << endl;
        }
        else{
            wcerr << bytesReceived << " !Full Packet Received!  ";
        }
        PRR = read_Packet();
        if (PRR == 1)//if the packet is relevent to us and is data queue it up
        {
        	status = queue_data();//put data in the fifo
        }
        if (PRR == 2)//if the packet is relevent and is an ACK ajust ack recived flag
        {
        	ack_Received == true;
        }
    }
}

int read_Packet ()
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
