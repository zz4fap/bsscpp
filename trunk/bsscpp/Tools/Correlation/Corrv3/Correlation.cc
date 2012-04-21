#include "Correlation.hh"

Correlation::Correlation()
{
	this->fft = new Fourier();
}

Correlation::~Correlation()
{
	delete fft;
}

// Computes the correlation of two real data sets data1[0..N-1] and data2[0..N-1] (including
// any user-supplied zero padding). N must be an integer power of 2. The answer is returned in
// ans[0..N-1] stored in wraparound order, i.e., correlations at increasingly negative lags are in
// ans[N-1] on down to ans[N/2], while correlations at increasingly positive lags are in ans[0]
// (zero lag) on up to ans[(N/2)-1]. Sign convention of this routine: if data1 lags data2, i.e., is
// shifted to the right of it, then ans will show a peak at positive lags.
// INPUT: data1[] - N data values
// INPUT: data2[] - N data values
// INPUT: ans[] - N data values
void Correlation::doCorrelation(float data1[], float data2[], unsigned long nOfSamples, unsigned long N, float ans[])
{
 	unsigned long no2;
	float temp[2*N], tmp;

	this->fft->fastComplexFFT(data1, N, nOfSamples, FFT, ans);
	this->fft->fastComplexFFT(data2, N, nOfSamples, FFT, temp);

	no2 = N >> 1;                                   
	for (int i = 1; i < N; i++) // Multiply to find FFT of their correlation.
	{
		tmp = ans[2*i];
	    	ans[2*i] = (ans[2*i]*temp[2*i] + ans[2*i+1]*temp[2*i+1])/no2;
	    	ans[2*i+1] = (ans[2*i+1]*temp[2*i] - tmp*temp[2*i+1])/no2;
	}

	ans[0] = ans[0]*temp[0]/no2;
	ans[1] = ans[1]*temp[1]/no2;

	this->fft->fastComplexFFT(ans, N, nOfSamples, IFFT, ans);
}

// Computes the correlation of two real data sets data1[0..N-1] and data2[0..N-1] (including
// any user-supplied zero padding). N must be an integer power of 2. The answer is returned in
// ans[0..N-1] stored in wraparound order, i.e., correlations at increasingly negative lags are in
// ans[N-1] on down to ans[N/2], while correlations at increasingly positive lags are in ans[0]
// (zero lag) on up to ans[N/2-1]. Sign convention of this routine: if data1 lags data2, i.e., is
// shifted to the right of it, then ans will show a peak at positive lags.
void Correlation::doAutoCorrelation(float data[], unsigned long nOfSamples, unsigned long N, float ans[])
{
 	unsigned long no2;
	float tmp;

	this->fft->fastComplexFFT(data, N, nOfSamples, FFT, ans);

	no2 = N >> 1;                                   
	for (int i = 1; i < N; i++) // Multiply to find FFT of their correlation.
	{
		tmp = ans[2*i];
	    	ans[2*i] = (ans[2*i]*ans[2*i] + ans[2*i+1]*ans[2*i+1])/no2;
	    	ans[2*i+1] = (ans[2*i+1]*tmp - tmp*ans[2*i+1])/no2;
	}

	ans[0] = ans[0]*ans[0]/no2;
	ans[1] = ans[1]*ans[1]/no2;

	this->fft->fastComplexFFT(ans, N, nOfSamples, IFFT, ans);
}

