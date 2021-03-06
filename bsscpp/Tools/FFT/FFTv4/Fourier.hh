#ifndef FOURIER_HH
#define FOURIER_HH

#pragma once

#define ERROR -1

using namespace std;

class Fourier
{
	private:
		double pi;
		unsigned long int fundamental_frequency;
		float *vector;

		void calculateFundamentalFrequency(unsigned int N);
		void getComplexArray(float data[], unsigned long nOfSamples, unsigned int N, int sign);
		void getComplexArrayfromRealData(float data[], unsigned long nOfSamples, unsigned int N);
		void ComplexFFT(unsigned int N, int sign);

	public:

		Fourier(void);
		~Fourier(void);
		void fft(float data[], unsigned long nOfSamples, unsigned int N, int sign);
		unsigned long int getFundamentalFrequency();
		void getFFTVector(float *vector, unsigned int N);
};

#endif
