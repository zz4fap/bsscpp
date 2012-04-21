#include "CrossCorrelationMatrix.hh"

CrossCorrelationMatrix::CrossCorrelationMatrix(unsigned int D, unsigned int N)
{
	this->corr = new Correlation();
	this->D = D;
	this->N = N;
}

CrossCorrelationMatrix::~CrossCorrelationMatrix()
{
	delete this->corr;
}

