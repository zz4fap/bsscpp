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

class CrossCorrelationMatrix
{
	private:
		Correlation* corr;
		unsigned int D; 
		unsigned int N;

	public:
		CrossCorrelationMatrix(unsigned int D, unsigned int N);
		~CrossCorrelationMatrix();
		void getCrossCorrelationMatrix(float y1[], float y2[], float** CMM);
};

#endif
