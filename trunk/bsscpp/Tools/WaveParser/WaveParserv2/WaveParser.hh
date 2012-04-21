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

	public:
		WaveParser(const string& fileName);
		~WaveParser();
		void getStandardHeader(WAVEFORMATEX *WAV);
		int getNumberOfChannels();
		int getSampleRate();
		int getBytesPerSample();
		int getDataSubChunkSize();
		int getOffsetFromBeginning();
		long getCurrentPosition();
		template <class T> int getChannelData(int channelNo, int nOfSamples2Read, T* samplesBuffer);
		template <class T> int getChannelData(int channelNo, int nOfSamples2Read, T* samplesBuffer, int offset);
		void printWaveFileInfo();
};

template <class T>
inline
int WaveParser::getChannelData(int channelNo, int nOfSamples2Read, T* samplesBuffer)
{
	long positionBeforeReading;

	if((this->bitsPerSampleM/8) != sizeof(T))
	{
		cout << "Use the proper method!!!" << endl;
		return ERROR;
	}

	if(channelNo == 2)
		fseek(waveFileM, (this->bitsPerSampleM/8), SEEK_CUR);

	positionBeforeReading = this->getCurrentPosition();

	for(int i = 0; i < nOfSamples2Read; i++)
	{
		if (fread(&samplesBuffer[i], (this->bitsPerSampleM/8), 1, waveFileM) != 1)
		{
			cout << "getChannelData: Error" << endl;
			return ERROR;
		}
		fseek(waveFileM, (this->bitsPerSampleM/8), SEEK_CUR);
	}
	
	/*if(channelNo == 1)
		leftChannelCurrentPosM = this->getCurrentPosition();
		rightChannelCurrentPosM = positionBeforeReading;
	else
		leftChannelCurrentPosM = positionBeforeReading;
		rightChannelCurrentPosM = this->getCurrentPosition();*/

	return nOfSamples2Read;
}

template <class T>
inline
int WaveParser::getChannelData(int channelNo, int nOfSamples2Read, T* samplesBuffer, int offset)
{

}

#endif
