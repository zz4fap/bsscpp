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

void Fourier::getComplexArray(float data[], unsigned long nOfSamples, unsigned int N, int sign)
{
	//the complex array is real+complex so the array 
    	//as a size n = 2* number of complex samples
    	//real part is the data[index] and 
	//the complex part is the data[index+1]
	//new complex array of size n=2*sample_rate
	if(this->vector != NULL)
        	delete [] this->vector;

	this->vector = new float[2*N];

	if(sign == 1)
		this->getComplexArrayfromRealData(data, nOfSamples, N);
	else
	{
		for(int i = 0; i < 2*N; i++)
			this->vector[i] = data[i];
	}
}

// It fills all the odd indexes with zeros and fills the even ones with the real data from vector data.
// IN: 	data: vector with real values.
//	nOfSamples: number of samples contained in data.
//	N: Discrete Fourier Transform of N points, N must be a power of 2.
// OUT:	vector: It is a complex vector data member which gets filled with real data into the even indexes
//		and 0 (zeros) into the odd ones.
void Fourier::getComplexArrayfromRealData(float data[], unsigned long nOfSamples, unsigned int N)
{
	//variables for the fft
	unsigned long n;

	//put the real array in a complex array
	//the complex part is filled with 0's
	//the remaining vector with no data is filled with 0's
	for(n = 0; n < N; n++)
	{
		if(n < nOfSamples)
			this->vector[2*n] = data[n];
		else
			this->vector[2*n] = 0;
		this->vector[2*n+1] = 0;
	}
}

// FFT 1D
// data is a vector with complex data, i.e., data[even_index] = real_value, data[odd_index] = complex_value, 
void Fourier::ComplexFFT(unsigned int N, int sign)
{
	//variables for the fft
	unsigned long n,mmax,m,j,istep,i;
	double wtemp,wr,wpr,wpi,wi,theta,tempr,tempi;

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
}

void Fourier::calculateFundamentalFrequency(unsigned int N)
{
	//determine the fundamental frequency
	//look for the maximum absolute value in the complex array
	fundamental_frequency = 0;
	for(int i = 2; i <= N; i += 2)
	{
		if((pow(vector[i],2)+pow(vector[i+1],2)) > (pow(vector[fundamental_frequency],2)+pow(vector[fundamental_frequency+1],2)))
		{
			fundamental_frequency = i;
		}
	}

	//since the array of complex has the format [real][complex]=>[absolute value]
	//the maximum absolute value must be ajusted to half
	fundamental_frequency = (long)floor((float)fundamental_frequency/2);
}

unsigned long int Fourier::getFundamentalFrequency()
{
	if(this->vector == NULL)
	{
		cout << "vector is empty!!!" << endl;
		return ERROR;
	}

	return this->fundamental_frequency;
}

// It is the public method which calcultes the FFT using a N point complex vector, if the input vector is not a power of 2
// it creates a new vector with the next power of 2
// IN: 	data: vector with real values.
//	nOfSamples: number of samples contained in data.
//	N: Discrete Fourier Transform of N points, N must be a power of 2.
//	sign: it is the sign of the exponential used during the fft calculation. (1 the method calculates the transform and -1 it calculates the inverse)
// OUT:	vector: It is a complex vector data member which gets filled with real data into the even indexes
//		and 0 (zeros) into the odd ones.
void Fourier::fft(float data[], unsigned long nOfSamples, unsigned int N, int sign)
{	
	if(N < 2 || (N&(N-1)))
	{
		cout << "N is not a power of 2!!!" << endl;
		return;
	}

	this->getComplexArray(data, nOfSamples, N, sign);

	this->ComplexFFT(N, sign);

	if(sign == 1)
		this->calculateFundamentalFrequency(N);

	if(sign == -1)
		for(int i = 0; i < 2*N; i++)
			this->vector[i] = this->vector[i]/N;
}

void Fourier::getFFTVector(float *vector, unsigned int N)
{
	if(vector == NULL)
	{
		cout << "Vector was not initialized!!!" << endl;
		return;
	}

	memset((char*)vector, 0, 2*N);

	for(int i = 0; i < 2*N; i++)
		vector[i] = this->vector[i];
}

// FFT 1D
// data is a vector with complex data, i.e., data[even_index] = real_value, data[odd_index] = complex_value,
// INPUT: data[N | nOfSamples] = array with REAL data, no complex values here!!
// OUTPUT: vector[2*N] = array with FFT of the signal.
void Fourier::fastComplexFFT(float data[], unsigned int N, unsigned long nOfSamples, int sign, float* vector)
{
	//variables for the fft
	unsigned long n,mmax,m,j,istep,i;
	double wtemp,wr,wpr,wpi,wi,theta,tempr,tempi;

	if(N < 2 || (N&(N-1)))
	{
		cout << "fastComplexFFT: N is not a power of 2!!!" << endl;
		return;
	}

	if(sign != FFT && sign != IFFT)
	{
		cout << "fastComplexFFT: Neither FFT nor IFFT" << endl;
		return;
	}

	if(sign == FFT && data != vector)
	{
		for(int i = 0; i < N; i++)
		{
			if(i < nOfSamples)
				vector[2*i] = data[i];
			else
				vector[2*i] = 0;
			vector[2*i+1] = 0;
		}
	}

	if(sign == IFFT && data != vector)
	{
		cout << "fastComplexFFT: data != vector!!!" << endl;
		return;
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

	if(sign == IFFT)
		for(int i = 0; i < 2*N; i++)
			vector[i] = vector[i]/N;
}
