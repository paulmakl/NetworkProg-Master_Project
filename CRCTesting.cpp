#import "packet.h"
using namespace std;

int main(int argc, char const *argv[])
{
	Packet p;
	int normal_size = 104;
	int size = 108;
	char test[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
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
	p.build_CRC(&testt[0], size, 79764919);
	cout << testt[104] + 0 << " " << testt[105] + 0 << " " << testt[106] + 0 << " " << testt[107] + 0 << endl;
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