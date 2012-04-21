#include <iostream>
#include <math.h>
#include "Fourier.hh"

using namespace std;

#define PI 4*atan((double)1)

int main(int argc, char* argv[])
{
	double sum = 0.0;

	{
		//Creates a Fourier object
		//The CFourier class has the FFT function and a couple of usefull variables
		Fourier<float> fft;

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

		for(int i = 0; i < captured_samples; i++)
		{
				double temp=(double)(2*PI*frequency*((float)i/N));		
				data[i]=((double)30*sin(temp));	
		}

		//apply the FFT to the signal
		fft.fft(data, captured_samples, N, 1);

		float *vector;
		vector = new float[2*N];
		fft.getFFTVector(vector, N);

		float absResult[N];

		for(int i = 0; i < N; i++)
		{
			absResult[i] = (float)sqrt(pow((float)vector[2*i],2) + pow((float)vector[2*i+1],2));
		}

	  	FILE * pFile;
	  	pFile = fopen ("fourier6.dat" , "wb" );

		for(int i=0;i<2*N;i++)
		{
			//cout << fft.vector[i] << endl;
			//cout << absResult[i] << endl;
	  		fwrite(&vector[i], sizeof(float) , 1, pFile);
		}

	  	fclose (pFile);

		pFile = NULL;
	  	pFile = fopen ("fourier6.dat" , "rb" );
		if(pFile <= 0 || ferror(pFile) || feof(pFile))
			cout << "ERROR!!!!!!!!!!!!!!!!!!!" << endl;
		
		float value;
		sum = 0.0;
		for(int i=0;i<2*N;i++)
		{
	  		fread(&value, sizeof(float) , 1, pFile);
			sum += (value - vector[i]);
		}

		if(sum != 0.0)
			cout << "ERROR!!!!!! - SUM: " << sum << endl;
		else
			cout << "Everything went fine!" << endl;

	  	fclose (pFile);
	}
	
	{
		long N = 8;

		float data[] = {1.0, 2.0, 3.0, 2.0, 1.0, 1.0, 3.0, 2.0};

		float ans[2*N];

		Fourier<float> *fft2 = new Fourier<float>();

		fft2->fft(data, N, N, 1);
		fft2->getFFTVector(ans, N);

		fft2->fft(ans, N, N, -1);
		memset(ans,0,2*N);
		fft2->getFFTVector(ans, N);

		sum = 0;
		for(int i = 0; i < 2*N; i+=2)
		{
			sum += data[i/2] - ans[i];
			sum += sqrt(pow(ans[i+1],2));
		}
		if(sum >= 0.000000000000001)
			cout << "IFFT: NOK!!!!!" << endl;
		else
			cout << "IFFT: OK!" << endl;

		delete fft2;
	}

	{
		long N = 8;

		float data[] = {1.0, 2.0, 3.0, 2.0, 1.0, 1.0, 3.0, 2.0};
		float ans[] = {15.0000,0,0.707107,0.707107,-4.0000,-1.0000,-0.707107,0.707107,1.0000,0,-0.707107,-0.707107,-4.0000,1.0000,0.707107,-0.707107};
		float vector[2*N];

		Fourier<float> *fft2 = new Fourier<float>();

		fft2->fft(data, N, N, 1);
		fft2->getFFTVector(vector, N);

		sum = 0;
		for(int i = 0; i < 2*N; i++)
			sum += sqrt(pow((vector[i] - ans[i]),2));

		sum = sum/2*N;

		if(sum >= 0.00007)
			cout << "FFT: NOK!!!!!" << endl;
		else
			cout << "FFT: OK!" << endl;


		delete fft2;
	}
                           

	return 0;
}

