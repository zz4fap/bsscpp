//g++ WaveParserUnitTest.cc WaveParser.cc -o WaveParserUnitTest

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <iomanip>

#include "WaveParser.hh"

using namespace std;

typedef struct
{
	int test;
}TEST;

int main(int argc, char* argv[])
{
	try
	{
		WaveParser<short> wp("FelipeSozinho.wav");
		//WaveParser wp("online.wav");
		//WaveParser wp("F221610.WAV");
		wp.printWaveFileInfo();

		cout << endl << endl;

		cout << wp.getNumberOfChannels() << endl;
		cout << wp.getSampleRate() << endl;
		cout << wp.getBytesPerSample() << endl;
		cout << wp.getDataSubChunkSize() << endl;
		cout << wp.getOffsetFromBeginning()  << endl;
		cout << wp.getCurrentPosition() << endl;

		cout << endl << endl;

		short buffer[11];
		/*wp.getChannelData16Bits(1, 11, buffer);
		for(int i=0; i<11; i++)
			cout << buffer[i] << endl;*/


		cout << endl << endl;

		wp.getChannelData(2, 11, buffer);
		for(int i=0; i<11; i++)
			cout << buffer[i] << endl;

	}
	catch(string& str)
	{
		cout << str << endl;
		exit(1);
	}


	return 0;
}
