#ifndef CONVOLUTION_HH
#define CONVOLUTION_HH

#include <math.h>
#include <iostream>
#include <string>
#include <string.h>
#include <fstream>
#include <stdlib.h>
#include "Fourier.hh"

#define ERROR -1
#define FFT 1
#define IFFT -1

using namespace std;

template <class T>
class Convolution
{
	private:
		Fourier<T> *fft;
		
	public:
		Convolution();
		~Convolution();
		void doConvolution(T data1[], T data2[], unsigned long nOfSamples, unsigned long N, T ans[]);
};

template <class T>
Convolution<T>::Convolution()
{
	this->fft = new Fourier<T>();
}

template <class T>
Convolution<T>::~Convolution()
{
	delete this->fft;
}	

template <class T>
void Convolution<T>::doConvolution(T data1[], T data2[], unsigned long nOfSamples, unsigned long N, T ans[])
{
 	unsigned long no2;
	T temp[2*N], tmp;

	this->fft->fastComplexFFT(data1, N, nOfSamples, FFT, ans);
	this->fft->fastComplexFFT(data2, N, nOfSamples, FFT, temp);

	no2 = N >> 1;                                   
	for (int i = 1; i < N; i++) // Multiply to find FFT of their correlation.
	{
		tmp = ans[2*i];
	    	ans[2*i] = (ans[2*i]*temp[2*i] - ans[2*i+1]*temp[2*i+1])/no2;
	    	ans[2*i+1] = (ans[2*i+1]*temp[2*i] + tmp*temp[2*i+1])/no2;
	}

	ans[0] = ans[0]*temp[0]/no2;
	ans[1] = ans[1]*temp[1]/no2;

	this->fft->fastComplexFFT(ans, N, nOfSamples, IFFT, ans);
}

#endif
