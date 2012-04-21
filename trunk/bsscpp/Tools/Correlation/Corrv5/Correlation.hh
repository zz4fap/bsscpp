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

template <class T>
class Correlation
{
	struct TreadData
	{
		T* data;
		unsigned long nOfSamples;
		unsigned long N;
		T* ans;
		Correlation<T>* corr;
	};

	private:
		pthread_mutex_t mutex;

		Fourier<T> *fft;
		static void *start_thread(void *obj);
		
	public:
		Correlation();
		~Correlation();
		void doCorrelation(T data1[], T data2[], unsigned long nOfSamples, unsigned long N, T ans[]);
		void doCorrelationv2(T data1[], T data2[], unsigned long nOfSamples, unsigned long N, T ans[]);
		void doAutoCorrelation(T data[], unsigned long nOfSamples, unsigned long N, T ans[]);
};

template <class T>
Correlation<T>::Correlation()
{
	this->fft = new Fourier<T>();

	pthread_mutex_init(&mutex, NULL);
}

template <class T>
Correlation<T>::~Correlation()
{
	delete fft;

	pthread_mutex_destroy(&mutex);
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
template <class T>
void Correlation<T>::doCorrelationv2(T data1[], T data2[], unsigned long nOfSamples, unsigned long N, T ans[])
{
 	unsigned long no2;
	T temp[2*N], tmp;

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
// (zero lag) on up to ans[(N/2)-1]. Sign convention of this routine: if data1 lags data2, i.e., is
// shifted to the right of it, then ans will show a peak at positive lags.
// INPUT: data1[] - N data values
// INPUT: data2[] - N data values
// INPUT: ans[] - N data values
template <class T>
void Correlation<T>::doCorrelation(T data1[], T data2[], unsigned long nOfSamples, unsigned long N, T ans[])
{
	char str[100];
 	unsigned long no2;
	int rc;
	T temp[2*N], tmp;
	TreadData threadData1, threadData2;
	pthread_t threads[2];

	threadData1.nOfSamples = threadData2.nOfSamples = nOfSamples;
	threadData1.N = threadData2.N = N;
	threadData1.corr = threadData2.corr = this;

	threadData1.data = data1;
	threadData1.ans = ans;
      	rc = pthread_create(&threads[0], NULL, &Correlation<T>::start_thread, &threadData1);
      	if (rc)
	{
         	sprintf(str, "ERROR; return code from pthread_create() is %d\n", rc);
		throw str;
      	}

	threadData2.data = data2;
	threadData2.ans = temp;
      	rc = pthread_create(&threads[1], NULL, &Correlation<T>::start_thread, &threadData2);
      	if (rc)
	{
         	sprintf(str, "ERROR; return code from pthread_create() is %d\n", rc);
		throw str;
      	}

	// Wait for both threads to terminate the fft.
   	for(int j = 0; j < 2; j++)
   	{
      		pthread_join(threads[j], NULL); 
   	}

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

// This is the static class function that serves as a C style function pointer
// for the pthread_create call
template <class T>
void *Correlation<T>::start_thread(void *obj)
{	
	TreadData* td = (TreadData*)obj;

	td->corr->fft->fastComplexFFT(td->data, td->N, td->nOfSamples, FFT, td->ans);

	pthread_exit(NULL);
}

// Computes the correlation of two real data sets data1[0..N-1] and data2[0..N-1] (including
// any user-supplied zero padding). N must be an integer power of 2. The answer is returned in
// ans[0..N-1] stored in wraparound order, i.e., correlations at increasingly negative lags are in
// ans[N-1] on down to ans[N/2], while correlations at increasingly positive lags are in ans[0]
// (zero lag) on up to ans[N/2-1]. Sign convention of this routine: if data1 lags data2, i.e., is
// shifted to the right of it, then ans will show a peak at positive lags.
template <class T>
void Correlation<T>::doAutoCorrelation(T data[], unsigned long nOfSamples, unsigned long N, T ans[])
{
 	unsigned long no2;
	T tmp;

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

#endif

