#ifndef LINKLAYER_H
#define LINKLAYER_H

/*
 * This file contains the prototypes and descriptions of the 802.11~ 
 * interface routines.  
 */

#include <iostream>

/*
 * These are the currently defined status codes.  See the online
 * documentation for more details.
 */


#define SUCCESS				1
#define UNSPECIFIED_ERROR	2
#define RF_INIT_FAILED		3
#define TX_DELIVERED		4
#define TX_FAILED			5
#define BAD_BUF_SIZE		6
#define BAD_ADDRESS			7
#define BAD_MAC_ADDRESS		8
#define ILLEGAL_ARGUMENT	9
#define INSUFFICIENT_BUFFER_SPACE	10
 
/* 
 * This routine must be called before any of the other 802.11~ routines 
 * are used. The station adopts the 16-bit MAC address passed as argument.
 * The second argument is an output stream to which diagnostic messages
 * should be printed, unless the argument is null. The call returns -1
 * on error and sets an internal error code.
 */
int dot11_init(short MACaddr, std::ostream *output);
 
/* 
 * This non-blocking function asks the 802.11~ layer to transmit
 * bufSize bytes, starting at address buf, to the specified destination
 * address. The function returns the number of bytes queued for
 * transmission, or -1 on error, in which case an internal error code
 * is set. Once the queued data is successfully transmitted and
 * acknowledged, the internal status code is updated.
 */
int dot11_send(short destAddr, char *buf, int bufSize);


/* 
 * This function blocks until data arrives, at which point it returns
 * the number of bytes of data copied into buf, respecting the size
 * limit passed in via bufSize. The source and destination addresses
 * are filled in as a result of the call. (Only data addressed to this
 * station is returned via 802_recv, but destAddr can help a station
 * determine whether the frame was broadcast to all stations, or
 * addressed specifically to this station.) If the buffer is too small
 * to hold the entire data payload of the incoming transmission, the
 * rest should be discarded. The function returns -1 on error, and sets
 * an internal error code.
 */
int dot11_recv(short *srcAddr, short *destAddr, char *buf, int bufSize);

/*
 * This function returns a code representing the current status of the
 * 802.11~ layer. The code reflects the most recent error, or the
 * status of the most recent transmission or other operation.  See the
 * online documentation for a list of status codes.
 */
int dot11_status();
 
/* 
 * This function provides a mechanism to pass command or configuration
 * data to an 802.11~ layer at runtime. One could use it to enable or
 * disable debugging output on-the-fly, change system parameters, or
 * prompt the 802.11~ layer to summarize network activity, for example.
 * Note: User-defined command values should be greater than 10. The
 * 802.11~ specification committee has reserved the values 0—10 and is
 * still debating their functionality.
 */
int dot11_command(int cmd, int val);

#endif
