#ifndef BLOCK_NATURAL_GRADIENT_WITH_SC_HH
#define BLOCK_NATURAL_GRADIENT_WITH_SC_HH

#include <string>
#include <string.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <vector>
#include <pthread.h>
#include <memory>
#include <new>
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
		typedef std::vector< Matrix<T>* > VectorMatrix_p;

		struct ThreadData
		{
			int type;
			T* y1; 
			T* y2;
			VectorMatrix_p* R;
			BlockNaturalGradientWithSC<T>* ng;
		};

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

		pthread_mutex_t mutex;

	protected:
		void getRyy(const VectorMatrix_p& R, Matrix<T>*& Ryy);
		void getbdiagRyy(const VectorMatrix_p& R, Matrix<T>*& bdiagRyy);
		void applySylvesterConstraint(const VectorMatrix* Wsc, VectorMatrix*& deltaW);
		UINT updateW(Matrix<T>& R1, Matrix<T>& R2, Matrix<T>& Wm1, Matrix<T>& Wm2, Matrix<T>& Wsc1, Matrix<T>& Wsc2);
		static void *startCorrelationThread(void *obj);
		void getAllCorrelations(ThreadData& td);
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

	pthread_mutex_init(&mutex, NULL);
}

template <class T>
BlockNaturalGradientWithSC<T>::~BlockNaturalGradientWithSC()
{
	delete this->ccm;
	delete this->acm;

	pthread_mutex_destroy(&mutex);
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
	VectorMatrix_p R;
	UINT numOfEpochs, detr11 = 0, detr22 = 0;
	T *y1 = new T[N+D-1], *y2 = new T[N+D-1];
	long int startFrom = 0, initialValue = 0, regularizationFactor = 0;
	long double detRyy = 0, detbdiagRyy = 0;
	ThreadData threadData;
	pthread_t threads[2];
	char str[100];
	int rc = 0;

	// Initializing the Correlation matrices
	R.push_back(new Eye<T>(D,D));		//R[0] = R11
	R.push_back(new Matrix<T>(D,D));	//R[1] = R12
	R.push_back(new Matrix<T>(D,D));	//R[2] = R21
	R.push_back(new Eye<T>(D,D));		//R[3] = R22

	// Initializing heap allocations
	memset(y1,0,(sizeof(T)*(N+D-1)));
	memset(y2,0,(sizeof(T)*(N+D-1)));

	// Initializing costFunctionValue
	costFunctionValue = 0;

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
		threadData.type = 1;
		threadData.y1 = y1; 
		threadData.y2 = y2;
		threadData.R = &R;
		threadData.ng = this;

		for(int i = 0; i < 2; i++)
		{
	      		rc = pthread_create(&threads[i], NULL, &BlockNaturalGradientWithSC<T>::startCorrelationThread, &threadData);
	      		if (rc)
			{
		 		sprintf(str, "ERROR; return code from pthread_create() is %d\n", rc);
				throw str;
	      		}
		}

   		for(int j = 0; j < 2; j++)
   		{
      			pthread_join(threads[j], NULL); 
   		}
	    	//--------------------------------------------------------------//

		//--------------------------------------------------------------//
		detr11 += updateW((*R[0]), (*R[2]), (*WmAux)[1], (*WmAux)[3], (*Wsc)[0], (*Wsc)[2]);

		if(detr11 != 0)
			std::cout << "det(R11)==0: " << detr11 << " times." << std::endl;

		detr22 += updateW((*R[3]), (*R[1]), (*WmAux)[0], (*WmAux)[2], (*Wsc)[1], (*Wsc)[3]); 

		if(detr22 != 0)
			std::cout << "det(R22)==0: " << detr22 << " times." << std::endl;
	    	//-------------------------------------------------------------//

		regularizationFactor = 1e-323;

		// Get all the determiant values at once.
		getRyy(R, Ryy);
		detRyy = det((*Ryy));									// Point of problem with speed.......

		getbdiagRyy(R, bdiagRyy);
		detbdiagRyy = det((*bdiagRyy));								// Point of problem with speed.......

		if(detRyy == 0 || detbdiagRyy == 0)
			costFunctionValue += (log10((detbdiagRyy + regularizationFactor)) - log10((detRyy + regularizationFactor)));
		else if(detRyy < 0 || detbdiagRyy < 0)
		    	costFunctionValue += (log10(fabs(detbdiagRyy)) - log10(fabs(detRyy)));
		else
		    	costFunctionValue += (log10((detbdiagRyy)) - log10((detRyy)));

		// Verify whether there are imaginary values among the output values from the cost function.
		if(detRyy < 0 || detbdiagRyy < 0)
		    	std::cout << "An imaginary value seems to have been found in the CostFuntionValue!!! det(Ryy) or det(bdiag(Ryy)) < 0." << std::endl;

		(*outputPowerOverIterations)(0,epoch) = (*R[0])(0,0);
		(*outputPowerOverIterations)(1,epoch) = (*R[3])(0,0);
	}
	
	costFunctionValue /= (double)numOfEpochs;

	for(int i = 0; i < this->numOfChannels; i++)
		(*Wsc)[i] = (*Wsc)[i]/((T)numOfEpochs);

	this->applySylvesterConstraint(Wsc, deltaW);

	delete Wsc;
	if(WmAux != NULL && this->constraint != BlockNaturalGradientWithSC::COLUMN)
		delete WmAux;

	delete [] y1;
	delete [] y2;
}

