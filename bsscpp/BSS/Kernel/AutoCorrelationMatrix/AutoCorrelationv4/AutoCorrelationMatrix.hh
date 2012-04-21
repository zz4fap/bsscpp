#ifndef AUTO_CORRELATION_MATRIX_HH
#define AUTO_CORRELATION_MATRIX_HH

#include <math.h>
#include <iostream>
#include <string>
#include <string.h>
#include <fstream>
#include <stdlib.h>
#include "Fourier.hh"
#include "Correlation.hh"
#include "Matrix.hh"

template <class T>
class AutoCorrelationMatrix
{
	private:
		Correlation<T>* corr;
		unsigned int D; 
		unsigned int N;

	public:
		AutoCorrelationMatrix(unsigned int D, unsigned int N);
		~AutoCorrelationMatrix();
		void getAutoCorrelationMatrix(T y1[], Matrix<T>& CMM);
		void getOutputPowerMatrix(T y1[], T factor, Matrix<T>& CMM);
};

template <class T>
AutoCorrelationMatrix<T>::AutoCorrelationMatrix(unsigned int D, unsigned int N)
{
	this->corr = new Correlation<T>();
	this->D = D;
	this->N = N;
}

template <class T>
AutoCorrelationMatrix<T>::~AutoCorrelationMatrix()
{
	delete this->corr;
}

//INPUT: y1[] is a vector with N+D-1 samples.
template <class T>
void AutoCorrelationMatrix<T>::getAutoCorrelationMatrix(T y1[], Matrix<T>& CMM)
{
	T ans[4*this->N];
	T data1[this->N];

	for(int i = 0; i < this->D; i++) // Iterates over all rows: 0 up to (D-1)
	{
		for(int k = 0; k < this->N; k++)
			data1[k] = y1[(this->D-1) + k - i];

		this->corr->doAutoCorrelation(data1, this->N, 2*this->N, ans);

		for(int j = i; j < this->D; j++)
		{
			CMM(i,j) = N*ans[2*(j-i)];
			if(i != j)
				CMM(j,i) = N*ans[2*(j-i)];
		}
	}
}

// INPUT: 	y1[] is a vector with N+D-1 samples.
//		factor is the regularization factor.
//		CMM is the Output Power Matrix.
template <class T>
void AutoCorrelationMatrix<T>::getOutputPowerMatrix(T y1[], T factor, Matrix<T>& CMM)
{
	T res = 0, regFactor = 0;
	
	for(int i = 0; i < this->N; i++)
		res += (y1[D-1+i]*y1[D-1+i]);

	Eye<T> eye(D,D);
	CMM = eye * res;

	res *= (T)-1;
	regFactor = factor*exp(res);

	CMM = CMM + eye*regFactor;
}

#endif
