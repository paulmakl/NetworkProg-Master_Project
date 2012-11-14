/*
 * You are not expected to understand the code in this file.  See
 * RF.h for a list of methods and their descriptions.
 *
 * Brad Richards, 10/2008
 */


#include <iostream>
#include <stdlib.h>     // For exit()
#include "RF.h"


using namespace std;

unsigned char bcastAddr[] = {255, 255, 255, 255};



/*
 * It looks like we need to call attachThread once from each pthread 
 * that wants to use the RF functions.  Each gets its own env pointer,
 * which is a little odd.  The JVM pointer stays the same across all
 * threads, so we can use it too lookup a thread's env pointer before
 * each call to the Java object.
 *
 * When looking up VMs and envPtrs, I pass a single var instead of
 * an array of JavaVM pointers and JNIEnv pointers.  This only works if
 * there's a single JVM running.
 */
void RF::attachThread() {
    JNIEnv* envPtrs;

    if (jvm->AttachCurrentThread((void**)&envPtrs, NULL) != 0) {
        cerr << "Attach failed!" << endl;
        exit(1);
    }
}


/*
 * The constructor has to set up the Java side of things:  Invoke a
 * JVM instance, create a Java RF object, create a thread for it, and
 * invoke the thread's start method.  We need to ensure that we maintain
 * a global reference to the RF object so that each of the member
 * functions in the C++ class can access their counterparts in the Java
 * version.
 */
RF::RF()
{
    /*
     * First we need to start up a JVM (or find an existing one)
     */
    
    JNIEnv* env;
    JavaVMInitArgs vm_args;
    JavaVMOption options[3];
    
    options[0].optionString = (char*)"-Djava.compiler=NONE";
    options[1].optionString = (char*)"-Djava.classpath=.";
    options[2].optionString = (char*)"-verbose:jni";
    
    vm_args.version = JNI_VERSION_1_4;
    vm_args.nOptions = 2; // Only use first two
    vm_args.options = options;
    vm_args.ignoreUnrecognized = JNI_TRUE;

    jint numVMs = 0;
    jint result = JNI_GetCreatedJavaVMs(&jvm, (jint)1, &numVMs);
    if (!(result == 0 && numVMs > 0)) {
//    cerr << "Didn't find any existing VMs..." << endl;
        result = JNI_CreateJavaVM(&jvm, (void**)&env, &vm_args);
        if (result < 0) {
            cerr << "Wasn't able to create Java Virtual Machine!" << endl;
            exit(-1);
        }
    }
    else {
    // cerr << "Using existing VM" << endl;
    env = getEnvPtr();
    }

    
    /*
     * If we get this far, we've got a working JVM, and we've stored
     * the environment pointer in the global variable "env".  Next we
     * pull out references to the Java RF methods and create an RF object.
     */
    
    rfClass = env->FindClass("rf/RF");
    if (rfClass == 0) {
        cerr << "JVM started, but wasn't able to create an instance of RF." << endl;
	cerr << "Make sure that RF.class is in a directory named rf, and that " << endl;
	cerr << "the rf directory is in the same directory as your executable." << endl;
        exit(-1);
    }   
 
// This was the original, with a one-argument constructor   
//    constructorID = env->GetMethodID(rfClass, "<init>", "(Ljava/io/PrintWriter;)V");
    constructorID = env->GetMethodID(rfClass, "<init>", "(Ljava/io/PrintWriter;[B)V");
    inUseID = env->GetMethodID(rfClass, "inUse", "()Z");
    getIdleTimeID = env->GetMethodID(rfClass, "getIdleTime", "()J");
    dataWaitingID = env->GetMethodID(rfClass, "dataWaiting", "()Z");
    receiveID = env->GetMethodID(rfClass, "receive", "()[B");
    transmitID = env->GetMethodID(rfClass, "transmit", "([B)I");
    clockID = env->GetMethodID(rfClass, "clock", "()J");
    
    if (constructorID==0 || inUseID==0 || getIdleTimeID==0 || dataWaitingID==0 || 
        receiveID==0 || transmitID==0 || clockID == 0)
    {
        cerr << "C++ wrapper was unable to find all Java methods in RF.class" << endl;
        exit(-1);
    }
    
    attachThread(); 
    
    // Create the Java RF object, and make ourselves a global reference to it
//    theRF = env->NewObject(rfClass, constructorID, 0, 0);
    jbyteArray bytes = env->NewByteArray(4);
    if (bytes == NULL) {
        cerr << "C++ constructor unable to create broadcast address!" << endl;
        exit(1);
    }
    env->SetByteArrayRegion(bytes, 0, (jint)4, (jbyte*)bcastAddr);
    theRF = env->NewObject(rfClass, constructorID, 0, bytes);
    env->DeleteLocalRef(bytes);
   

 
    theRF = env->NewGlobalRef(theRF);
    
    /*
     * Almost done!  Now we need to create a Java Thread around our RF object, and
     * invoke the thread's start method.
     *
     
    jclass thread = env->FindClass("java/lang/Thread");
    if (thread == 0) {
        cerr << "Wasnt able to find java.lang.Thread" << endl;
        exit(-1);
    }
    
    jmethodID threadConstructor = env->GetMethodID(thread, "<init>", "(Ljava/lang/Runnable;)V");
    jmethodID start = env->GetMethodID(thread, "start", "()V");
    if (threadConstructor == 0 || start == 0) {
        cerr << "Wasn't able to find all Thread methods" << endl;
        exit(-1);
    }
    
    // Create the thread
    jobject theThread = env->NewObject(thread, threadConstructor, theRF);
    
    // Call its start method
    env->CallVoidMethod(theThread, start);   
*/
}


