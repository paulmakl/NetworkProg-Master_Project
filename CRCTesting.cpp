#import "packet.h"
using namespace std;

int main(int argc, char const *argv[])
{
	Packet p;
	int size = 52;
	char test[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
	char testt[size+4];
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
	//p.shift_char_array(&test[0], size);
	//p.flip_nth_bit(&testt[0], 5);
	int i = 0;
	while(i < size+4){
		cout << testt[i] + 0 << endl;
		i++;
	}
	return 0;
}