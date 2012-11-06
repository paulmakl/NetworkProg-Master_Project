#include "sender.h"
#include "CircularFifo.h"

using namespace std;

int main(int argc, char const *argv[]) {
    
    
    //Test Sender
    RF* RFtest = new RF();
    CircularFifo<Packet*,2> fifo;
    CircularFifo<Packet*,2>* fifoTest;
    unsigned short flag = 1;
    unsigned short* flagTest = &flag;
    bool recFlag = false;
    bool* recFlagTest = &recFlag;
    unsigned short macTest = 12;

    Sender* test = new Sender(RFtest, fifoTest, flagTest, recFlagTest, macTest);

    //Test Packet
    short frameTest = 12;
    bool resendTest = false;
    unsigned short seqTest = 34;
    unsigned short destTest = 56;
    unsigned short sourceTest = 78;
    char data = 'a';
    char* dataTest = &data;
    int crcTest = 910;
    int sizeTest = 1112;
    Packet testPack;
    testPack.initPacket(frameTest, resendTest, seqTest, destTest, sourceTest, 
            dataTest, crcTest, sizeTest);
}
