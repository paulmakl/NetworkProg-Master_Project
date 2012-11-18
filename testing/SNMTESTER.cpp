#include "SeqNumManager.h"
#include <iostream>

using std::cout;
using std::endl;

int main(int argc, char* argv[]) {
    unsigned short four = 4;
    unsigned short one = 1;
    unsigned short two = 2;
    SeqNumManager test = SeqNumManager(four);

    cout << "expecting -1 : " <<  test.getSeqNum(one) << endl;
    test.increment(2);
    cout << "expecting 0 : " << test.getSeqNum(two) << endl;
    cout << "expecting counting up to 0 : " << endl;
    for (int i=0; i<5; i++) {
        test.increment(two);
        cout << test.getSeqNum(two) << endl;
    }
    return 1;
}
