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

class AutoCorrelationMatrix
{
	private:
		Correlation* corr;
		unsigned int D; 
		unsigned int N;

	public:
		AutoCorrelationMatrix(unsigned int D, unsigned int N);
		~AutoCorrelationMatrix();
		template <class T>
		void getAutoCorrelationMatrix(float y1[], Matrix<T>& CMM);
		template <class T>
		void getOutputPowerMatrix(float y1[], T factor, Matrix<T>& CMM);
};

//INPUT: y1[] is a vector with N+D-1 samples.
template <class T>
void AutoCorrelationMatrix::getAutoCorrelationMatrix(float y1[], Matrix<T>& CMM)
{
	float ans[4*this->N];
	float data1[this->N];

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
void AutoCorrelationMatrix::getOutputPowerMatrix(float y1[], T factor, Matrix<T>& CMM)
{
	T res = 0, regFactor = 0;
	
	for(int i = 0; i < this->N; i++)
		res += (y1[D-1+i]*y1[D-1+i]);

	Eye<T> eye(D,D);
	CMM = eye * res;

	res *= -1;
	regFactor = factor*exp(res);

	CMM = CMM + eye*regFactor;
}

#endif
