#include <iostream>
#include <math.h>
#include "fourier1.hh"

using namespace std;

#define pi 4*atan((double)1)

int main(int argc, char* argv[])
{
	//sample rate of the signal (must be 2^n)
	long N = 8192; //2^13

	//number of samples you want to send for processing in the fft (any)
	//for example 100 samples
	long captured_samples = 500;

	//frequency of the signal (has to be smaller than sample_rate/2)
	//for example 46
	int frequency = 250;

	//example of a sin signal(you can try to add some more signals to see the
	//fourier change
	float data[captured_samples];
	memset(data,0,captured_samples);
	for(int i=0; i<captured_samples; i++)
	{
		double temp = (double)(6.28318530717959*frequency*((float)i/N));		
		data[i] = ((double)30*sin(temp));
	}

	//put the real array in a complex array
	//the complex part is filled with 0's
	//the remaining vector with no data is filled with 0's
	float *vector = new float[2*N];
	memset(vector,0,2*N);
	for(int n = 1; n < 2*N; n+=2)
	{
		if((n-1) < (2*captured_samples-1))
			vector[n] = data[(n-1)/2];
		else
			vector[n] = 0;
		vector[n+1] = 0;
	}

	//for(int i=997; i<1010; i++)
	//	cout << vector[i] << endl;

	//for(int i=0; i<502; i++)
	//	cout << vector[i] << endl;

	//apply the FFT to the signal
	four1(vector, N, 1);

	float absResult[N];

	for(int i=1; i<N;i++)
	{
		absResult[i] = (float)sqrt(pow((float)vector[i],2) + pow((float)vector[i+1],2));
	}

  	FILE * pFile;
  	pFile = fopen ("fourier2.dat" , "wb" );

	for(int i = 0; i < (2*N)+1; i++)
	{
		//cout << vector[i] << endl;
		//cout << absResult[i] << endl;
  		fwrite (&vector[i], sizeof(float) , 1, pFile);
	}

	cout << endl << endl;

  	fclose (pFile);

  	pFile = fopen ("fourier2.dat" , "rb" );
	float value;
	for(int i=0;i<2*N+1;i++)
	{
  		fread(&value, sizeof(float) , 1, pFile);
		cout << value << endl;
	}

  	fclose (pFile);

	delete [] vector;
	return 0;
}

