#ifndef FOURIER_HH
#define FOURIER_HH

#pragma once

using namespace std;

class Fourier
{
	public:
		double pi;
		unsigned long int fundamental_frequency;
		float *vector;
		Fourier(void);
		~Fourier(void);
		void getComplexArrayfromRealData(float data[], unsigned long nOfSamples, unsigned int N);
		// FFT 1D
		void ComplexFFT(float data[], unsigned long number_of_samples, unsigned int N, int sign);
};

#endif
