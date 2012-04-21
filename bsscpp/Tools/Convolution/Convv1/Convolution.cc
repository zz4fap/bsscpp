#include "Convolution.hh"

Convolution::Convolution()
{
	this->fft = new Fourier();
}

Convolution::~Convolution()
{
	delete this->fft;
}	

void Convolution::doConvolution(float data1[], float data2[], unsigned long nOfSamples, unsigned long N, float ans[])
{
 	unsigned long no2;
	float temp[2*N], tmp;

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

