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

class AutoCorrelationMatrix
{
	private:
		Correlation* corr;
		unsigned int D; 
		unsigned int N;

	public:
		AutoCorrelationMatrix(unsigned int D, unsigned int N);
		~AutoCorrelationMatrix();
		void getAutoCorrelationMatrix(float y1[], float y2[], float** CMM);
};

#endif
