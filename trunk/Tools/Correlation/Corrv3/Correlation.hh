#ifndef CORRELATION_HH
#define CORRELATION_HH

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

class Correlation
{
	private:
		Fourier *fft;
		
	public:
		Correlation();
		~Correlation();
		void doCorrelation(float data1[], float data2[], unsigned long nOfSamples, unsigned long N, float ans[]);
		void doAutoCorrelation(float data[], unsigned long nOfSamples, unsigned long N, float ans[]);
};

#endif
