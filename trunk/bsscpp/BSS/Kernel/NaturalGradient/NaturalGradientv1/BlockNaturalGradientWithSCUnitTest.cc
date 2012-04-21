#include <string.h>
#include <iostream>
#include <string>
#include <fstream>
#include <stdlib.h>
#include <vector>
#include <math.h>
#include "MatrixOperations.hh"
#include "BlockNaturalGradientWithSC.hh"

using namespace std;

typedef std::vector< Matrix<float> > VectorMatrix;

void allocate(float ***y, UINT rows, UINT columns)
{
	(*y) = (float **)malloc(rows*sizeof(float*));
	for(int i=0; i<rows; i++)
		(*y)[i] = (float*)malloc(columns*sizeof(float));
}

void deallocate(float ***y, UINT rows)
{
	if((*y))
	{
		for(int i = 0; i < rows; i++)
			free((*y)[i]);

		free((*y));
	}
}

enum InitializationType
{
	SHIFTED,
	NON_SHIFTED
};

void initialiseWMatrix(UINT L, UINT D, UINT numOfChannels, InitializationType initializationType, VectorMatrix** W)
{
	int inc = 0;
	(*W) = new vector< Matrix<float> >(numOfChannels, Matrix<float>(2*L,D));

	switch(initializationType)
	{
		case SHIFTED:
			// Inicializa somente as matrizes W11 e W22 com um filtro passa tudo deslocado de ceil(L/2)amostras(impulso unitario)
			inc = 0;
			for(int m = 0; m < (int)sqrt(numOfChannels); m++)
			{			
				for(int i = 0; i < D; i++)
				{
					(**W)[inc]((ceil(L/2)+(i-1)), i) = 1;
				}
				inc = inc + (int)sqrt(numOfChannels) + 1;
			}
			break;
	    	case NON_SHIFTED:
			// Inicializa somente as matrizes W11 e W22 com um filtro passa tudo(impulso unitario)
			inc = 0;
			for(int m = 0; m < (int)sqrt(numOfChannels); m++)
			{			
				for(int i = 0; i < D; i++)
				{
					(**W)[inc](i,i) = 1;
				}
				inc = inc + (int)sqrt(numOfChannels) + 1;
			}
			break;
		default:
			std::cout << "Unknown Initialization Type" << std::endl;
	}
}

float testInitialiseWMatrix(UINT L, UINT D, UINT numOfChannels, VectorMatrix* Wm)
{
	float sum = 0;

	for(int a=0; a<numOfChannels; a++)
	{
		for(int i=0; i<2*L; i++)
		{
			for(int j=0; j<D; j++)
			{
				
				if(a == 0 || a == 3)
				{
					if(i == j)
						sum += ((*Wm)[a](i,j) - 1);
					else
						sum += (*Wm)[a](i,j);
				}
				else
					sum += (*Wm)[a](i,j);
			}
		}
	}

	return sum;
}

int main()
{
	// L = D = 128
	UINT L = 128, D = 128, N = 256, numOfChannels = 4, overlappingFactor = 2, numOfSamples = 512;
	float **y = NULL;
	float sum = 0;
	double costFunctionValue = 0;
	vector<Matrix<float> > *deltaW = NULL;
	vector<Matrix<float> > *Wm = NULL;
	Matrix<float> *Ryy = NULL;
	Matrix<float> *bdiagRyy = NULL;
	Matrix<float> *outputPowerOverIterations = NULL;

	initialiseWMatrix(L, D, numOfChannels, NON_SHIFTED, &Wm);

	sum = testInitialiseWMatrix(L, D, numOfChannels, Wm);

	if(sum != 0)
		cout << "Something went wrong!!!!" << endl;
	else
		cout << "Everything went fine!" << endl;

	//Allocate and Initialize y.
	allocate(&y, 2, numOfSamples);

	for(int i = 0; i < numOfSamples; i++)
	{
		y[0][i] = y[1][i] = 1;
	}

	try
	{
		BlockNaturalGradientWithSC<float> ng(L, D, N, numOfChannels, BlockNaturalGradientWithSC<float>::ROW, overlappingFactor, true);

		ng.getNaturalGradient(y, numOfSamples, Wm, deltaW, costFunctionValue, bdiagRyy, Ryy, outputPowerOverIterations);
	}
	catch(char const* str)
	{
		cout << str << endl;
	}

	deallocate(&y, 2);

	delete deltaW;
	delete Wm;
	delete Ryy;
	delete bdiagRyy;
	delete outputPowerOverIterations;

	return 0;
}

