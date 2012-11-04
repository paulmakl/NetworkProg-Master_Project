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

int makePacket(short frm, bool resen, unsigned short sn, unsigned short dest, unsigned short sendr, char* dta, int CS){
	frametype = frm;
	resend = resen;
	sequence_number = sn;
	destination = dest;
	sender = sendr;
	data = dta;
	CRC = CS;
}
void Packet(){

}
int buildByteArray(){
	// Create an unsigned char array
	unsigned char CRCArray[4];
	// Set the CRC
	int i = 0;
	unsigned int temp_int = 0;
	while(i < 4){
		temp_int = CRC << 24 - 8*i;
		temp_int = temp_int >> 24;
		CRCArray[i] = temp_int;
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
	unsigned char Dest_Src_Array[4];
	unsigned short temp_short = 0;
	temp_short = sender << 8;
	temp_short = temp_short >> 8;
	Dest_Src_Array[2] = temp_short;
	cout << ":" << temp_short << "\n";
	temp_short = sender >> 8;
	Dest_Src_Array[3] = temp_short;
	cout << ":" << temp_short << "\n";
	// put in dest address
	temp_short = destination << 8;
	temp_short = temp_short >> 8;
	Dest_Src_Array[0] = temp_short;
	cout << ":" << temp_short << "\n";
	temp_short = destination >> 8;
	Dest_Src_Array[1] = temp_short;
	cout << ":" << temp_short << "\n";
}
int make_resend(){
	return 1;
}


/*int main(int argc, char const *argv[])
{
	CRC = 4294967294;
	sender = 49;
	destination = 48;
	buildByteArray();
    return 0;
}*/
