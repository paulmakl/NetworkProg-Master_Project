#include <iostream>
#include <arpa/inet.h>
#include <pthread.h> 
#include <time.h>
using namespace std;

short frametype;
bool resend;
unsigned short sequence_number;
unsigned short destination;
unsigned short sender;
char* data;
unsigned int CRC;
int bytes_to_send;
int packet_size;

int makePacket(short frm, bool resen, unsigned short sn, unsigned short dest, unsigned short sendr, char* dta, int CS, int size){
	frametype = frm;
	resend = resen;
	sequence_number = sn;
	destination = dest;
	sender = sendr;
	data = dta;
	CRC = CS;
	bytes_to_send = size;
	packet_size = size + 10;
}
void Packet(){

}
int buildByteArray(unsigned char* Packet){
	//unsigned char Packet[packet_size];
	// Set the CRC
	int i = 0;
	unsigned int temp_int = 0;
	while(i < 4){
		temp_int = CRC << 24 - 8*i;
		temp_int = temp_int >> 24;
		Packet[packet_size-5+i] = temp_int;
		cout << temp_int << "\n";
		i++;
	}

	// put all the data in the packet
	/*i = 0;
	while(i<2038){
		packet[i + 7] = data[i];
		i++;
	}*/
	// put in source address
	unsigned short temp_short = 0;
	temp_short = sender << 8;
	temp_short = temp_short >> 8;
	Packet[4] = temp_short;
	cout << ":" << temp_short << "\n";
	temp_short = sender >> 8;
	Packet[5] = temp_short;
	cout << ":" << temp_short << "\n";
	// put in dest address
	temp_short = destination << 8;
	temp_short = temp_short >> 8;
	Packet[2] = temp_short;
	cout << ":" << temp_short << "\n";
	temp_short = destination >> 8;
	Packet[3] = temp_short;
	cout << ":" << temp_short << "\n";
	// deal with control garbage
}
int make_resend(){
	return 1;
}


int main(int argc, char const *argv[])
{
	CRC = 4294967294;
	sender = 49;
	destination = 48;
	int packy_size = 2048;
	unsigned char packy[packy_size];
	unsigned char * q = &packy[0];
	buildByteArray(q);
    return 0;
}
