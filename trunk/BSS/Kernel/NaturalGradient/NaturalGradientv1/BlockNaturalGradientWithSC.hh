#ifndef BLOCK_NATURAL_GRADIENT_WITH_SC_HH
#define BLOCK_NATURAL_GRADIENT_WITH_SC_HH

#include <string>
#include <string.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <vector>
#include "Fourier.hh"
#include "Correlation.hh"
#include "CrossCorrelationMatrix.hh"
#include "AutoCorrelationMatrix.hh"
#include "MatrixOperations.hh"
#include "Matrix.hh"

typedef unsigned int UINT;

template <class T>
class BlockNaturalGradientWithSC
{
		typedef std::vector< Matrix<T> > VectorMatrix;

	public:
		enum Constraint
		{
			ROW,
			COLUMN
		};

		BlockNaturalGradientWithSC(UINT L, UINT D, UINT N, UINT numOfChannels, Constraint constraint, UINT overlappingFactor, bool isOverlappingAllowed);
		~BlockNaturalGradientWithSC();
		void getNaturalGradient(T** y, UINT numOfSamples, const VectorMatrix* Wm, VectorMatrix*& deltaW, double& CostFunctionValue, Matrix<T>*& bdiagRyy, Matrix<T>*& Ryy, Matrix<T>*& outputPowerOverIterations);

	private:
		UINT L;
		UINT D;
		UINT N;
		UINT numOfChannels;
		UINT overlappingFactor;
		
		Constraint constraint;
		bool isOverlappingAllowed;

		CrossCorrelationMatrix<T>* ccm;
		AutoCorrelationMatrix<T>* acm;

		void getRyy(const VectorMatrix& R, Matrix<T>*& Ryy);
		void getbdiagRyy(const VectorMatrix& R, Matrix<T>*& bdiagRyy);

};

template <class T>
BlockNaturalGradientWithSC<T>::BlockNaturalGradientWithSC(UINT L, UINT D, UINT N, UINT numChannels, Constraint constraint, UINT overlappingFactor, bool isOverlappingAllowed)
{
	this->L = L;
	this->D = D;
	this->N = N;
	this->numOfChannels = numOfChannels;
	this->overlappingFactor = overlappingFactor;
		
	this->constraint = constraint;
	this->isOverlappingAllowed = isOverlappingAllowed;

	this->ccm = new CrossCorrelationMatrix<T>(this->D, this->N);
	this->acm = new AutoCorrelationMatrix<T>(this->D, this->N);
}

template <class T>
BlockNaturalGradientWithSC<T>::~BlockNaturalGradientWithSC()
{
	delete this->ccm;
	delete this->acm;
}

