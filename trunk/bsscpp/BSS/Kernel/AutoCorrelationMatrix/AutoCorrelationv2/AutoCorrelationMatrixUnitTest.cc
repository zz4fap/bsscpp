#include <math.h>
#include <iostream>
#include <string>
#include <string.h>
#include <fstream>
#include <iomanip>
#include <stdlib.h>
#include "Fourier.hh"
#include "Correlation.hh"
#include "AutoCorrelationMatrix.hh"
#include "Matrix.hh"

using namespace std;

int main(int argc, char* argv[])
{
	unsigned int D, N;

	AutoCorrelationMatrix* ccm = NULL;

	{
		D = 4;
		N = 8;

		ccm = new AutoCorrelationMatrix(D, N);

		Matrix<float> CMM(D,D);

		float y1[] = {0.0, 0.0, 0.0, 1.0, 2.0, 3.0, 2.0, 1.0, 1.0, 3.0, 2.0};

		ccm->getAutoCorrelationMatrix(y1,y1,CMM);

		for(int i = 0; i < D; i++)
		{
			for(int j = 0; j < D; j++)
				cout << setw(6) << CMM(i,j) << " ";
			cout << endl;
		}
		cout << endl << endl;

		delete ccm;
	}

	{
		D = 4;
		N = 8;

		ccm = new AutoCorrelationMatrix(D, N);

		Matrix<float> CMM(D,D);

		float y2[] = {0.0, 0.0, 0.0, 5.0, 2.0, 6.0, 7.0, 8.0, 1.0, 4.0, 9.0};

		ccm->getAutoCorrelationMatrix(y2,y2,CMM);

		for(int i = 0; i < D; i++)
		{
			for(int j = 0; j < D; j++)
				cout << setw(6) << CMM(i,j) << " ";
			cout << endl;
		}

		delete ccm;
	}
}
