#include<iostream>
#include<string>
#include <string.h>

using namespace std;

typedef unsigned short WORD;
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
	char sbID[4];
	DWORD  cbSize; 
} WAVEFORMATEX;

int main(int argc, char* argv[])
{
	WAVEFORMATEX WAV;
	FILE *arquivo;
	unsigned long nBytesLidos;
	char buffer[100];

	memset(&WAV,0,sizeof(WAVEFORMATEX));
	
	//if( (arquivo = fopen( "FelipeSozinho.wav", "rb" )) == NULL )
	
	if( (arquivo = fopen( "online.wav", "rb" )) == NULL )
		cout << "The file was not opened" << endl;
	else
		cout << "The file was opened" << endl;

	if (fread(buffer, 1, 100, arquivo) != 1) 
		cout <<  "Erro de leitura" << endl;

	for(int i=0;i<100;i++)
		cout << hex << (int)buffer[i] << " ";

	cout << endl;

	for(int i=0;i<100;i++)
		cout << buffer[i] << " ";

	cout << endl;



	/*if (fread(&WAV, sizeof(WAVEFORMATEX), 1, arquivo) != 1) 
		cout <<  "Erro de leitura" << endl;

	cout << "sizeof(char): " << sizeof(char) << endl;
	cout << "sizeof(WORD): " << sizeof(WORD) << endl;
	cout << "sizeof(unsigned short): " << sizeof(unsigned short) << endl;
	cout << "sizeof(unsigned long): " << sizeof(unsigned long) << endl;
	cout << "sizeof(DWORD): " << sizeof(DWORD) << endl << endl;

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
	cout << "Sub Chunk ID= " << WAV.sbID << endl;
	cout << "cbSize= " << WAV.cbSize << endl;*/

	return 0;
}
