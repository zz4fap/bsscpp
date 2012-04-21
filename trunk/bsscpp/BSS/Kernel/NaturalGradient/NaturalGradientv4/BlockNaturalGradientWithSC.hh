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
		virtual ~BlockNaturalGradientWithSC();
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

	protected:
		void getRyy(const VectorMatrix& R, Matrix<T>*& Ryy);
		void getbdiagRyy(const VectorMatrix& R, Matrix<T>*& bdiagRyy);
		void applySylvesterConstraint(const VectorMatrix* Wsc, VectorMatrix*& deltaW);
};

template <class T>
BlockNaturalGradientWithSC<T>::BlockNaturalGradientWithSC(UINT L, UINT D, UINT N, UINT numOfChannels, Constraint constraint, UINT overlappingFactor, bool isOverlappingAllowed)
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

/***********************************************************************Important Information/***********************************************************************
Wm[0](:,:) = W11
Wm[1](:,:) = W12
Wm[2](:,:) = W21
Wm[3](:,:) = W22

You have to delete yourself the following pointers Wm, deltaW, costFunctionValue, bdiagRyy, Ryy, outputPowerOverIterations
*********************************************************************************************************************************************************************/
template <class T>
void BlockNaturalGradientWithSC<T>::getNaturalGradient(T** y, UINT numOfSamples, const VectorMatrix* Wm, VectorMatrix*& deltaW, double& costFunctionValue, Matrix<T>*& bdiagRyy, Matrix<T>*& Ryy, Matrix<T>*& outputPowerOverIterations)
{	
	VectorMatrix *Wsc = NULL, *WmAux = NULL;
	VectorMatrix R(numOfChannels, Matrix<T>(D,D)); // R[0] = R11, R[1] = R12, R[2] = R21, R[3] = R22
	UINT numOfEpochs, detr11 = 0, detr22 = 0;
	Matrix<T> Rtemp(D,D), *Raux = NULL;
	Eye<T> eye(D,D);
	T *y1 = new T[N+D-1], *y2 = new T[N+D-1];
	long int startFrom = 0, initialValue = 0, regularizationFactor = 0;

	if(isOverlappingAllowed)
		numOfEpochs = (UINT)(floor(numOfSamples/(N/overlappingFactor)) - (overlappingFactor - 1));
	else
		numOfEpochs = (UINT)floor(numOfSamples/N);

	outputPowerOverIterations = new Matrix<T>(2,numOfEpochs);

	switch(this->constraint)
	{
		case BlockNaturalGradientWithSC::ROW:
		{
			Wsc = new std::vector< Matrix<T> >(this->numOfChannels, Matrix<T>(1,D));
			WmAux = new std::vector< Matrix<T> >(this->numOfChannels, Matrix<T>(1,D));
			for(int i = 0; i < this->numOfChannels; i++)
			{
				for(int k = 0; k < D; k++)
					(*WmAux)[i](0, k) = (*Wm)[i](L-1, k);
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
			throw "BlockNaturalGradientWithSC::getNaturalGradient: Invalid Constraint!";
	}

	for(int epoch = 0; epoch < numOfEpochs; epoch++)
	{
		// The input vector for both of the correlation functions y1[] and y2[] must be a vector with N+D-1 samples.
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
	    	ccm->getCrossCorrelationMatrix(y1,y2,R[1]);
	    	acm->getOutputPowerMatrix(y2, (float)10.0, R[3]);
	    	if(det(R[3]) == 0)
		{
			R[3] = R[3] + (eye*sqrt(var(diag(R[3]))));
			std::cout << "det(R22)==0: Trying to fix it by adding variance to the matrix" << std::endl;
		}
	    	if(det(R[3]) != 0) //If the determinant of R continues being equal to zero this test fails.
		{
			Rtemp = R[1]*inv(R[3]);

			switch(this->constraint)
			{
			    	case BlockNaturalGradientWithSC::ROW:
				{
					(*Wsc)[1] = (*Wsc)[1] + (*WmAux)[0]*Rtemp;
					(*Wsc)[3] = (*Wsc)[3] + (*WmAux)[2]*Rtemp;
					break;
				}
			    	case BlockNaturalGradientWithSC::COLUMN:
				{
					for(int i = 0; i < D; i++)
						(*Raux)(i,0) = Rtemp(i,0);				
					
					(*Wsc)[1] = (*Wsc)[1] + (*WmAux)[0]*(*Raux);
					(*Wsc)[3] = (*Wsc)[3] + (*WmAux)[2]*(*Raux);
					break;
				}
			}
		}
	    	else
		{
			detr22++;
			std::cout << "det(R22)==0: " << detr22 << " times." << std::endl;
		}
	    	//--------------------------------------------------------------//

		//--------------------------------------------------------------//
	    	acm->getOutputPowerMatrix(y1, (float)10.0, R[0]);
	    	R[2] = transpose(R[1]);
	    	if(det(R[0]) == 0)
		{
			R[0] = R[0] + (eye*sqrt(var(diag(R[0]))));
			std::cout << "det(R11)==0: Trying to fix it by adding variance to the matrix" << std::endl;
		}
	    	if(det(R[0]) != 0) //If the determinant of R continues being equal to zero this test fails.
		{
			Rtemp = R[2]*inv(R[0]);

			switch(this->constraint)
			{
			    	case BlockNaturalGradientWithSC::ROW:
				{
					(*Wsc)[0] = (*Wsc)[0] + (*WmAux)[1]*Rtemp;
					(*Wsc)[2] = (*Wsc)[2] + (*WmAux)[3]*Rtemp;
					break;
				}
			    	case BlockNaturalGradientWithSC::COLUMN:
				{
					for(int i = 0; i < D; i++)
						(*Raux)(i,0) = Rtemp(i,0);				
					
					(*Wsc)[0] = (*Wsc)[0] + (*WmAux)[1]*(*Raux);
					(*Wsc)[2] = (*Wsc)[2] + (*WmAux)[3]*(*Raux);
					break;
				}
			}
		}
	    	else
		{
			detr11++;
			std::cout << "det(R11)==0: " << detr11 << " times." << std::endl;
		}
	    	//-------------------------------------------------------------//

		regularizationFactor = 1e-323;
		getRyy(R, Ryy);
		getbdiagRyy(R, bdiagRyy);

		if(det((*Ryy)) == 0 || det((*bdiagRyy)) == 0)
			costFunctionValue = costFunctionValue + (log10((det((*bdiagRyy)) + regularizationFactor)) - log10((det((*Ryy)) + regularizationFactor)));
		else if(det((*Ryy)) < 0 || det((*bdiagRyy)) < 0)
		    	costFunctionValue = costFunctionValue + (log10(abs(det((*bdiagRyy)))) - log10(abs(det((*Ryy)))));
		else
		    	costFunctionValue = costFunctionValue + (log10((det((*bdiagRyy)))) - log10((det((*Ryy)))));

		// Verify whether there are imaginary values among the output values from the cost function.
		if(det((*Ryy)) < 0 || det((*bdiagRyy)) < 0)
		    	std::cout << "An imaginary value seems to have been found in the CostFuntionValue!!! det(Ryy) or det(bdiag(Ryy)) < 0." << std::endl;

		(*outputPowerOverIterations)(0,epoch) = R[0](0,0);
		(*outputPowerOverIterations)(1,epoch) = R[3](0,0);
	}
	
	costFunctionValue = costFunctionValue/numOfEpochs;

	for(int i = 0; i < this->numOfChannels; i++)
		(*Wsc)[i] = (*Wsc)[i]*(1/numOfEpochs);

	this->applySylvesterConstraint(Wsc, deltaW);

	delete Wsc;
	if(WmAux != NULL && this->constraint != BlockNaturalGradientWithSC::COLUMN)
		delete WmAux;
	if(Raux != NULL)
		delete Raux;

	delete [] y1;
	delete [] y2;
}

template <class T>
void BlockNaturalGradientWithSC<T>::getRyy(const VectorMatrix& R, Matrix<T>*& Ryy)
{
	Ryy = new Matrix<T>(2*D,2*D);

	for(int i = 0; i < D; i++)
	{
		for(int j = 0; j < D; j++)
		{
			(*Ryy)(i,j) = R[0](i,j);
			(*Ryy)(i,j+D) = R[1](i,j);
			(*Ryy)(D+i,j) = R[2](i,j);
			(*Ryy)(D+i,j+D) = R[3](i,j);
		}
	}
}

template <class T>
void BlockNaturalGradientWithSC<T>::getbdiagRyy(const VectorMatrix& R, Matrix<T>*& bdiagRyy)
{
	bdiagRyy = new Matrix<T>(2*D,2*D);

	for(int i = 0; i < D; i++)
	{
		for(int j = 0; j < D; j++)
		{
			(*bdiagRyy)(i,j) = R[0](i,j);
			(*bdiagRyy)(D+i,j+D) = R[3](i,j);
		}
	}
}

//INPUT: Wsc
//OUTPUT: deltaW
template <class T>
void BlockNaturalGradientWithSC<T>::applySylvesterConstraint(const VectorMatrix* Wsc, VectorMatrix*& deltaW)
{
	deltaW = new vector< Matrix<T> >(this->numOfChannels, Matrix<T>(2*L,D));

	switch(this->constraint)
	{
		case BlockNaturalGradientWithSC::ROW:
		{
			for(int channel = 0; channel < this->numOfChannels; channel++)
				for(int column = 0; column < this->D; column++)
					for(int row = 0; row < this->L; row++)
				    		(*deltaW)[channel]((row+column),column) = (*Wsc)[channel](0,((D-1) - row));
			break;
		}
	    	case BlockNaturalGradientWithSC::COLUMN:
		{
			for(int channel = 0; channel < this->numOfChannels; channel++)
				for(int column = 0; column < this->D; column++)
					for(int row = 0; row < this->L; row++)
				    		(*deltaW)[channel]((row+column),column) = (*Wsc)[channel](row,0);
			break;
		}
	}
}

#endif

