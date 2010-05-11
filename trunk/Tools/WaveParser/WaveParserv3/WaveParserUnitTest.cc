//g++ WaveParserUnitTest.cc WaveParser.cc -o WaveParserUnitTest

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <iomanip>

#include "WaveParser.hh"

using namespace std;

int testingReadingChannel1(WaveParser& wp)
{
	short buffer[100], buffer2[100];
	FILE* arquivoMatlab = NULL;

	if( (arquivoMatlab = fopen( "felipeLeft.dat", "rb" )) == NULL )
		cout << "The file: matlab.mat was not opened" << endl;
	else
		cout << "The file: matlab.mat was opened" << endl;

	if (fread(buffer, sizeof(short), 100, arquivoMatlab) != 100) 
		cout <<  "Erro de leitura" << endl;

	wp.getChannelData(1, 100, buffer2);

	int sum = 0;
	for(int i=0; i<100; i++)
	{
		sum += buffer2[i] - buffer[i];
	}

	cout << "Sum is equal to: " << sum << endl;

	return sum;
}

int testingReadingChannel2(WaveParser& wp)
{
	short buffer[100], buffer2[100];
	FILE* arquivoMatlab = NULL;

	if( (arquivoMatlab = fopen( "felipeRight.dat", "rb" )) == NULL )
		cout << "The file: matlab.mat was not opened" << endl;
	else
		cout << "The file: matlab.mat was opened" << endl;

	if (fread(buffer, sizeof(short), 100, arquivoMatlab) != 100) 
		cout <<  "Erro de leitura" << endl;

	wp.getChannelData(2, 100, buffer2);

	int sum = 0;
	for(int i=0; i<100; i++)
	{
		sum += buffer2[i] - buffer[i];
	}

	fclose(arquivoMatlab);

	cout << "Sum is equal to: " << sum << endl;

	return sum;
}

int testingReadingChannel1WithOffset(WaveParser& wp, int offset)
{
	short buffer[100+offset], buffer2[100];
	FILE* arquivoMatlab = NULL;

	if( (arquivoMatlab = fopen( "felipeLeft.dat", "rb" )) == NULL )
		cout << "The file: matlab.mat was not opened" << endl;
	else
		cout << "The file: matlab.mat was opened" << endl;

	if (fread(buffer, sizeof(short), (100+offset), arquivoMatlab) != (100+offset)) 
		cout <<  "Erro de leitura" << endl;

	wp.getChannelData(1, 100, buffer2, offset);

	int sum = 0;
	for(int i=0; i<100; i++)
	{
		sum += buffer2[i] - buffer[i+offset];
	}

	fclose(arquivoMatlab);

	cout << "testingReadingChannel1WithOffset: Sum is equal to: " << sum << endl;

	return sum;
}

int testingReadingChannel2WithOffset(WaveParser& wp, int offset)
{
	short buffer[100+offset], buffer2[100];
	FILE* arquivoMatlab = NULL;

	if( (arquivoMatlab = fopen( "felipeRight.dat", "rb" )) == NULL )
		cout << "The file: matlab.mat was not opened" << endl;
	else
		cout << "The file: matlab.mat was opened" << endl;

	if (fread(buffer, sizeof(short), (100+offset), arquivoMatlab) != (100+offset)) 
		cout <<  "Erro de leitura" << endl;

	wp.getChannelData(2, 100, buffer2, offset);

	int sum = 0;
	for(int i=0; i<100; i++)
	{
		sum += buffer2[i] - buffer[i+offset];
	}

	fclose(arquivoMatlab);

	cout << "testingReadingChannel2WithOffset: Sum is equal to: " << sum << endl;

	return sum;
}

int main(int argc, char* argv[])
{
	try
	{
		WaveParser wp("FelipeSozinho.wav");
		//WaveParser wp("online.wav");
		//WaveParser wp("F221610.WAV");
		wp.printWaveFileInfo();

		cout << endl << endl;

		cout << "getNumberOfChannels: " << wp.getNumberOfChannels() << endl;
		cout << "getSampleRate: " << wp.getSampleRate() << endl;
		cout << "getBytesPerSample: " << wp.getBytesPerSample() << endl;
		cout << "getDataSubChunkSize: " << wp.getDataSubChunkSize() << endl;
		cout << "getOffsetFromBeginningtoDataChunk: " << wp.getOffsetFromBeginningtoDataChunk()  << endl;
		cout << "getAbsolutePosition: " << wp.getAbsolutePosition() << endl;

		cout << endl << endl;

		if(!testingReadingChannel1(wp) && !testingReadingChannel2(wp) && !testingReadingChannel1WithOffset(wp, 67) && !testingReadingChannel1WithOffset(wp, 36) && !testingReadingChannel2WithOffset(wp,20) && !testingReadingChannel2WithOffset(wp,17))
			cout << "Sum is equal to: 0 - RESULT: OK!" << endl;
		else
			cout << "THERE WAS A PROBLEM - RESULT NOK!!!!!!!!!!!!" << endl;
	}
	catch(string& str)
	{
		cout << str << endl;
		exit(1);
	}

	return 0;
}
