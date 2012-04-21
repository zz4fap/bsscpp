#ifndef CROSS_CORRELATION_MATRIX_HH
#define CROSS_CORRELATION_MATRIX_HH

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
class CrossCorrelationMatrix
{
	private:
		Correlation<T>* corr;
		unsigned int D; 
		unsigned int N;

	public:
		CrossCorrelationMatrix(unsigned int D, unsigned int N);
		~CrossCorrelationMatrix();
		void getCrossCorrelationMatrix(T y1[], T y2[], Matrix<T>& CMM);
};

template <class T>
CrossCorrelationMatrix<T>::CrossCorrelationMatrix(unsigned int D, unsigned int N)
{
	this->corr = new Correlation<T>();
	this->D = D;
	this->N = N;
}

template <class T>
CrossCorrelationMatrix<T>::~CrossCorrelationMatrix()
{
	delete this->corr;
}

//INPUT: y1[] and y2[] vectors with N+D-1 samples.
template <class T>
void CrossCorrelationMatrix<T>::getCrossCorrelationMatrix(T y1[], T y2[], Matrix<T>& CMM)
{
	T ans[4*this->N];
	T data1[this->N], data2[this->N];

	for(int i = 0; i < this->D; i++) // Iterates over all rows: 0 up to (D-1)
	{
		for(int k = 0; k < this->N; k++)
		{
			data1[k] = y1[(this->D-1) + k - i];
			data2[k] = y2[(this->D-1) + k - i];
		}

		this->corr->doCorrelation(data1, data2, this->N, 2*this->N, ans);

		for(int j = i; j < this->D; j++)
		{
			CMM(i,j) = N*ans[2*(j-i)];
			if(i != j)
				CMM(j,i) = N*ans[4*N - (2*(j-i))];
		}
	}
}

#endif
