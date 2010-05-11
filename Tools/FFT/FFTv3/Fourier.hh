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

		void calculateFundamentalFrequency(unsigned int N);
		void getComplexArrayfromRealData(float data[], unsigned long nOfSamples, unsigned int N);
		void ComplexFFT(unsigned int N, int sign);

	public:
		float *vector;

		Fourier(void);
		~Fourier(void);
		void fft(float data[], unsigned long nOfSamples, unsigned int N, int sign);
		unsigned long int getFundamentalFrequency();
};

#endif
