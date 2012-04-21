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

class Convolution
{
	private:
		Fourier *fft;
		
	public:
		Convolution();
		~Convolution();
		void doConvolution(float data1[], float data2[], unsigned long nOfSamples, unsigned long N, float ans[]);
};

#endif
