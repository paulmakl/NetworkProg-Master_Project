#include <iostream>
#include <arpa/inet.h>
#include <pthread.h> 
#include <time.h>
#include "packet.h"
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

void makePacket(){
}/*(short frm, bool resen, unsigned short sn, unsigned short dest, unsigned short sendr, char* dta, int CS, int size){
	frametype = frm;
	resend = resen;
	sequence_number = sn;
	destination = dest;
	sender = sendr;
	data = dta;
	CRC = CS;
	bytes_to_send = size;
	packet_size = size + 10;
}*/

int make_resend(){
	resend = true;
	return 1;
}