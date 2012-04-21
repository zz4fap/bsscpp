//g++ WaveParserUnitTest.cc WaveParser.cc -o WaveParserUnitTest

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <iomanip>

using namespace std;

int main()
{
	short t = 0xAABBCC;
	cout << hex << setfill('0') << setw(2) << t << endl;



	return 0;
}
