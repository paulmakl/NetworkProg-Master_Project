#include <iostream>;
#include "RF.h";
#include <Packet.h>; 
using namespace std;

// taking a blind swing at some of things we will want in this bad boy not sure I have all my shit strait
//I have no idea how to properly do a constructor in relation to the whole .h stuff I figure this is shit we can work out
RF* daRF = new RF();
daRF->attachThread();
MACACK = 0;
ack_Received = false;
