#include "CrossCorrelationMatrix.hh"

CrossCorrelationMatrix::CrossCorrelationMatrix(unsigned int D, unsigned int N)
{
	this->corr = new Correlation();
	this->D = D;
	this->N = N;
}

CrossCorrelationMatrix::~CrossCorrelationMatrix()
{
	delete [] this->corr;
}

//INPUT: y1[] and y2[] vectors with N+D-1 samples.
void CrossCorrelationMatrix::getCrossCorrelationMatrix(float y1[], float y2[], float** CMM)
{
	float ans[4*this->N];
	float data1[this->N], data2[this->N];

	if(CMM == NULL)
	{
		cout << "getCrossCorrelationMatrix: ERROR!!!" << endl;
		return;
	}

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
			CMM[i][j] = N*ans[2*(j-i)];
			if(i != j)
				CMM[j][i] = N*ans[4*N - (2*(j-i))];
		}
	}
}

