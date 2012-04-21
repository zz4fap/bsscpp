#ifndef WAV_PARSER_HH
#define WAV_PARSER_HH

#include <iostream>
#include <string>
#include <string.h>
#include <fstream>

using namespace std;

#define ERROR -1

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
		bool findDataSubChunk();
		bool openWaveFile(const string& fileName);

		FILE* waveFileM;

		WAVEFORMATEX WAV;
		DATACHUNKINFO DINFO;

		DWORD sampleRateM;
		WORD  numChannelsM; 
		WORD  bitsPerSampleM; 
		DWORD dataSubChunkSizeM;
		DWORD offsetFromBeginningM;
		DWORD leftChannelCurrentPosM;
		DWORD rightChannelCurrentPosM;

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

	if((this->bitsPerSampleM/8) != sizeof(T))
	{
		cout << "Use the proper method!!!" << endl;
		return ERROR;
	}

	// The position I'm about to read.
	currentChannelPosition = (channelNo == 1) ? leftChannelCurrentPosM:rightChannelCurrentPosM;

	if(currentChannelPosition != this->getAbsolutePosition())
		fseek(waveFileM, currentChannelPosition, SEEK_SET);
	
	for(int i = 0; i < nOfSamples2Read; i++)
	{
		if (fread(&samplesBuffer[i], (this->bitsPerSampleM/8), 1, waveFileM) != 1)
		{
			cout << "getChannelData: Error" << endl;
			return ERROR;
		}
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
		if (fread(&samplesBuffer[i], (this->bitsPerSampleM/8), 1, waveFileM) != 1)
		{
			cout << "getChannelData: Error" << endl;
			return ERROR;
		}
		fseek(waveFileM, (this->bitsPerSampleM/8), SEEK_CUR);
	}

	return nOfSamples2Read;
}

#endif
