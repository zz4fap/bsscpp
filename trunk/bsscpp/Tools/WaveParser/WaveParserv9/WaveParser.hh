#ifndef WAV_PARSER_HH
#define WAV_PARSER_HH

#include <iostream>
#include <string>
#include <string.h>
#include <fstream>
#include <stdlib.h>

using namespace std;

#define ERROR -1
#define EOFR 0

typedef unsigned short WORD;
typedef unsigned long DWORD;

typedef struct 
{ 
	char  chunckID[4];
	DWORD chunkSize;
	char  format[4];
	char  subChunk1ID[4];
	DWORD subChunk1Size;
	WORD  audioFormat; 
	WORD  numChannels; 
	DWORD sampleRate; 
	DWORD byteRate; 
	WORD  blockAlign; 
	WORD  bitsPerSample;
} WAVEFORMATEX;

typedef struct 
{
	DWORD dataSubChunkSize;
	DWORD offsetFromBeginning; 
}DATACHUNKINFO;
 
class WaveParser
{
		bool retrieveStandardHeader();
		bool retrieveDataSubChunkInfo();
		bool findID(const string& tag);
		bool openWaveFile(const string& fileName);
		void findEndOfFile();

		FILE* waveFileM;

		WAVEFORMATEX WAV;
		DATACHUNKINFO DINFO;

		DWORD sampleRateM;
		WORD  numChannelsM; 
		WORD  bitsPerSampleM; 
		DWORD dataSubChunkSizeM;
		DWORD totalSamplesM;
		DWORD readSamplesM;
		DWORD offsetFromBeginningM;
		DWORD leftChannelCurrentPosM;
		DWORD rightChannelCurrentPosM;
		DWORD endOfFilePosM;

	public:
		WaveParser(const string& fileName);
		~WaveParser();
		void getStandardHeader(WAVEFORMATEX *WAV);
		int getNumberOfChannels();
		int getSampleRate();
		int getBytesPerSample();
		int getDataSubChunkSize();
		int getOffsetFromBeginningtoDataChunk();
		long getAbsolutePosition();
		bool goBackToBeginOfDataChunk();
		void rewind();
		template <class T> int readWave(T** samples, int nOfSamples2Read);
		template <class T> int getChannelData(int channelNo, int nOfSamples2Read, T* samplesBuffer);
		template <class T> int getChannelData(int channelNo, int nOfSamples2Read, T* samplesBuffer, int offset);
		void printWaveFileInfo();
};

//Carry out a continous reading of the data chunk related to the desired channel.
template <class T>
inline
int WaveParser::getChannelData(int channelNo, int nOfSamples2Read, T* samplesBuffer)
{
	long currentChannelPosition;
	int readShorts = 0, totalShorts = 0;

	if((this->bitsPerSampleM/8) != sizeof(T) || channelNo > this->numChannelsM)
	{
		cout << "There was an error due to either the buffer size or the channel you are trying to read!!!" << endl;
		return ERROR;
	}

	// The position I'm about to read.
	currentChannelPosition = (channelNo == 1) ? leftChannelCurrentPosM:rightChannelCurrentPosM;

	if(currentChannelPosition != this->getAbsolutePosition())
		fseek(waveFileM, currentChannelPosition, SEEK_SET);
	
	for(int i = 0; i < nOfSamples2Read; i++)
	{
		if ((readShorts = fread(&samplesBuffer[i], (this->bitsPerSampleM/8), 1, waveFileM)) != 1)
		{
			if(ferror(waveFileM) > 0)
			{
				cout << "getChannelData: Error reading the file" << endl;
				return ERROR;
			}
			if(feof(waveFileM) > 0)
			{
				cout << "getChannelData: End of file reached" << endl;
				if(channelNo == 1)
					leftChannelCurrentPosM = this->endOfFilePosM;
				else
					rightChannelCurrentPosM = this->endOfFilePosM;

				return totalShorts;
			}
		}
		totalShorts += readShorts;
		fseek(waveFileM, (this->bitsPerSampleM/8), SEEK_CUR);
	}

	if(channelNo == 1)
		leftChannelCurrentPosM = this->getAbsolutePosition();
	else
		rightChannelCurrentPosM = this->getAbsolutePosition();

	return nOfSamples2Read;
}

// Read nOfSamples2Read samples from the data chunck related to channelNo channel beginning from the specified offset.
template <class T>
inline
int WaveParser::getChannelData(int channelNo, int nOfSamples2Read, T* samplesBuffer, int offset)
{
	long currentChannelPosition;
	int readShorts = 0, totalShorts = 0;

	if((this->bitsPerSampleM/8) != sizeof(T))
	{
		cout << "Use the proper method!!!" << endl;
		return ERROR;
	}
	
	// The position I'm about to read. It keeps the reading function on track with the desired channel.
	if(channelNo == 1)
		currentChannelPosition = this->offsetFromBeginningM + 2*offset*(this->bitsPerSampleM/8);
	else
		currentChannelPosition = this->offsetFromBeginningM + 2*offset*(this->bitsPerSampleM/8) + (this->bitsPerSampleM/8);

	if(currentChannelPosition < this->offsetFromBeginningM)
	{
		cout << "You MUST only read the contents of the data chunk!!!" << endl;
		return ERROR;
	}

	if(currentChannelPosition != this->getAbsolutePosition())
		fseek(waveFileM, currentChannelPosition, SEEK_SET);
	
	for(int i = 0; i < nOfSamples2Read; i++)
	{
		if ((readShorts = fread(&samplesBuffer[i], (this->bitsPerSampleM/8), 1, waveFileM)) != 1)
		{
			if(ferror(waveFileM) > 0)
			{
				cout << "getChannelData: Error reading the file" << endl;
				return ERROR;
			}
			if(feof(waveFileM) > 0)
			{
				cout << "getChannelData: End of file reached" << endl;
				if(channelNo == 1)
					leftChannelCurrentPosM = this->endOfFilePosM;
				else
					rightChannelCurrentPosM = this->endOfFilePosM;

				return totalShorts;
			}
		}
		totalShorts += readShorts;
		fseek(waveFileM, (this->bitsPerSampleM/8), SEEK_CUR);
	}

	return nOfSamples2Read;
}

template <class T>
inline
int WaveParser::readWave(T** samples, int nOfSamples2Read)
{
	signed char* buffer = (signed char*) malloc(4*nOfSamples2Read);
	long readBytes = 0;
	int currentPosition = 0;

	if(this->bitsPerSampleM != 16)
		throw string("This method is only used with 16 bit-wave files!!!");

	// Get current position inside the Data chunk.
	currentPosition = (this->readSamplesM*4) + this->offsetFromBeginningM;
	
	if(currentPosition != this->getAbsolutePosition())
		fseek(waveFileM, currentPosition, SEEK_SET);

	readBytes = fread(buffer, 1, 4*(nOfSamples2Read), this->waveFileM);
	if(feof(waveFileM) > 0)
		cout << "readWave: End of file reached" << endl;

	if(this->totalSamplesM && this->readSamplesM + (readBytes/4) > this->totalSamplesM)
		readBytes = 4*(this->totalSamplesM - this->readSamplesM);

	this->readSamplesM += readBytes/4;

	for(int i = 0; i < nOfSamples2Read; i++)
	{
		samples[0][i] = (T) (((buffer[4*i+1] << 8) | (((int)buffer[4*i]) & 0xFF))/32768.0);
		samples[1][i] = (T) (((buffer[4*i+3] << 8) | (((int)buffer[4*i+2]) & 0xFF))/32768.0);
	}

	free(buffer);

	return readBytes/4;
}

#endif
