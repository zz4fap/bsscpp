#include<iostream.h>
#include<string.h>
#include<stdio.h>

typedef unsigned int WORD;
typedef unsigned long DWORD;

typedef struct 
{ 
	char rID[4];
	DWORD rLen;
	char wID[4];
	char fID[4];
	DWORD pcm_header_len;
	WORD  wFormatTag; 
	WORD  nChannels; 
	DWORD nSamplesPerSec; 
	DWORD nAvgBytesPerSec; 
	WORD  nBlockAlign; 
	WORD  wBitsPerSample; 
	WORD  cbSize; 
} WAVEFORMATEX;

#include<io.h>

void main(void)
{

	WAVEFORMATEX WAV;
	FILE *arquivo;
	unsigned long nBytesLidos;
	
	if( (arquivo=fopen( "online.wav", "r" )) == NULL )
		printf( "The file pcm was not opened\n" );
	else
		printf( "The file online was opened\n" );
	
	if(ReadFile(arquivo,&WAV,sizeof(WAVEFORMATEX),&nBytesLidos,NULL)==false)
		printf( "Erro de leitura\n" );

	cout << "rID= " << WAV.rID << endl;
	cout << "rLen= " << WAV.rLen << endl;
	cout << "wID= " << WAV.wID << endl;
	cout << "fID= " << WAV.fID << endl;
	cout << "pcm_header_len= " << WAV.pcm_header_len << endl;
	cout << "wFormatTag= " << WAV.wFormatTag << endl;
	cout << "nChannels= " << WAV.nChannels << endl;
	cout << "nSamplesPerSec= " << WAV.nSamplesPerSec << endl;
	cout << "nAvgBytesPerSec= " << WAV.nAvgBytesPerSec << endl;
	cout << "nBlockAlign= " << WAV.nBlockAlign << endl;
	cout <<	"wBitsPerSample= " << WAV.wBitsPerSample << endl;
	cout << "cbSize= " << WAV.cbSize << endl;

}
