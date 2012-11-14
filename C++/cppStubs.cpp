/*
 * This file contains "stub" code that helps bridge the gap between the
 * Java-based client application and your link layer (802.11~) code.  The
 * client application doesn't call the 802.11~ routines directly.  Instead,
 * it calls the C++ functions defined in this file, and THEY call the 
 * 802.11~ routines.  This allows the client to interface with the 802.11~
 * routines even though they have signatures that cannot be supported in
 * Java.  (E.g. the dot11_recv function makes use of reference parameters
 * to essentially return three values.)  This extra layer also defines the
 * stream mechanism that's used to pass character-based output from your
 * code to the display window.
 * 
 * You should NOT need to modify any of this code, with the exception of 
 * the list of MAC addresses used to initialize the array "addrs" below.
 * (It's used to provide default MAC addresses for the client's send
 * buttons.)
 */

#include <jni.h>
#include <iostream>
#include <sstream>
#include <string>
#include <stdlib.h>     // For exit()
#include <arpa/inet.h>	// For htons()
#include "WiFiClient.h"
#include "linklayer.h"
    
    
/*
 * This array of MAC addresses is used to initialize the buttons in the
 * client application.  Feel free to edit the list of addresses, but be
 * sure to update NUM_ADDRS if you change the length of the list.  (The
 * client won't look at more than 9 anyway, though it should tolerate a
 * shorter list.)
 */
unsigned short addrs[] = {101, 201, 301, 401, 501, 601, 701, 801, 901};
#define NUM_ADDRS  9


using namespace std;

#define MAX_CHAR_BUF_LEN  10240
#define MAX_RECV_BUF_LEN  2048
    
char char_buf[MAX_CHAR_BUF_LEN];
char recv_buf[MAX_RECV_BUF_LEN];

/*
 * The stubs layer defines an output stream of type stringstream.  This
 * stream is passed as argument to dot11_init, and is also accessed by
 * the pollForStreamOutput, which is called periodically by the client
 * to retrieve output from your layer.
 */
stringstream os;



/*
 * Class:     WiFiClient
 * Method:    getDefaultAddrs
 * Signature: ()[S
 * 
 * Returns an array of MAC addresses (shorts) to be used to initialize
 * the send buttons.
 */
JNIEXPORT jshortArray JNICALL Java_WiFiClient_getDefaultAddrs
    (JNIEnv *env, jobject obj) 
{
    jshortArray MACs = env->NewShortArray(NUM_ADDRS);
    if (MACs == NULL) {
        cerr << "Unable to create array of MAC addresses!" << endl;
        exit(1);
    }
    env->SetShortArrayRegion(MACs, 0, (jint)NUM_ADDRS, (jshort*)addrs);
    return MACs;
}



/*
 * Class:     CppGUIAdapter
 * Method:    initializeLinkLayer
 * Signature: (S)I
 *
 * Calls the 802 layer's init function.
 */
JNIEXPORT jint JNICALL Java_WiFiClient_initializeLinkLayer
    (JNIEnv *env, jobject obj, jshort MACaddr)
{
    return (jint)dot11_init((short)MACaddr, &os);
}



/*
 * Class:     CppGUIAdapter
 * Method:    watchForIncomingData
 * Signature: ()[B
 *
 * Calls the 802 layer's receive routine, waits for the results, then
 * prepends the sender's address to the data and returns it as an array
 * of bytes.  This allows both the data and the sender's address to be
 * passed to the Java side, without having to create an object to contain
 * them.  (The destination address is not retrieved by the client.)
 */
JNIEXPORT jbyteArray JNICALL Java_WiFiClient_watchForIncomingData
    (JNIEnv *env, jobject obj)
{
    short src, dst;
    
    int result = dot11_recv(&src, &dst, recv_buf, MAX_RECV_BUF_LEN);
    if (result >= 0) {
        jbyteArray bytes = env->NewByteArray(result+2);
        env->SetByteArrayRegion(bytes, 2, (jint)result, (jbyte*)recv_buf);
        src = htons(src);  // reorder the bytes
        env->SetByteArrayRegion(bytes, 0, (jint)2, (jbyte*)&src);
        return bytes;
    }
    else
        return (jbyteArray)NULL;
}



/*
 * Class:     CppGUIAdapter
 * Method:    sendOutgoingData
 * Signature: (S[B)I
 * 
 * Calls the 802 layer's send routine and returns the result.
 */
JNIEXPORT jint JNICALL Java_WiFiClient_sendOutgoingData
    (JNIEnv *env, jobject obj, jshort dest, jbyteArray data)
{
    int dataSize = (int)(env->GetArrayLength(data));  // See how much to send
    char buf[dataSize];                               // create C++ array to hold it
    env->GetByteArrayRegion(data, 0, dataSize, (jbyte*)buf);  // Copy from Java array
    int result = dot11_send((short)dest, buf, dataSize);      // Pass C++ array to 802.11
    return (jint)result;
}



/*
 * Class:     CppGUIAdapter
 * Method:    pollForStreamOutput
 * Signature: ()[B
 *
 * Grabs any new output on the output stream and passes it to the display
 * object as a byte array.
 */
JNIEXPORT jbyteArray JNICALL Java_WiFiClient_pollForStreamOutput
    (JNIEnv *env, jobject obj)
{
    os.flush();
    stringbuf* buf = os.rdbuf();               // Get pointer to stringstream's chars
    int i=0;
    while (i < MAX_CHAR_BUF_LEN && buf->sgetc() != EOF)                // Copy new chars to a buffer
        char_buf[i++] = (char)(buf->sbumpc());
    
    if (i==0) return (jbyteArray)NULL;
    
    jbyteArray bytes = env->NewByteArray(i);   // Copy *again* to a Java byte array
    env->SetByteArrayRegion(bytes, 0, (jint)i, (jbyte*)char_buf);
    return bytes;
}


/*
 * Class:     CppGUIAdapter
 * Method:    sendCommand
 * Signature: (II)I
 */
JNIEXPORT jint JNICALL Java_WiFiClient_sendCommand
  (JNIEnv *env, jobject obj, jint cmd, jint val)
{
	return (jint)dot11_command((int)cmd, (int)val);
}

