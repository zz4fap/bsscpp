#include <math.h>
#include <iostream>
#include <string>
#include <string.h>
#include <fstream>
#include <stdlib.h>
#include "Fourier.hh"

#define SWAP(a,b) tempr=(a);(a)=(b);(b)=tempr

Fourier::Fourier(void)
{
	pi = 4*atan((double)1);
	vector = NULL;
}

Fourier::~Fourier(void)
{
	if(vector != NULL)
		delete [] vector;
}

void Fourier::getComplexArrayfromRealData(float data[], unsigned long nOfSamples, unsigned int N)
{
	//variables for the fft
	unsigned long n;

	//the complex array is real+complex so the array 
    	//as a size n = 2* number of complex samples
    	//real part is the data[index] and 
	//the complex part is the data[index+1]

	//new complex array of size n=2*sample_rate
	if(vector != NULL)
        	delete [] vector;

	vector = new float[2*N];

	//put the real array in a complex array
	//the complex part is filled with 0's
	//the remaining vector with no data is filled with 0's
	for(n = 0; n < N; n++)
	{
		if(n < nOfSamples)
			vector[2*n] = data[n];
		else
			vector[2*n] = 0;
		vector[2*n+1] = 0;
	}
}

// FFT 1D
// data is a vector with real data.
void Fourier::ComplexFFT(float data[], unsigned long number_of_samples, unsigned int N, int sign)
{

	//variables for the fft
	unsigned long n,mmax,m,j,istep,i;
	double wtemp,wr,wpr,wpi,wi,theta,tempr,tempi;

	if(N < 2 || (N&(N-1)))
	{
		cout << "N is not a power of 2!!!" << endl;
		return;
	}

	//the complex array is real+complex so the array 
    	//as a size n = 2* number of complex samples
    	//real part is the data[index] and 
	//the complex part is the data[index+1]

	//new complex array of size n=2*sample_rate
	if(vector != NULL)
        	delete [] vector;

	vector = new float[2*N];

	//put the real array in a complex array
	//the complex part is filled with 0's
	//the remaining vector with no data is filled with 0's
	for(n = 0; n < N; n++)
	{
		if(n < number_of_samples)
			vector[2*n] = data[n];
		else
			vector[2*n] = 0;
		vector[2*n+1] = 0;
	}

	//binary inversion (note that the indexes 
    	//start from 0 which means that the
    	//real part of the complex is on the even-indexes 
    	//and the complex part is on the odd-indexes)
	n = N << 1;
	j = 0;
	for (i=0;i<n/2;i+=2) {
		if (j > i) {
			SWAP(vector[j],vector[i]);
			SWAP(vector[j+1],vector[i+1]);
			if((j/2)<(n/4)){
				SWAP(vector[(n-(i+2))],vector[(n-(j+2))]);
				SWAP(vector[(n-(i+2))+1],vector[(n-(j+2))+1]);
			}
		}
		m=n >> 1;
		while (m >= 2 && j >= m) {
			j -= m;
			m >>= 1;
		}
		j += m;
	}
	//end of the bit-reversed order algorithm

	//Danielson-Lanzcos routine
	mmax=2;
	while (n > mmax) 
	{
		istep=mmax << 1;
		theta=sign*(2*pi/mmax);
		wtemp=sin(0.5*theta);
		wpr = -2.0*wtemp*wtemp;
		wpi=sin(theta);
		wr=1.0;
		wi=0.0;
		for (m=1;m<mmax;m+=2) 
		{
			for (i=m;i<=n;i+=istep) 
			{
				j=i+mmax;
				tempr=wr*vector[j-1]-wi*vector[j];
				tempi=wr*vector[j]+wi*vector[j-1];
				vector[j-1]=vector[i-1]-tempr;
				vector[j]=vector[i]-tempi;
				vector[i-1] += tempr;
				vector[i] += tempi;
			}
			wr=(wtemp=wr)*wpr-wi*wpi+wr;
			wi=wi*wpr+wtemp*wpi+wi;
		}
		mmax=istep;
	}
	//end of the algorithm
	
	//determine the fundamental frequency
	//look for the maximum absolute value in the complex array
	fundamental_frequency = 0;
	for(i = 2; i <= N; i += 2)
	{
		if((pow(vector[i],2)+pow(vector[i+1],2)) > (pow(vector[fundamental_frequency],2)+pow(vector[fundamental_frequency+1],2)))
		{
			fundamental_frequency=i;
		}
	}

	//since the array of complex has the format [real][complex]=>[absolute value]
	//the maximum absolute value must be ajusted to half
	fundamental_frequency=(long)floor((float)fundamental_frequency/2);

}