/********Important Information***********
Wm[1](:,:) = W11
Wm[2](:,:) = W12
Wm[3](:,:) = W21
Wm[4](:,:) = W22
*****************************************/
template <class T>
void BlockNaturalGradientWithSC<T>::getNaturalGradient(T** y, UINT numOfSamples, const VectorMatrix* Wm, VectorMatrix*& deltaW, double& costFunctionValue, Matrix<T>*& bdiagRyy, Matrix<T>*& Ryy, Matrix<T>*& outputPowerOverIterations)
{
	VectorMatrix *Wsc = NULL, *WmAux = NULL, Raux< Matrix<float> >(numOfChannels, Matrix<float>(D,D));
	UINT numOfEpochs, detr11 = 0, detr22 = 0;
	Matrix<T> R(D,D), R12(D,D), R21(D,D), *Raux = NULL;
	Eye<T> R11(D,D), R22(D,D), eye(D,D);
	T *y1 = new T[N+D-1], *y2 = new T[N+D-1];
	long int startFrom = 0, initialValue = 0 regularizationFactor = 0;

	if(isOverlappingAllowed)
		numOfEpochs = (UINT)(floor(numOfSamples/(N/overlappingFactor)) - (overlappingFactor - 1));
	else
		numOfEpochs = (UINT)floor(numOfSamples/N);

	switch(this->constraint)
	{
		case BlockNaturalGradientWithSC::ROW:
		{
			Wsc = new std::vector< Matrix<T> >(this->numOfChannels, Matrix<T>(1,D));
			WmAux = new std::vector< Matrix<T> >(this->numOfChannels, Matrix<T>(1,D));
			for(int i = 0; i < this->numOfChannels; i++)
			{
				for(int k = 0; k < D; k++)
					(*WmAux)[i](1, k) = (*Wm)[i](L, k);
			}
			break;
		}
	    	case BlockNaturalGradientWithSC::COLUMN:
		{
			Wsc = new std::vector< Matrix<T> >(this->numOfChannels, Matrix<T>(2*L,1));
			Raux = new Matrix<T>(D, 1);
			WmAux = const_cast<VectorMatrix*>(Wm);
			break;
		}
	    	default:
		{
			std::cout << "Invalid Constraint. \n Assuming a row constraint." << std::endl;
			Wsc = new std::vector< Matrix<T> >(this->numOfChannels, Matrix<T>(1,D));
		}
	}

	for(int epoch = 0; epoch < numOfEpochs; epoch++)
	{
		//TODO: The input vector for both of the correlation functions y1[] and y2[] must be a vector with N+D-1 samples.
		initialValue = (epoch*(N/overlappingFactor)) - (D - 1);
		if(initialValue < 0)
			startFrom = -1*initialValue;
		else
			startFrom = 0;

		for(int k = startFrom; k < (N+D-1); k++)
		{
			y1[k] = y[0][initialValue + k];
			y2[k] = y[1][initialValue + k];
		}

		//--------------------------------------------------------------//
	    	ccm->getCrossCorrelationMatrix(y1,y2,R12);
	    	acm->getOutputPowerMatrix(y2, (float)10.0, R22);
	    	if(det(R22) == 0)
		{
			R22 = R22 + (eye*sqrt(var(diag(R22))));
			std::cout << "det(R22)==0: Trying to fix it by adding variance to the matrix" << std::endl;
		}
	    	if(det(R22) != 0) //If the determinant of R continues being equal to zero this test fails.
		{
			R = R12*inv(R22);

			switch(this->constraint)
			{
			    	case BlockNaturalGradientWithSC::ROW:
				{
					(*Wsc)[2] = (*Wsc)[2] + (*WmAux)[1]*R;
					(*Wsc)[4] = (*Wsc)[4] + (*WmAux)[3]*R;
					break;
				}
			    	case BlockNaturalGradientWithSC::COLUMN:
				{
					for(int i = 0; i < D; i++)
						(*Raux)(i,1) = R(i,1);				
					
					(*Wsc)[2] = (*Wsc)[2] + (*WmAux)[1]*(*Raux);
					(*Wsc)[4] = (*Wsc)[4] + (*WmAux)[3]*(*Raux);
					break;
				}
			}
		}
	    	else
		{
			detr22++;
			std::cout << "det(R22)==0: " << detr22 << "vezes" << std::endl;
		}
	    	//--------------------------------------------------------------//

		//--------------------------------------------------------------//
	    	acm->getOutputPowerMatrix(y1, (float)10.0, R11);
	    	R21 = transpose(R12);
	    	if(det(R11) == 0)
		{
			R11 = R11 + (eye*sqrt(var(diag(R11))));
			std::cout << "det(R11)==0: Trying to fix it by adding variance to the matrix" << std::endl;
		}
	    	if(det(R11) != 0) //If the determinant of R continues being equal to zero this test fails.
		{
			R = R21*inv(R11);

			switch(this->constraint)
			{
			    	case BlockNaturalGradientWithSC::ROW:
				{
					(*Wsc)[1] = (*Wsc)[1] + (*WmAux)[2]*R;
					(*Wsc)[3] = (*Wsc)[3] + (*WmAux)[4]*R;
					break;
				}
			    	case BlockNaturalGradientWithSC::COLUMN:
				{
					for(int i = 0; i < D; i++)
						(*Raux)(i,1) = R(i,1);				
					
					(*Wsc)[1] = (*Wsc)[1] + (*WmAux)[2]*(*Raux);
					(*Wsc)[3] = (*Wsc)[3] + (*WmAux)[4]*(*Raux);
					break;
				}
			}
		}
	    	else
		{
			detr11++;
			std::cout << "det(R11)==0: " << detr11 << "vezes" << std::endl;
		}
	    	//-------------------------------------------------------------//
	}

//matalab code -  start
        regularizationFactor = 1e-323;
	Raux[0] = R11; Raux[1] = R12; Raux[2] = R21; Raux[3] = R22; 


        //Ryy = [R11 R12;R21 R22];
        //bdiagRyy = [R11 zeros(D,D);zeros(D,D) R22];

        if(det(Ryy) == 0 || det(bdiagRyy) == 0)
            CostFunctionValue = CostFunctionValue + (log10((det(bdiagRyy) + regularizationfactor)) - log10((det(Ryy) + regularizationfactor)));
        else if(det(Ryy) < 0 || det(bdiagRyy) < 0)
            CostFunctionValue = CostFunctionValue + (log10(abs(det(bdiagRyy))) - log10(abs(det(Ryy))));
        else
            CostFunctionValue = CostFunctionValue + (log10((det(bdiagRyy))) - log10((det(Ryy))));

        // Verify wether there is imaginary values among the output values from the cost.
        if(det(Ryy) < 0 || det(bdiagRyy) < 0)
            std::cout << "An imaginary value seems to have been found in the CostFuntionValue!!! det(Ryy) or det(bdiag(Ryy)) < 0." << std::endl;

//matalab code -  end

	delete Wsc;
	if(WmAux != NULL && this->constraint != BlockNaturalGradientWithSC::COLUMN)
		delete WmAux;
	if(Raux != NULL)
		delete Raux;
}

template <class T>
void BlockNaturalGradientWithSC<T>::getRyy(const VectorMatrix& R, Matrix<T>*& Ryy)
{
	Ryy = new Matrix<T>(2*D,2*D);

	for(int i = 0; i < R.size(); i++)
	{
		fo


	}
	


}

template <class T>
void BlockNaturalGradientWithSC<T>::getbdiagRyy(const VectorMatrix& R, Matrix<T>*& Ryy)
{


}

#endif

