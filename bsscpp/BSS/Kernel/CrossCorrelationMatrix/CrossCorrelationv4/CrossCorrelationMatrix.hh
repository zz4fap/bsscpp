#ifndef CROSS_CORRELATION_MATRIX_HH
#define CROSS_CORRELATION_MATRIX_HH

#include <math.h>
#include <iostream>
#include <string>
#include <string.h>
#include <stdio.h>
#include <fstream>
#include <stdlib.h>
#include <memory>
#include <new>
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
	unsigned int tempN = 2*N;

	T ans[4*tempN];

	T* data1 = new T[tempN];
	T* data2 = new T[tempN];

	memset(data1,0,sizeof(T)*tempN);
	memset(data2,0,sizeof(T)*tempN);

	for(int i = 0; i < (N+D-1); i++)
		data1[(N-(D-1))+i] = y1[i];

	for(int i = 0; i < this->D; i++) // Iterate over all rows: 0 up to (D-1)
	{
		for(int k = 0; k < this->N; k++)
			data2[N+k] = y2[k+i];

		this->corr->doCorrelation(data1, data2, tempN, 2*tempN, ans);

		for(int j = 0; j < this->D; j++)
		{
			if(j == 0)
				CMM(j,(D-1)-i) = tempN*ans[0];
			else
				CMM(j,(D-1)-i) = tempN*ans[4*tempN-2*j];
		}
	}

	delete [] data1;
	delete [] data2;
}

#endif
