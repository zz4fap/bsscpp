#include<iostream.h>
#include<string.h>
#include<stdio.h>
#include<windows.h>

void main(void)
{

	HANDLE arq;
	WAVEFORMATEX WAV;
	unsigned long nBytesLidos;

	arq=CreateFile("c:\\C++\\locucoes\\01.wav",GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if(arq==INVALID_HANDLE_VALUE)
		printf( "The file was not opened\n" );
	else
		printf( "The file was opened\n" );

	//posicionando o ponteiro no inicio do cabecalho
	SetFilePointer(arq,20,NULL,FILE_BEGIN);
	
	
	if(ReadFile(arq,&WAV,sizeof(WAVEFORMATEX),&nBytesLidos,NULL)==false)
		printf( "Erro de leitura\n" );

	if(WAV.wFormatTag==1)
		cout << "wFormatTag= PCM Format" << endl;
	else
		cout << "wFormatTag= ADPCM Format" << endl;

	cout << "nChannels= " << WAV.nChannels << endl;
	cout << "nSamplesPerSec= " << WAV.nSamplesPerSec << endl;
	cout << "nAvgBytesPerSec= " << WAV.nAvgBytesPerSec << endl;
	cout << "nBlockAlign= " << WAV.nBlockAlign << endl;
	cout <<	"wBitsPerSample= " << WAV.wBitsPerSample << endl;
	cout << "cbSize= " << WAV.cbSize << endl;

}