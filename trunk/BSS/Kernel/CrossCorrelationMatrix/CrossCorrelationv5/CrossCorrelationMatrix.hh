#ifndef CROSS_CORRELATION_MATRIX_HH
#define CROSS_CORRELATION_MATRIX_HH

#include <math.h>
#include <iostream>
#include <string>
#include <string.h>
#include <stdio.h>
#include <fstream>
#include <stdlib.h>
#include <memory>
#include <new>
#include <pthread.h>
#include "Fourier.hh"
#include "Correlation.hh"
#include "Matrix.hh"

template <class T>
class CrossCorrelationMatrix
{
	struct ThreadData
	{
		T* y2;
		T* data1;
		int column; 
		Matrix<T>* CMM;
	};

	private:
		Correlation<T>* corr;
		unsigned int D;
		unsigned int N;
		ThreadData threadData;
		pthread_mutex_t mutex;

		static void *start_thread(void *obj);
		void getColumnValues();

	public:
		CrossCorrelationMatrix(unsigned int D, unsigned int N);
		~CrossCorrelationMatrix();
		void getCrossCorrelationMatrix(T y1[], T y2[], Matrix<T>& CMM);
		void getCrossCorrelationMatrixv2(T y1[], T y2[], Matrix<T>& CMM);
		void getCrossCorrelationMatrixv3(T y1[], T y2[], Matrix<T>& CMM);
		void getCrossCorrelationMatrixv4(T y1[], T y2[], Matrix<T>& CMM);
};

template <class T>
CrossCorrelationMatrix<T>::CrossCorrelationMatrix(unsigned int D, unsigned int N)
{
	this->corr = new Correlation<T>();
	this->D = D;
	this->N = N;

	pthread_mutex_init(&mutex, NULL);
}

template <class T>
CrossCorrelationMatrix<T>::~CrossCorrelationMatrix()
{
	delete this->corr;

	pthread_mutex_destroy(&mutex);
}

//INPUT: y1[] and y2[] vectors with N+D-1 samples.
template <class T>
void CrossCorrelationMatrix<T>::getCrossCorrelationMatrixv2(T y1[], T y2[], Matrix<T>& CMM)
{
	unsigned int tempN = 2*N;

	T ans[4*tempN];

	T* data1 = new T[tempN];
	T* data2 = new T[tempN];

	memset(data1,0,sizeof(T)*tempN);
	memset(data2,0,sizeof(T)*tempN);

	for(int i = 0; i < (N+D-1); i++)
		data1[(N-(D-1))+i] = y1[i];

	for(int i = 0; i < this->D; i++) // Iterate over all rows: 0 up to (D-1)
	{
		for(int k = 0; k < this->N; k++)
			data2[N+k] = y2[k+i];

		this->corr->doCorrelation(data1, data2, tempN, 2*tempN, ans);

		for(int j = 0; j < this->D; j++)
		{
			if(j == 0)
				CMM(j,(D-1)-i) = tempN*ans[0];
			else
				CMM(j,(D-1)-i) = tempN*ans[4*tempN-2*j];
		}
	}

	delete [] data1;
	delete [] data2;
}

// WARNING: This method also produces the Cross Correlation Matrix but it takes so much time to do so.
//INPUT: y1[] and y2[] vectors with N+D-1 samples.
template <class T>
void CrossCorrelationMatrix<T>::getCrossCorrelationMatrixv3(T y1[], T y2[], Matrix<T>& CMM)
{
	Matrix<T> R1(D,N);
	Matrix<T> R2(N,D);

	for(int i = 0; i < D; i++)
	{
		for(int j = 0; j < N; j++)
		{
			R1(i,j) = y1[(D-1)-i+j];
		}
	}

	for(int i = 0; i < N; i++)
	{
		for(int j = 0; j < D; j++)
		{
			R2(i,j) = y2[(D-1)+i-j];
		}
	}

	CMM = R1*R2;
}

// WARNING: This method also produces the Cross Correlation Matrix but it takes so much time to do so.
//INPUT: y1[] and y2[] vectors with N+D-1 samples.
template <class T>
void CrossCorrelationMatrix<T>::getCrossCorrelationMatrixv4(T y1[], T y2[], Matrix<T>& CMM)
{
	Matrix<T> R1(D,N);
	Matrix<T> R2(N,D);

	for(int i = 0; i < D; i++)
	{
		memcpy(R1.getColumnAddress(i), (y1+((D-1)-i)), (sizeof(T)*N));
	}

	for(int i = 0; i < N; i++)
	{
		for(int j = 0; j < D; j++)
		{
			R2(i,j) = y2[(D-1)+i-j];
		}
	}

	CMM = R1*R2;
}

//INPUT: y1[] and y2[] vectors with N+D-1 samples.
template <class T>
void CrossCorrelationMatrix<T>::getCrossCorrelationMatrix(T y1[], T y2[], Matrix<T>& CMM)
{
	char str[100];
	unsigned int tempN = 2*N;
	int rc, column = 0;
	pthread_t threads[D];

	T* data1 = new T[tempN];

	memset(data1,0,sizeof(T)*tempN);

	memcpy((data1+(N-(D-1))),y1,(sizeof(T)*(N+D-1)));

	threadData.y2 = y2;
	threadData.data1 = data1;
	threadData.CMM = &CMM;
	threadData.column = D-1;

	for(int i = 0; i < D; i++) // Iterate over all rows: 0 up to (D-1)
	{
      		rc = pthread_create(&threads[i], NULL, &CrossCorrelationMatrix<T>::start_thread, this);
      		if (rc)
		{
         		sprintf(str, "ERROR; return code from pthread_create() is %d\n", rc);
			throw str;
      		}
	}

   	for(int j = 0; j < D; j++)
   	{
      		pthread_join(threads[j], NULL); 
   	}

	delete [] data1;
}

template <class T>
void CrossCorrelationMatrix<T>::getColumnValues()
{
	int column = 0;

	pthread_mutex_lock(&mutex);
 	column = threadData.column;
	--threadData.column;
	pthread_mutex_unlock(&mutex);

	T* data2 = new T[2*N];

	T ans[8*N];

	memset(data2,0,(sizeof(T)*2*N));

	memcpy((data2+N),(threadData.y2+column),(sizeof(T)*N));

	this->corr->doCorrelation(threadData.data1, data2, 2*N, 4*N, ans);

	for(int j = 0; j < this->D; j++)
	{
		if(j == 0)
			(*threadData.CMM)(j,((D-1)-column)) = 2*N*ans[0];
		else
			(*threadData.CMM)(j,((D-1)-column)) = 2*N*ans[8*N-2*j];
	}

	delete [] data2;
}

// This is the static class function that serves as a C style function pointer
// for the pthread_create call
template <class T>
void *CrossCorrelationMatrix<T>::start_thread(void *obj)
{
	//All we do here is call the getColumnValues() method.
        reinterpret_cast<CrossCorrelationMatrix<T> *>(obj)->getColumnValues();

	pthread_exit(NULL);
}

#endif
