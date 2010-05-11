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

int readingChannelDataUntilTheEnd(WaveParser& wp, int channel, int bufferSize)
{
	short buffer[bufferSize];
	int sizeInBytes = 0;
	int readShorts = 0;
	
	while((readShorts = wp.getChannelData(channel, bufferSize, buffer)) == bufferSize)
		sizeInBytes += readShorts*2;

	if(readShorts > 0)
		sizeInBytes += readShorts*2;

	return sizeInBytes; 
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

		cout << endl << endl;

		if(readingChannelDataUntilTheEnd(wp, 2, 23) != 2384440)
			cout << "ERROR!!!!!!!"  << endl;

		if(readingChannelDataUntilTheEnd(wp, 1, 36) != 2384440)
			cout << "ERROR!!!!!!!"  << endl;
	
		if(!readingChannelDataUntilTheEnd(wp, 1, 21) && !readingChannelDataUntilTheEnd(wp, 1, 78) && !readingChannelDataUntilTheEnd(wp, 1, 53) && !readingChannelDataUntilTheEnd(wp, 2, 12))
			cout << "O size returned - RESULT: OK!" << endl;

		wp.goBackToBeginOfDataChunk();

		if(readingChannelDataUntilTheEnd(wp, 1, 21) == 2384640)
			cout << "Channel 1 size: 2384640 - RESULT: OK!" << endl;

		if(readingChannelDataUntilTheEnd(wp, 2, 21) == 2384640)
			cout << "Channel 2 size: 2384640 - RESULT: OK!" << endl;

		wp.goBackToBeginOfDataChunk();
		if((readingChannelDataUntilTheEnd(wp, 1, 123) + readingChannelDataUntilTheEnd(wp, 2, 234)) != wp.getDataSubChunkSize())
			cout << "Size of the file is different from the one in the data sub chunk info!!!!!!!!!!" <<  endl;
		else
			cout << "Size of Data Chunk: " <<  wp.getDataSubChunkSize() << " - RESULT: OK!" << endl;

		wp.goBackToBeginOfDataChunk();

		cout << endl << endl;
		//TODO: Put your code here!!
		
	}
	catch(string& str)
	{
		cout << str << endl;
		exit(1);
	}

	return 0;
}
