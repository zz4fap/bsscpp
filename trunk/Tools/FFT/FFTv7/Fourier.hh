#ifndef FOURIER_HH
#define FOURIER_HH

#include <math.h>
#include <iostream>
#include <string>
#include <string.h>
#include <fstream>
#include <stdlib.h>

#pragma once

#define SWAP(a,b) tempr=(a);(a)=(b);(b)=tempr

#define ERROR -1
#define FFT 1
#define IFFT -1

using namespace std;

template <class T>
class Fourier
{
	private:
		double pi;
		unsigned long int fundamental_frequency;
		T *vector;

		void calculateFundamentalFrequency(unsigned int N);
		void getComplexArray(T data[], unsigned long nOfSamples, unsigned int N, int sign);
		void getComplexArrayfromRealData(T data[], unsigned long nOfSamples, unsigned int N);
		void ComplexFFT(unsigned int N, int sign);

	public:

		Fourier(void);
		~Fourier(void);
		void fft(T data[], unsigned long nOfSamples, unsigned int N, int sign);
		void fastComplexFFT(T data[], unsigned int N, unsigned long nOfSamples, int sign, T* vector);
		unsigned long int getFundamentalFrequency();
		void getFFTVector(T* vector, unsigned int N);
		void realft(T data[], unsigned int n, int isign);
		void fourierTransform(T data[], unsigned long nn, int isign);
		void twofft(T data1[], T data2[], T fft1[], T fft2[], unsigned long n);
};

template <class T>
Fourier<T>::Fourier(void)
{
	pi = 4*atan((double)1);
	vector = NULL;
}

template <class T>
Fourier<T>::~Fourier(void)
{
	if(vector != NULL)
		delete [] vector;
}

template <class T>
void Fourier<T>::getComplexArray(T data[], unsigned long nOfSamples, unsigned int N, int sign)
{
	//the complex array is real+complex so the array 
    	//as a size n = 2* number of complex samples
    	//real part is the data[index] and 
	//the complex part is the data[index+1]
	//new complex array of size n=2*sample_rate
	if(this->vector != NULL)
        	delete [] this->vector;

	this->vector = new T[2*N];

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
template <class T>
void Fourier<T>::getComplexArrayfromRealData(T data[], unsigned long nOfSamples, unsigned int N)
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

template <class T>
void Fourier<T>::calculateFundamentalFrequency(unsigned int N)
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
	fundamental_frequency = (long)floor((T)fundamental_frequency/2);
}

// It is the public method which calcultes the FFT using a N point complex vector, if the input vector is not a power of 2
// it creates a new vector with the next power of 2
// IN: 	data: vector with real values.
//	nOfSamples: number of samples contained in data.
//	N: Discrete Fourier Transform of N points, N must be a power of 2.
//	sign: it is the sign of the exponential used during the fft calculation. (1 the method calculates the transform and -1 it calculates the inverse)
// OUT:	vector: It is a complex vector data member which gets filled with real data into the even indexes
//		and 0 (zeros) into the odd ones.
template <class T>
void Fourier<T>::fft(T data[], unsigned long nOfSamples, unsigned int N, int sign)
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

template <class T>
void Fourier<T>::getFFTVector(T* vector, unsigned int N)
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
// INPUT: data[N | nOfSamples] = array with REAL data, no complex values here!!
// OUTPUT: vector[2*N] = array with FFT of the signal.
template <class T>
void Fourier<T>::fastComplexFFT(T data[], unsigned int N, unsigned long nOfSamples, int sign, T* vector)
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

// FFT 1D
// data is a vector with complex data, i.e., data[even_index] = real_value, data[odd_index] = complex_value.
template <class T>
void Fourier<T>::ComplexFFT(unsigned int N, int sign)
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

template <class T>
unsigned long int Fourier<T>::getFundamentalFrequency()
{
	if(this->vector == NULL)
	{
		cout << "vector is empty!!!" << endl;
		return ERROR;
	}

	return this->fundamental_frequency;
}

