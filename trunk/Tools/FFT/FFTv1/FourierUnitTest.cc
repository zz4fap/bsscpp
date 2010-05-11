#include <iostream>
#include <math.h>
#include "Fourier.hh"

using namespace std;

int main(int argc, char* argv[])
{
	//Creates a Fourier object
	//The CFourier class has the FFT function and a couple of usefull variables
	Fourier fft;

	//sample rate of the signal (must be 2^n)
	long N = 8192; //2^13

	//number of samples you want to send for processing in the fft (any)
	//for example 100 samples
	long captured_samples = 500;

	//frequency of the signal (has to be smaller than sample_rate/2)
	//for example 46
	int frequency = 250;

	float data[5000];

	//example of a sin signal(you can try to add some more signals to see the
	//fourier change

	for(int i=0; i<captured_samples; i++)
	{
			double temp=(double)(2*fft.pi*frequency*((float)i/N));		
			data[i]=((double)30*sin(temp));	
	}

	//apply the FFT to the signal
	fft.ComplexFFT(data, captured_samples, N, 1);

	float absResult[N];

	for(int i=0; i<N;i++)
	{
		absResult[i] = (float)sqrt(pow((float)fft.vector[2*i],2) + pow((float)fft.vector[2*i+1],2));
	}

  	FILE * pFile;
  	pFile = fopen ("fourier1.dat" , "wb" );

	for(int i=0;i<2*N;i++)
	{
		cout << fft.vector[i] << endl;
		//cout << absResult[i] << endl;
  		fwrite (&fft.vector[i], sizeof(float) , 1, pFile);
	}

	cout << endl << endl;

  	fclose (pFile);

  	pFile = fopen ("fourier1.dat" , "rb" );
	float value;
	for(int i=0;i<2*N;i++)
	{
  		fread(&value, sizeof(float) , 1, pFile);
		//cout << value << endl;
	}

  	fclose (pFile);

	return 0;
}

