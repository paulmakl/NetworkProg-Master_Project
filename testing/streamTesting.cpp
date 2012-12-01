#include <iostream>
#include <fstream>
using namespace std;

int main(int argc, char const *argv[])
{
	filebuf fb;
	fb.open ("test.txt",ios::out);
	ostream os(&fb);
	os << "Test sentence\n";
	fb.close();
	return 0;
}