// This is the static class function that serves as a C style function pointer
// for the pthread_create call
template <class T>
void *BlockNaturalGradientWithSC<T>::startCorrelationThread(void *obj)
{
	ThreadData* td = (ThreadData*)obj;
	td->ng->getAllCorrelations(*td);

	pthread_exit(NULL);
}

template <class T>
void BlockNaturalGradientWithSC<T>::getAllCorrelations(ThreadData& td)
{
	int type = 0;

	pthread_mutex_lock(&mutex);
 	type = td.type;
	--td.type;
	pthread_mutex_unlock(&mutex);

	if(type)
		ccm->getCrossCorrelationMatrix(td.y1,td.y2,(*(*td.R)[1])); // R12
	else
	{
		acm->getOutputPowerMatrix(td.y2, (T)10.0, (*(*td.R)[3])); // R22
		acm->getOutputPowerMatrix(td.y1, (T)10.0, (*(*td.R)[0])); // R11
		(*(*td.R)[2]) = transpose((*(*td.R)[1])); // R21
	}
}

template <class T>
void BlockNaturalGradientWithSC<T>::getRyy(const VectorMatrix_p& R, Matrix<T>*& Ryy)
{
	Ryy = new Matrix<T>(2*D,2*D);

	for(int i = 0; i < D; i++)
	{
		for(int j = 0; j < D; j++)
		{
			(*Ryy)(i,j) = (*R[0])(i,j);
			(*Ryy)(i,j+D) = (*R[1])(i,j);
			(*Ryy)(D+i,j) = (*R[2])(i,j);
			(*Ryy)(D+i,j+D) = (*R[3])(i,j);
		}
	}
}

template <class T>
void BlockNaturalGradientWithSC<T>::getbdiagRyy(const VectorMatrix_p& R, Matrix<T>*& bdiagRyy)
{
	bdiagRyy = new Matrix<T>(2*D,2*D);

	for(int i = 0; i < D; i++)
	{
		for(int j = 0; j < D; j++)
		{
			(*bdiagRyy)(i,j) = (*R[0])(i,j);
			(*bdiagRyy)(D+i,j+D) = (*R[3])(i,j);
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

template <class T>
UINT BlockNaturalGradientWithSC<T>::updateW(Matrix<T>& R1, Matrix<T>& R2, Matrix<T>& Wm1, Matrix<T>& Wm2, Matrix<T>& Wsc1, Matrix<T>& Wsc2)
{
	Matrix<T> Rtemp(D,D);

	if(det(R1) != 0) //If the determinant of R is equal to zero this test fails.		// Point of problem with speed.......
	{
		Rtemp = (R2)*inv(R1);								// Point of problem with speed.......	

		switch(this->constraint)
		{
			case BlockNaturalGradientWithSC::ROW:
			{
				(Wsc1) = (Wsc1) + (Wm1)*Rtemp;
				(Wsc2) = (Wsc2) + (Wm2)*Rtemp;
				break;
			}
			case BlockNaturalGradientWithSC::COLUMN:
			{
				Matrix<T> Raux(D,1);
				for(int i = 0; i < D; i++)
					(Raux)(i,0) = Rtemp(i,0);				
					
				(Wsc1) = (Wsc1) + (Wm1)*(Raux);
				(Wsc2) = (Wsc2) + (Wm2)*(Raux);
				break;
			}
		}
		return 0;
	}
	else
		return 1;
}

#endif

