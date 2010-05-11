#include<iostream.h>
#include<string.h>
#include<stdio.h>


//#include<windows.h>

//using namespace std;

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

void main(void)
{

	//HANDLE arq;
	WAVEFORMATEX WAV;
	FILE *arquivo;

	/*arq=CreateFile("online.wav",GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if(arq==INVALID_HANDLE_VALUE)
		printf( "The file online was not opened\n" );
	else
		printf( "The file online was opened\n" );*/
	
	if( (arquivo=fopen( "pcm.wav", "rb" )) == NULL )
		printf( "The file pcm was not opened\n" );
	else
		printf( "The file pcm was opened\n" );
	
	fread(&WAV,sizeof(WAV),1,arquivo);
	

}
