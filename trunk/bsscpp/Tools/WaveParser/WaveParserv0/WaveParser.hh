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
		int getChannelData16Bits(int channelNo, int nOfSamples2Read, short* samplesBuffer);
		int getChannelData(int channelNo, int nOfSamples2Read, void* samplesBuffer, int offset);
		void printWaveFileInfo();
};

#endif
