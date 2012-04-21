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
	signed char var[] = {0xfe,0xff};

	//for(int i=0;i<2;i++)
	//	cout << hex << setfill('0') << setw(2) << (int)var[i] << " ";

	//float value = (float) ((var[1] << 8) | (((int)var[0]) & 0xFF));

	//int value = (int)var[0];

	float value = (float) (((var[1] << 8) | (((int)var[0]) & 0xFF))/32768.0);

	cout << value << endl;

	return 0;
}