/*
 * This non-blocking method returns true if the RF layer has one or 
 * more packets to deliver to the MAC layer above it.
 */
bool RF::dataWaiting() {  
    return getEnvPtr()->CallBooleanMethod(theRF, dataWaitingID);
}

/*
 * Returns the number of milliseconds elapsed since the last 
 * transmission on the shared channel ended.
 */
long long RF::getIdleTime() {
    return getEnvPtr()->CallLongMethod(theRF, getIdleTimeID);
}

/*
 * Inspects the shared channel to see if it's currently being used.
 */
bool RF::inUse() {
    return getEnvPtr()->CallBooleanMethod(theRF, inUseID);
}

/*
 * This method blocks until an RF packet arrives, then returns its 
 * contents as a byte array.  We call RF's receive method, which returns an
 * array of bytes that's of just the right length to contain all of the data
 * On the C++ side we do a more traditional "copy into user buffer" thing.
 */
int RF::receive(char* buf, int bufSize) {
    // Call the Java receive to get an array of bytes
    JNIEnv* env = getEnvPtr();
    jbyteArray bytes = static_cast<jbyteArray>(getEnvPtr()->CallObjectMethod(theRF, receiveID));
    int actualLen = env->GetArrayLength(bytes);
    
    // Copy the Java bytes to the user's buffer, respecting user's limit
    bufSize = (bufSize < actualLen) ? bufSize : actualLen;
    env->GetByteArrayRegion(bytes, 0, bufSize, (jbyte*)buf);
    env->DeleteLocalRef(bytes);
    return bufSize;
}

long long RF::clock() {
    return getEnvPtr()->CallLongMethod(theRF, clockID);
}

/*
 * This routine takes data from the MAC layer and broadcasts it to all 
 * units on the same subnet.
 */
int RF::transmit(char* data, int len) {
    JNIEnv* env = getEnvPtr();
    jbyteArray bytes = env->NewByteArray(len);
    if (bytes == NULL) {
        cerr << "C++ transmit method unable to create NewByteArray!" << endl;
        cerr << "JNI env was " << env << endl;
        exit(1);
    }
    env->SetByteArrayRegion(bytes, 0, len, (jbyte*)data);
    int sent = env->CallIntMethod(theRF, transmitID, bytes);
    env->DeleteLocalRef(bytes);
    return sent;
}



/*
 * This helper function looks up the JVM's current env pointer and,
 * if all goes well, returns it.
 */
JNIEnv *RF::getEnvPtr() {
    JNIEnv *env;

    jint res = jvm->GetEnv((void**)&env, JNI_VERSION_1_4);
    if (res == JNI_OK)
        return env;
    else if (res == JNI_EDETACHED) {
        cerr << "Tried to get env pointer from detached thread!" << endl;
        exit(1);
    }
    else {
        cerr << "Unable to retrieve current env pointer" << res << endl;
        exit(1);
    }
}
