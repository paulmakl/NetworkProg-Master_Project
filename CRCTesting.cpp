#import "packet.h"
using namespace std;

int main(int argc, char const *argv[])
{
	Packet p;
	int size = 52;
	char test[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
	char testt[size+4];
	int a = 7;
	int b = 8;
	int c = a/b;
	cout << c << endl;
	
	int j = 0;
	while(j < size+4){
		if (j < size)
		{
			testt[j] = test[j];
		}else{
			testt[j] = 0;
		}
		j++;
	}
	p.build_CRC(&test[0], size, 79764919);
	/*
	//p.shift_char_array(&test[0], size);
	//p.flip_nth_bit(&testt[0], 5);
	int i = 0;
	while(i < size+4){
		cout << testt[i] + 0 << endl;
		i++;
	}*/
	return 0;
}