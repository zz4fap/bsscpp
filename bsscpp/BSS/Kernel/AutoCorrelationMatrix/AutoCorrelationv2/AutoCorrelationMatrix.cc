#include "AutoCorrelationMatrix.hh"

AutoCorrelationMatrix::AutoCorrelationMatrix(unsigned int D, unsigned int N)
{
	this->corr = new Correlation();
	this->D = D;
	this->N = N;
}

AutoCorrelationMatrix::~AutoCorrelationMatrix()
{
	delete this->corr;
}