// Fast Fourier Transform used by the Real FFT method to calculate the FFT in a fast way.
template <class T>
void Fourier<T>::fourierTransform(T data[], unsigned long nn, int isign)
{
	unsigned long n,mmax,m,j,istep,i;
 	double wtemp,wr,wpr,wpi,wi,theta;
 	T tempr,tempi;
   
 	n=nn << 1;
 	j=1;
 	for (i=1;i<n;i+=2) 
	{
  		if (j > i) 
		{
   			SWAP(data[j],data[i]);
   			SWAP(data[j+1],data[i+1]);
  		}
  		m=n >> 1;
  		while (m >= 2 && j > m) 
		{
   			j -= m;
   			m >>= 1;
  		}
  		j += m;
 	}

 	mmax=2;
 	while (n > mmax) 
	{
  		istep=mmax << 1;
  		theta=isign*(6.28318530717959/mmax);
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
    				tempr=wr*data[j]-wi*data[j+1];
    				tempi=wr*data[j+1]+wi*data[j];
    				data[j]=data[i]-tempr;
    				data[j+1]=data[i+1]-tempi;
    				data[i] += tempr;
    				data[i+1] += tempi;
   			}
   			wr=(wtemp=wr)*wpr-wi*wpi+wr;
   			wi=wi*wpr+wtemp*wpi+wi;
  		}
  		mmax=istep;
 	}
}

// It calculates the FT of a real-valued signal.
template <class T>
void Fourier<T>::realft(T data[], unsigned int n, int isign)
{
	unsigned long i,i1,i2,i3,i4,np3;
 	T c1=0.5,c2,h1r,h1i,h2r,h2i;
 	double wr,wi,wpr,wpi,wtemp,theta;

	if(n < 2 || (n&(n-1)))
		throw "realft: N is not a power of 2!!!";
   
 	theta=3.141592653589793/(double) (n>>1);
 	if (isign == 1) 
	{
  		c2 = -0.5;
  		fourierTransform(data,n>>1,1);
 	} 
	else 
	{
  		c2=0.5;
  		theta = -theta;
 	}

 	wtemp=sin(0.5*theta);
 	wpr = -2.0*wtemp*wtemp;
 	wpi=sin(theta);
 	wr=1.0+wpr;
 	wi=wpi;
 	np3=n+3;

 	for (i=2;i<=(n>>2);i++) 
	{
		i4=1+(i3=np3-(i2=1+(i1=i+i-1)));
  		h1r=c1*(data[i1]+data[i3]);
  		h1i=c1*(data[i2]-data[i4]);
  		h2r = -c2*(data[i2]+data[i4]);
  		h2i=c2*(data[i1]-data[i3]);

  		data[i1]=h1r+wr*h2r-wi*h2i;
  		data[i2]=h1i+wr*h2i+wi*h2r;
  		data[i3]=h1r-wr*h2r+wi*h2i;
  		data[i4] = -h1i+wr*h2i+wi*h2r;
  		wr=(wtemp=wr)*wpr-wi*wpi+wr;
  		wi=wi*wpr+wtemp*wpi+wi;
 	}

 	if (isign == 1) 
	{
  		data[1] = (h1r=data[1])+data[2];
  		data[2] = h1r-data[2];
	} 
	else 
	{
  		data[1]=c1*((h1r=data[1])+data[2]);
  		data[2]=c1*(h1r-data[2]);
  		fourierTransform(data,n>>1,-1);
 	}
}

template <class T>
void Fourier<T>::twofft(T data1[], T data2[], T fft1[], T fft2[], unsigned long n)
{
 	unsigned long nn3,nn2,jj,j;
 	T rep,rem,aip,aim;
   
 	nn3=1+(nn2=2+n+n);
 	for (j=1,jj=2;j<=n;j++,jj+=2) 
	{
  		fft1[jj-1]=data1[j];
  		fft1[jj]=data2[j];
 	}

 	fourierTransform(fft1,n,1);

 	fft2[1]=fft1[2];
 	fft1[2]=fft2[2]=0.0;
 	for (j=3;j<=n+1;j+=2) 
	{
  		rep=0.5*(fft1[j]+fft1[nn2-j]);
  		rem=0.5*(fft1[j]-fft1[nn2-j]);
  		aip=0.5*(fft1[j+1]+fft1[nn3-j]);
  		aim=0.5*(fft1[j+1]-fft1[nn3-j]);
  		fft1[j]=rep;
  		fft1[j+1]=aim;
  		fft1[nn2-j]=rep;

  		fft1[nn3-j] = -aim;
  		fft2[j]=aip;
  		fft2[j+1] = -rem;
  		fft2[nn2-j]=aip;
  		fft2[nn3-j]=rem;
 	}
}

#endif
