#include <math.h>
#include <iostream>
#include <string>
#include <string.h>
#include <fstream>
#include <iomanip>
#include <stdlib.h>
#include "Fourier.hh"
#include "Correlation.hh"
#include "CrossCorrelationMatrix.hh"

using namespace std;

int main(int argc, char* argv[])
{
	float** CMM = NULL;
	unsigned int D = 4, N = 8;

	float y1[] = {0.0, 0.0, 0.0, 1.0, 2.0, 3.0, 2.0, 1.0, 1.0, 3.0, 2.0};

	CMM = (float**)malloc(D*sizeof(float*));
	for(int i = 0; i < D; i++)
		CMM[i] = (float*)malloc(D*sizeof(float));

	CrossCorrelationMatrix* ccm = new CrossCorrelationMatrix(D, N);

	ccm->getCrossCorrelationMatrix(y1,y1,CMM);

	for(int i = 0; i < D; i++)
	{
		for(int j = 0; j < D; j++)
			cout << setw(6) << CMM[i][j] << " ";
		cout << endl;
	}
	cout << endl << endl;

	float y2[] = {0.0, 0.0, 0.0, 2.0, 1.0, 2.0, 3.0, 3.0, 2.0, 1.0, 2.0};

	float y3[] = {0.0, 0.0, 0.0, 5.0, 2.0, 6.0, 7.0, 8.0, 1.0, 4.0, 9.0};

	ccm->getCrossCorrelationMatrix(y2,y3,CMM);

	for(int i = 0; i < D; i++)
	{
		for(int j = 0; j < D; j++)
			cout << setw(6) << CMM[i][j] << " ";
		cout << endl;
	}

	delete [] CMM;

}
