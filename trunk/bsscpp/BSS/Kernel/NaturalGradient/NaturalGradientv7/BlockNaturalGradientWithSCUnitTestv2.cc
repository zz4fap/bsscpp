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

typedef std::vector< Matrix<long double> > VectorMatrix;

#define ROW 0
#define COLUMN 1

template <class T>
void allocate(T ***y, UINT rows, UINT columns)
{
	(*y) = (T **)malloc(rows*sizeof(T*));
	for(int i=0; i<rows; i++)
		(*y)[i] = (T*)malloc(columns*sizeof(T));

	for(int i = 0; i < rows; i++)
	{
		for(int j = 0; j < columns; j++)
		{
			(*y)[i][j] = 0;
		}
	}
}

template <class T>
void deallocate(T ***y, UINT rows)
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

template <class T>
void initialiseWMatrix(UINT L, UINT D, UINT numOfChannels, InitializationType initializationType, VectorMatrix** W)
{
	int inc = 0;
	(*W) = new vector< Matrix<T> >(numOfChannels, Matrix<T>(2*L,D));

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
	double sum = 0;

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
	long double sum = 0;
	bool result = true;

	//******************Testing getNaturalGradient************************
	if(1)
	{
		try
		{
			cout << "Testing getNaturalGradient" << endl;

			UINT L = 128, D = 128, N = 1024, numOfChannels = 4, overlappingFactor = 2, numOfSamples = 70748;
			long double **y = NULL;
			double costFunctionValue = 0;
			vector<Matrix<long double> > *deltaW = NULL;
			vector<Matrix<long double> > *Wm = NULL;
			Matrix<long double> *Ryy = NULL;
			Matrix<long double> *bdiagRyy = NULL;
			Matrix<long double> *outputPowerOverIterations = NULL;
			FILE* arquivo = NULL;
			double value = 0, sum = 0;
			int countZeros = 0;

			//Allocate and Initialize y.
			allocate(&y, 2, numOfSamples);

			// ------reading y1---------
			if( (arquivo = fopen( "y1.dat", "rb" )) == NULL )
				cout << "The file was not opened" << endl;
			else
				cout << "The file was opened" << endl;

			for(int i = 0; i < numOfSamples; i++)
			{
				if (fread(&value, sizeof(double), 1, arquivo) != 1) 
					cout <<  "Erro de leitura" << endl;

				y[0][i] = (long double)value;
			}
			fclose(arquivo);
			// ------reading y1---------

			// ------reading y2---------
			if( (arquivo = fopen( "y2.dat", "rb" )) == NULL )
				cout << "The file was not opened" << endl;
			else
				cout << "The file was opened" << endl;

			for(int i = 0; i < numOfSamples; i++)
			{
				if (fread(&value, sizeof(double), 1, arquivo) != 1) 
					cout <<  "Erro de leitura" << endl;

				y[1][i] = (long double)value;
			}
			fclose(arquivo);
			// ------reading y2---------

			initialiseWMatrix<long double>(L, D, numOfChannels, NON_SHIFTED, &Wm);

			BlockNaturalGradientWithSC<long double> ng(L, D, N, numOfChannels, (BlockNaturalGradientWithSC<long double>::Constraint)ROW, overlappingFactor, true);

			ng.getNaturalGradient(y, numOfSamples, Wm, deltaW, costFunctionValue, bdiagRyy, Ryy, outputPowerOverIterations);

			// ------Checking deltaW-------
			if( (arquivo = fopen( "deltaW4.dat", "rb" )) == NULL )
				cout << "The file was not opened" << endl;
			else
				cout << "The file was opened" << endl;

			for(int k = 0; k < numOfChannels; k++)
			{
				for(int i = 0; i < 2*L; i++)
				{
					for(int j = 0; j < L; j++)
					{
						if (fread(&value, sizeof(double), 1, arquivo) != 1) 
							cout <<  "Erro de leitura" << endl;

						sum += sqrt(pow((value - (*deltaW)[k](i,j)),2));			
					}
				}
			}
			fclose(arquivo);

			if(sum > 2.5e-12)
			{
				cout << "Something went wrong!!!!!" << endl;
				result &= false;
			}
			else
			{
				cout << "Everything is fine!" << endl;
				result &= true;
			}
			// ------Checking deltaW-------

			// ------Checking costFunctionValue-------
			if(1)
			{
				if(sqrt(pow(costFunctionValue,2)) > sqrt(pow(-2.3688,2)))
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
			// ------Checking costFunctionValue-------

			// ------Checking bdiagRyy-------
			if(1)
			{
				if( (arquivo = fopen( "bdiagRyy4.dat", "rb" )) == NULL )
					cout << "The file was not opened" << endl;
				else
					cout << "The file was opened" << endl;

				sum = 0;
				for(int i = 0; i < 2*L; i++)
				{
					for(int j = 0; j < 2*L; j++)
					{
						if (fread(&value, sizeof(double), 1, arquivo) != 1) 
							cout <<  "Erro de leitura" << endl;

						sum += sqrt(pow((value - (*bdiagRyy)(i,j)),2));
					}
				}
				fclose(arquivo);

				if(sum > 7.98e-14)
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
			// ------Checking bdiagRyy-------
		
			// ------Checking Ryy-------
			if(1)
			{
				if( (arquivo = fopen( "Ryy4.dat", "rb" )) == NULL )
					cout << "The file was not opened" << endl;
				else
					cout << "The file was opened" << endl;

				sum = 0;
				for(int i = 0; i < 2*L; i++)
				{
					for(int j = 0; j < 2*L; j++)
					{
						if (fread(&value, sizeof(double), 1, arquivo) != 1) 
							cout <<  "Erro de leitura" << endl;

						sum += sqrt(pow((value - (*Ryy)(i,j)),2));
					}
				}
				fclose(arquivo);

				if(sum > 3.1e-08)
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
			// ------Checking Ryy-------

			// ------Checking outputPowerOverIterations-------
			if(1)
			{
				if( (arquivo = fopen( "outputPowerOverIterations4.dat", "rb" )) == NULL )
					cout << "The file was not opened" << endl;
				else
					cout << "The file was opened" << endl;
				sum = 0;
				for(int k = 0; k < 2; k++)
				{
					for(int i = 0; i < 15; i++)
					{
						if (fread(&value, sizeof(double), 1, arquivo) != 1) 
							cout <<  "Erro de leitura" << endl;

						sum += sqrt(pow(((*outputPowerOverIterations)(k,i) - value),2));
					}
				}
				fclose(arquivo);

				if(sum > 4.97e-14)
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
			// ------Checking outputPowerOverIterations-------

			deallocate(&y, 2);

			delete deltaW;
			delete Wm;
			delete Ryy;
			delete bdiagRyy;
			delete outputPowerOverIterations;
		}
		catch(char const* str)
		{
			cout << str << endl;
		}
	}
	//******************Testing getNaturalGradient************************

	if(result)
		cout << "FINAL RESULT: OK!" << endl;
	else
		cout << "FINAL RESULT: NOK!!!!!" << endl;

	return 0;
}

