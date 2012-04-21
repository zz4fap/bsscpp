#include <string.h>
#include <iostream>
#include <string>
#include <fstream>
#include <stdlib.h>
#include <vector>
#include <math.h>
#include "Matrix.hh"
#include "BlockNaturalGradientWithSC.hh"

using namespace std;

typedef std::vector< Matrix<float> > VectorMatrix;

#define ROW 0
#define COLUMN 1

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

template <class T>
class NaturalGradientTest : public BlockNaturalGradientWithSC<T>
{
	public:
		NaturalGradientTest(UINT L, UINT D, UINT N, UINT numOfChannels, int constraint, UINT overlappingFactor, bool isOverlappingAllowed);
		void getRyy(const VectorMatrix& R, Matrix<T>*& Ryy);
		void getbdiagRyy(const VectorMatrix& R, Matrix<T>*& bdiagRyy);
		void applySylvesterConstraint(const VectorMatrix* Wsc, VectorMatrix*& deltaW);
};

template <class T>
NaturalGradientTest<T>::NaturalGradientTest(UINT L, UINT D, UINT N, UINT numOfChannels, int constraint, UINT overlappingFactor, bool isOverlappingAllowed) : BlockNaturalGradientWithSC<T>((UINT)L, (UINT)D, (UINT)N, (UINT)numOfChannels, (typename BlockNaturalGradientWithSC<T>::Constraint) constraint, (UINT)overlappingFactor, (bool)isOverlappingAllowed)
{}

template <class T>
void NaturalGradientTest<T>::getRyy(const VectorMatrix& R, Matrix<T>*& Ryy)
{
	BlockNaturalGradientWithSC<T>::getRyy(R, Ryy);
}

template <class T>
void NaturalGradientTest<T>::getbdiagRyy(const VectorMatrix& R, Matrix<T>*& bdiagRyy)
{
	BlockNaturalGradientWithSC<T>::getbdiagRyy(R, bdiagRyy);
}

template <class T>
void NaturalGradientTest<T>::applySylvesterConstraint(const VectorMatrix* Wsc, VectorMatrix*& deltaW)
{
	BlockNaturalGradientWithSC<T>::applySylvesterConstraint(Wsc, deltaW);
}


