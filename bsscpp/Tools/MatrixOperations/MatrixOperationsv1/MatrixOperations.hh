#ifndef MATRIX_OPERATIONS_HH
#define MATRIX_OPERATIONS_HH

#include <math.h>
#include <iostream>
#include <string>
#include <string.h>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include "Matrix.hh"

template <class T>
class MatrixOperations : public Matrix<T>
{
	public:
		template <class D> 
		friend double det(const Matrix<D>& a);
		template <class D> 
		friend double var(const Matrix<D>& arr);
		template <class D> 
		friend Matrix<D> diag(const Matrix<D>& a);
		template <class D> 
		friend Matrix<D> transpose(const Matrix<D>& a);

	private:
		MatrixOperations(unsigned int rows, unsigned int columns);
		MatrixOperations(const Matrix<T>& p);
		~MatrixOperations();

};

template <class T>
MatrixOperations<T>::MatrixOperations(unsigned int rows, unsigned int columns) : Matrix<T>(rows,columns)
{

}

template <class T>
MatrixOperations<T>::MatrixOperations(const Matrix<T>& p) : Matrix<T>(p)
{
	// Do nothing but copy p.
}

template <class D> 
double det(const Matrix<D>& a)
{
	int i,j,j1,j2, n;
	double dett = 0;
	Matrix<D> *m = NULL;

	if(a.getNumberOfRows() != a.getNumberOfColumns())
		throw "Error: Matrix is not square";

	n = a.getNumberOfRows();
	
	if (n == 1) 
	{ 	
		/* Shouldn't get used */
      		dett = a(0,0);
   	} 
	else if (n == 2)
	{
      		dett = a(0,0) * a(1,1) - a(1,0) * a(0,1);
   	} 
	else 
	{
      		dett = 0;
	      	for (j1 = 0; j1 < n; j1++) 
		{
			m = new Matrix<D>(a.getNumberOfRows() - 1, a.getNumberOfColumns() - 1);

		 	for (i = 1; i < n; i++) 
			{
		    		j2 = 0;
		    		for (j = 0; j < n; j++) 
				{
		       			if (j == j1)
		          			continue;

		       			(*m)((i-1),j2) = a(i,j);
		       			j2++;
		    		}
         		}
         		dett += pow(-1.0,1.0+j1+1.0) * a(0,j1) * det((*m));

			delete m;
      		}
   	}
   	return(dett);
}

template <class D> 
double var(const Matrix<D>& arr)
{
    	int no = 0;
    	double sum = 0.0, avg = 0.0;

	if(arr.getNumberOfRows() != 1)
		throw "Error: Input parameter is not a vector.";

	no = arr.getNumberOfColumns();

    	for (int i = 0; i < no; i++)
		sum += arr(0,i);

    	avg = sum / (double) no;

	sum = 0.0;
    	for (int i = 0; i < no; i++)
		sum += (arr(0,i) - avg) * (arr(0,i) - avg);

    	return (sum / (double) (no - 1));

}

template <class D> 
Matrix<D> diag(const Matrix<D>& a)
{
	if(a.getNumberOfRows() != a.getNumberOfColumns())
		throw "Error: Matrix is not square";

	Matrix<D> temp(1, a.getNumberOfColumns());

	for(int i = 0; i < a.getNumberOfColumns(); i++)
		temp(0,i) = a(i,i);

	return temp;
}

template <class D> 
Matrix<D> transpose(const Matrix<D>& a)
{
	Matrix<D> temp(a.getNumberOfColumns(), a.getNumberOfRows());

	for(int i = 0; i < a.getNumberOfRows(); i++)
	{
		for(int j = 0; j < a.getNumberOfColumns(); j++)
		{
			temp(j,i) = a(i,j);
		}
	}

	return temp;
}

#endif
