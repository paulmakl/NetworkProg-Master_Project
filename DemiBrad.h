/*
 *
 * this will contain out DemiBrad class.
 * DemiBrad does lots of useful things like
 * store data in queues and make threads.
 *
 */

#include <iostream>

class DemiBrad{

	public:
		/*
		 * Constructor that does cool stuff and things like stuff
		 */
		dot11_init(short MACaddr, *ostream streamy);
		int dot11_command(int cmd, int val);
		int status();
		int dot11_recv(short *srcAddr, short *destAddr, char *buf, int bufSize);
		int dot11_send(short destAddr, char *buf, int bufSize);
		int create_sender_thread(*bool ack_);
	private:
		short MACaddr;
		short streamy;

}