int main()
{
	// L = D = 128
	UINT L = 128, D = 128, N = 256, numOfChannels = 4, overlappingFactor = 2, numOfSamples = 512;
	float **y = NULL;
	float sum = 0;
	bool result = true;
	double costFunctionValue = 0;
	vector<Matrix<float> > *deltaW = NULL;
	vector<Matrix<float> > *Wm = NULL;
	Matrix<float> *Ryy = NULL;
	Matrix<float> *bdiagRyy = NULL;
	Matrix<float> *outputPowerOverIterations = NULL;
	VectorMatrix *Wsc = NULL;

	initialiseWMatrix(L, D, numOfChannels, NON_SHIFTED, &Wm);

	sum = testInitialiseWMatrix(L, D, numOfChannels, Wm);

	if(sum != 0)
	{
		cout << "Something went wrong!!!!!" << endl;
		result &= false;
	}
	else
	{
		cout << "Everything is fine!" << endl;
		result &= true;
	}

	//Allocate and Initialize y.
	allocate(&y, 2, numOfSamples);

	for(int i = 0; i < numOfSamples; i++)
	{
		y[0][i] = y[1][i] = 1;
	}
	
	//******************Testing applySylvesterConstraint************************
	try
	{
			NaturalGradientTest<float> ng((UINT)4, (UINT)4, (UINT)N, (UINT)4, ROW, (UINT)overlappingFactor, true);
			Wsc = new std::vector< Matrix<float> >(4, Matrix<float>(1,4));
			(*Wsc)[0](0,0) = 1;
			(*Wsc)[0](0,1) = 2;
			(*Wsc)[0](0,2) = 3;
			(*Wsc)[0](0,3) = 4;

			(*Wsc)[1](0,0) = 5;
			(*Wsc)[1](0,1) = 6;
			(*Wsc)[1](0,2) = 7;
			(*Wsc)[1](0,3) = 8;

			(*Wsc)[2](0,0) = 9;
			(*Wsc)[2](0,1) = 10;
			(*Wsc)[2](0,2) = 11;
			(*Wsc)[2](0,3) = 12;

			(*Wsc)[3](0,0) = 13;
			(*Wsc)[3](0,1) = 14;
			(*Wsc)[3](0,2) = 15;
			(*Wsc)[3](0,3) = 16;

			(*Wsc)[0].dumpMatrixContents();
			(*Wsc)[1].dumpMatrixContents();
			(*Wsc)[2].dumpMatrixContents();
			(*Wsc)[3].dumpMatrixContents();
			cout << endl;
		
			ng.applySylvesterConstraint(Wsc, deltaW);
		
			(*deltaW)[0].dumpMatrixContents();
			cout << endl;
			(*deltaW)[1].dumpMatrixContents();
			cout << endl;
			(*deltaW)[2].dumpMatrixContents();
			cout << endl;
			(*deltaW)[3].dumpMatrixContents();

			sum = 0;
			for(int i = 0; i < 4; i++)
			{
				for(int k = 0; k < 4; k++)
				{
					for(int j = 0; j < 8; j++)
					{
						if(j >= k && j <= 3+k)
							sum += (*deltaW)[i](j,k) - (*Wsc)[i](0,(3+k-j));
						else
							sum += (*deltaW)[i](j,k);
					}
				}
			}

			if(sum != 0)
			{
				cout << "Something went wrong!!!!!" << endl;
				result &= false;
			}
			else
			{
				cout << "Everything is fine!" << endl;
				result &= true;
			}

			delete Wsc;
	}
	catch(char const* str)
	{
		cout << "Something went wrong!!!!!" << endl;
		result &= false;
		cout << str << endl;
	}
	//******************Testing applySylvesterConstraint************************

	//******************Testing applySylvesterConstraint************************
	try
	{
			NaturalGradientTest<float> ng((UINT)4, (UINT)4, (UINT)N, (UINT)4, COLUMN, (UINT)overlappingFactor, true);
			Wsc = new std::vector< Matrix<float> >(4, Matrix<float>(8,1));
			cout << endl;
			(*Wsc)[0](0,0) = 1;
			(*Wsc)[0](1,0) = 2;
			(*Wsc)[0](2,0) = 3;
			(*Wsc)[0](3,0) = 4;
			(*Wsc)[0](4,0) = 5;
			(*Wsc)[0](5,0) = 6;
			(*Wsc)[0](6,0) = 7;
			(*Wsc)[0](7,0) = 8;

			(*Wsc)[1](0,0) = 5;
			(*Wsc)[1](1,0) = 6;
			(*Wsc)[1](2,0) = 7;
			(*Wsc)[1](3,0) = 8;
			(*Wsc)[1](4,0) = 9;
			(*Wsc)[1](5,0) = 10;
			(*Wsc)[1](6,0) = 11;
			(*Wsc)[1](7,0) = 12;

			(*Wsc)[2](0,0) = 9;
			(*Wsc)[2](1,0) = 10;
			(*Wsc)[2](2,0) = 11;
			(*Wsc)[2](3,0) = 12;
			(*Wsc)[2](4,0) = 13;
			(*Wsc)[2](5,0) = 14;
			(*Wsc)[2](6,0) = 15;
			(*Wsc)[2](7,0) = 16;

			(*Wsc)[3](0,0) = 13;
			(*Wsc)[3](1,0) = 14;
			(*Wsc)[3](2,0) = 15;
			(*Wsc)[3](3,0) = 16;
			(*Wsc)[3](4,0) = 17;
			(*Wsc)[3](5,0) = 18;
			(*Wsc)[3](6,0) = 19;
			(*Wsc)[3](7,0) = 20;

			ng.applySylvesterConstraint(Wsc, deltaW);
		
			(*deltaW)[0].dumpMatrixContents();
			cout << endl;
			(*deltaW)[1].dumpMatrixContents();
			cout << endl;
			(*deltaW)[2].dumpMatrixContents();
			cout << endl;
			(*deltaW)[3].dumpMatrixContents();

			sum = 0;
			for(int i = 0; i < 4; i++)
			{
				for(int k = 0; k < 4; k++)
				{
					for(int j = 0; j < 8; j++)
					{
						if(j >= k && j <= 3+k)
							sum += (*deltaW)[i](j,k) - (*Wsc)[i]((j-k),0);
						else
							sum += (*deltaW)[i](j,k);
					}
				}
			}

			if(sum != 0)
			{
				cout << "Something went wrong!!!!!" << endl;
				result &= false;
			}
			else
			{
				cout << "Everything is fine!" << endl;
				result &= true;
			}

			delete Wsc;
	}
	catch(char const* str)
	{
		cout << "Something went wrong!!!!!" << endl;
		result &= false;
		cout << str << endl;
	}
	//******************Testing applySylvesterConstraint************************

	//******************Testing getRyy************************
	try
	{
		NaturalGradientTest<float> ng((UINT)4, (UINT)4, (UINT)N, (UINT)4, COLUMN, (UINT)overlappingFactor, true);
	
		VectorMatrix R(4, Matrix<float>(4,4));

		for(int i = 0; i < 4; i++)
		{
			for(int k = 0; k < 4; k++)
			{
				for(int j = 0; j < 4; j++)
				{
					R[i](k,j) = j+i;
				}
			}
		}

		Matrix<float>* Ryy = NULL;

		ng.getRyy(R, Ryy);

		cout << endl << endl << endl;

		(*Ryy).dumpMatrixContents();

		sum = 0;
		int cont = 0;
		for(int i = 0; i < 2; i++)
		{
			for(int l = 0; l < 2; l++)
			{
				for(int k = 0; k < 4; k++)
				{
					for(int j = 0; j < 4; j++)
						sum += (*Ryy)(k+(i*4),j+(l*4)) - (j+(2*i+l));
				}
			}
		}

		if(sum != 0)
		{
			cout << "Something went wrong!!!!!" << endl;
			result &= false;
		}
		else
		{
			cout << "Everything is fine!" << endl;
			result &= true;
		}
	}
	catch(char const* str)
	{
		cout << "Something went wrong!!!!!" << endl;
		result &= false;
		cout << str << endl;
	}
	//******************Testing getRyy************************

	//******************Testing getbdiagRyy************************
	try
	{
		NaturalGradientTest<float> ng((UINT)4, (UINT)4, (UINT)N, (UINT)4, COLUMN, (UINT)overlappingFactor, true);
	
		VectorMatrix R(4, Matrix<float>(4,4));

		for(int i = 0; i < 4; i++)
		{
			for(int k = 0; k < 4; k++)
			{
				for(int j = 0; j < 4; j++)
				{
					R[i](k,j) = j+i;
				}
			}
		}

		Matrix<float>* Ryy = NULL;

		ng.getbdiagRyy(R, Ryy);

		cout << endl << endl << endl;

		(*Ryy).dumpMatrixContents();

		sum = 0;
		int cont = 0;
		for(int i = 0; i < 2; i++)
		{
			for(int l = 0; l < 2; l++)
			{
				for(int k = 0; k < 4; k++)
				{
					for(int j = 0; j < 4; j++)
					{
						if((2*i+l) == 0 || (2*i+l) == 3)
							sum += (*Ryy)(k+(i*4),j+(l*4)) - (j+(2*i+l));
						else
							sum += (*Ryy)(k+(i*4),j+(l*4));
					}
				}
			}
		}

		if(sum != 0)
		{
			cout << "Something went wrong!!!!!" << endl;
			result &= false;
		}
		else
		{
			cout << "Everything is fine!" << endl;
			result &= true;
		}
	}
	catch(char const* str)
	{
		cout << "Something went wrong!!!!!" << endl;
		result &= false;
		cout << str << endl;
	}

	//******************Testing getbdiagRyy************************


	//******************Testing getNaturalGradient************************
	try
	{
		//BlockNaturalGradientWithSC<float> ng(L, D, N, numOfChannels, BlockNaturalGradientWithSC<float>::ROW, overlappingFactor, true);

		//ng.getNaturalGradient(y, numOfSamples, Wm, deltaW, costFunctionValue, bdiagRyy, Ryy, outputPowerOverIterations);

		

		
	}
	catch(char const* str)
	{
		cout << str << endl;
	}
	//******************Testing getNaturalGradient************************

	if(result)
		cout << "FINAL RESULT: OK!" << endl;
	else
		cout << "FINAL RESULT: NOK!!!!!" << endl;

	deallocate(&y, 2);

	delete deltaW;
	delete Wm;
	delete Ryy;
	delete bdiagRyy;
	delete outputPowerOverIterations;

	return 0;
}

