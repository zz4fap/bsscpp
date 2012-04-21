#include <iostream>
#include <math.h>

using namespace std;

int main(int argc, char* argv[])
{
	int *i = new int(1234);
	cout << *i << endl;

	char *c = new char('A');
	cout << *c << endl;

	float *f = new float(3.14);
	cout << *f << endl;

	bool *b = new bool;
	cout << *b << endl;

	return 0;
}
