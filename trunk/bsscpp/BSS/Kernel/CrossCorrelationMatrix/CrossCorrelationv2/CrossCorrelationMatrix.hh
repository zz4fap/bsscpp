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

class CrossCorrelationMatrix
{
	private:
		Correlation* corr;
		unsigned int D; 
		unsigned int N;

	public:
		CrossCorrelationMatrix(unsigned int D, unsigned int N);
		~CrossCorrelationMatrix();
		template <class T>
		void getCrossCorrelationMatrix(float y1[], float y2[], Matrix<T>& CMM);
};

//INPUT: y1[] and y2[] vectors with N+D-1 samples.
template <class T>
void CrossCorrelationMatrix::getCrossCorrelationMatrix(float y1[], float y2[], Matrix<T>& CMM)
{
	float ans[4*this->N];
	float data1[this->N], data2[this->N];

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
