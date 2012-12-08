
#include <iostream>
using namespace std;


 unsigned char pullByte(long long *number, int index) {}

int main(int *argc, char *argv[]) {
        long long temp = 0x50000001;
        cout << "Number: " << temp << endl;
        cout << "Pulling 1st byte, expecting 1: "<< pullByte(&temp, 1) << endl;
        cout << "Pulling 8th byte, expecting 80: "<< pullByte(&temp, 4) << endl;
}



