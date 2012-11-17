#include "SeqNumManager.h"
#include <iostream>

using std::cout;
using std::endl;

int main(int argc, char* argv[]) {
    SeqNumManager test = SeqNumManager(4);

    cout << "expecting -1 : " <<  test.getSeqNum(1) << endl;
    test.increment(2);
    cout << "expecting 0 : " << test.getSeqNum(2) << endl;
    cout << "expecting counting up to 0 : " << endl;
    for (int i=0; i<5; i++) {
        test.increment(2);
        cout << test.getSeqNum(2) << endl;
    }
    return 1;
}
