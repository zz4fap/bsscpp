#include <math.h>
#include <iostream>
#include <string>
#include <string.h>
#include <fstream>
#include <iomanip>
#include <stdlib.h>

using namespace std;

class A
{
	protected:
			int a;
	public:
			int b;
};

class B : public A
{
	public:
			void go();
};

void B::go()
{
	A c;
	
	c.a = 10;
	c.b = 10;
}

int main(int argc, char* argv[])
{
	B b;
	
	return 0;
}
	
