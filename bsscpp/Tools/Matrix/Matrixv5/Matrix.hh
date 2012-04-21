#ifndef MATRIX_HH
#define MATRIX_HH

#include <math.h>
#include <iostream>
#include <typeinfo>
#include <string>
#include <string.h>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>

#define TINY 1.0e-20

template <class T>
class Matrix
{
	protected:
		T** matrix;
		unsigned int rows;
		unsigned int columns;

	public:
		Matrix(unsigned int rows, unsigned int columns);
		Matrix(const Matrix& p);
		virtual ~Matrix();
		unsigned int getNumberOfRows() const;
		unsigned int getNumberOfColumns() const;
		void dumpMatrixContents();
		void swaprows(int i,int j);
		T* getColumnAddress(int row);
		Matrix& operator=(const Matrix& op2);
		T& operator()(unsigned int row, unsigned int col);
		T operator()(unsigned int row, unsigned int col) const;
		Matrix operator+(const Matrix& op2);
		Matrix operator+(T op2);
		Matrix operator-(const Matrix& op2);
		Matrix operator-(T op2);
		Matrix operator*(const Matrix& op2);
		Matrix operator*(T op2);
		Matrix operator/(Matrix op2);
		Matrix operator/(T op2);

		template <class D>
		friend Matrix<D> inv(const Matrix<D>& matrix);
		template <class D> 
		friend double det(const Matrix<D>& op);
		template <class D> 
		friend double var(const Matrix<D>& arr);
		template <class D> 
		friend Matrix<D> diag(const Matrix<D>& a);
		template <class D> 
		friend Matrix<D> transpose(const Matrix<D>& a);
};

template <class T>
Matrix<T>::Matrix(unsigned int rows, unsigned int columns)
{
	this->rows = rows;
	this->columns = columns;

	if(rows <= 0 || columns <= 0)
		throw "Error: Matrix must have dimension greater than 0!!!";

	this->matrix = (T**)malloc(rows*sizeof(T*));
	if(this->matrix == NULL)
		throw "Malloc function failed!!!";

	for(int i = 0; i < rows; i++)
	{
		matrix[i] = (T*)malloc(columns*sizeof(T));
		if(matrix[i] == NULL)
			throw "Malloc function failed!!!";
	}

	for(int i = 0; i < rows; i++)
		for(int j = 0; j < columns; j++)
			matrix[i][j] = 0;
}

template <class T>
Matrix<T>::Matrix(const Matrix& p)
{
	this->rows = p.getNumberOfRows();
	this->columns = p.getNumberOfColumns();

	this->matrix = (T**)malloc(rows*sizeof(T*));
	if(this->matrix == NULL)
		throw "Malloc function failed!!!";

	for(int i = 0; i < rows; i++)
	{
		matrix[i] = (T*)malloc(columns*sizeof(T));
		if(matrix[i] == NULL)
			throw "Malloc function failed!!!";
	}

	for(int i = 0; i < rows; i++)
		for(int j = 0; j < columns; j++)
			matrix[i][j] = p(i,j);
}

template <class T>
Matrix<T>::~Matrix()
{
	if(this->matrix)
	{
		for(int i = 0; i < this->rows; i++)
			free(this->matrix[i]);

		free(this->matrix);

		this->matrix = NULL;
	}
}

template <class T>
Matrix<T>& Matrix<T>::operator=(const Matrix& op2)
{
	for(int i = 0; i < rows; i++)
		for(int j = 0; j < columns; j++)
			matrix[i][j] = op2(i,j);

	return *this;
}

template <class T>
T& Matrix<T>::operator()(unsigned int row, unsigned int col)
{
	if (row >= this->rows || col >= this->columns)
     		throw "Matrix subscript out of bounds";
   
	return this->matrix[row][col];
}

template <class T>
T Matrix<T>::operator()(unsigned int row, unsigned int col) const
{
	if (row >= this->rows || col >= this->columns)
     		throw "Matrix subscript out of bounds";
   
	return this->matrix[row][col];
}

template <class T>
unsigned int Matrix<T>::getNumberOfRows() const
{
	return 	this->rows;
}

template <class T>
unsigned int Matrix<T>::getNumberOfColumns() const
{
	return this->columns;
}

template <class T>
void Matrix<T>::dumpMatrixContents()
{
	for(int i = 0; i < this->getNumberOfRows(); i++)
	{
		for(int j = 0; j < this->getNumberOfColumns(); j++)
		{
			std::cout << (*this)(i,j) << " ";
		}
		std::cout << std::endl;
	}
}

template <class T>
T* Matrix<T>::getColumnAddress(int row)
{
	return (T*)matrix[row];
}

template <class T>
Matrix<T> Matrix<T>::operator+(const Matrix& op2)
{
	Matrix temp(this->rows,this->columns);

	if(this->columns != op2.getNumberOfColumns() || this->rows != op2.getNumberOfRows())
		throw "Dimensions do NOT match!!!";

	for(int i = 0; i < rows; i++)
		for(int j = 0; j < columns; j++)
			temp(i,j) = this->matrix[i][j] + op2(i,j);

	return temp;
}

template <class T>
Matrix<T> Matrix<T>::operator+(T op2)
{
	Matrix temp(this->rows,this->columns);

	for(int i = 0; i < rows; i++)
		for(int j = 0; j < columns; j++)
			temp.matrix[i][j] = this->matrix[i][j] + op2;

	return temp;
}

template <class T>
Matrix<T> Matrix<T>::operator-(const Matrix& op2)
{
	Matrix temp(this->rows,this->columns);

	if(this->columns != op2.getNumberOfColumns() || this->rows != op2.getNumberOfRows())
		throw "Dimensions do NOT match!!!";

	for(int i = 0; i < rows; i++)
		for(int j = 0; j < columns; j++)
			temp(i,j) = this->matrix[i][j] - op2(i,j);

	return temp;
}

template <class T>
Matrix<T> Matrix<T>::operator-(T op2)
{
	Matrix temp(this->rows,this->columns);

	for(int i = 0; i < rows; i++)
		for(int j = 0; j < columns; j++)
			temp.matrix[i][j] = this->matrix[i][j] - op2;

	return temp;
}

template <class T>
Matrix<T> Matrix<T>::operator*(const Matrix& op2)
{
	unsigned int rows, columns;

	if(this->columns != op2.getNumberOfRows())
		throw "Dimensions do NOT match!!!";

	rows = this->rows;
	columns = op2.getNumberOfColumns();

	Matrix temp(rows,columns);

	for(int i = 0; i < rows; i++)
	{
		for(int j = 0; j < columns; j++)
		{
			for(int k = 0; k < this->columns; k++)
				temp(i,j) += this->matrix[i][k] * op2(k,j);
		}
	}

	return temp;
}

template <class T>
Matrix<T> Matrix<T>::operator*(T op2)
{
	Matrix temp(this->rows,this->columns);

	for(int i = 0; i < rows; i++)
		for(int j = 0; j < columns; j++)
			temp(i,j) = this->matrix[i][j] * op2;

	return temp;
}

template <class T>
Matrix<T> Matrix<T>::operator/(T op2)
{
	Matrix temp(this->rows,this->columns);

	for(int i = 0; i < rows; i++)
		for(int j = 0; j < columns; j++)
			temp(i,j) = this->matrix[i][j] / op2;

	return temp;
}

template <class T>
void Matrix<T>::swaprows(int i,int j)
{
	T aux;

	for(int k = 0; k < this->rows; k++)
	{
		aux = (*this)(i,k);
		(*this)(i,k) = (*this)(j,k);
		(*this)(j,k) = aux;
	}
}

// This is the very fisrt function used to calculate the Determinat, but it had 
// to be substituted by a new one as it takes so much time to calculate the determinat
// of big matrices.
template <class D> 
double getDet(const Matrix<D>& a)
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

   	return dett;
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

template <class D>
int ludcmp(Matrix<D>& a, int *order)
{
	int i, j, k, n, nm1;
	int flag = 1;    /* changes sign with each row interchange */
	double sum, diag;

	if(a.getNumberOfRows() != a.getNumberOfColumns())
		throw "Error: Matrix is not square";

	n = a.getNumberOfRows();

	/* establish initial ordering in order vector */
	
	for (i=0; i<n; i++) order[i] = i;

	/* do pivoting for first column and check for singularity */

	if (pivot(a,order,0)) flag = -flag;
	diag = 1.0/a(0,0);
	for (i=1; i<n; i++) a(0,i) *= diag;
	
	/* 
	*  Now complete the computing of L and U elements.
	*  The general plan is to compute a column of L's, then
	*  call pivot to interchange rows, and then compute
	*  a row of U's.
	*/
	
	nm1 = n - 1;
	for (j=1; j<nm1; j++) {
		/* column of L's */
		for (i=j; i<n; i++) {
			sum = 0.0;
			for (k=0; k<j; k++) sum += a(i,k)*a(k,j);
			a(i,j) -= sum;
		}
		/* pivot, and check for singularity */
		if (pivot(a,order,j)) flag = -flag;
		/* row of U's */
		diag = 1.0/a(j,j);
		for (k=j+1; k<n; k++) {
			sum = 0.0;
			for (i=0; i<j; i++) sum += a(j,i)*a(i,k);
			a(j,k) = (a(j,k)-sum)*diag;
		}
	}

	/* still need to get last element in L Matrix */

	sum = 0.0;
	for (k=0; k<nm1; k++) sum += a(nm1,k)*a(k,nm1);
	a(nm1,nm1) -= sum;
	return flag;
}

template <class D>
int pivot(Matrix<D>& a, int order[], int jcol)
{
	int i, ipvt,n;
	double big, anext, temp;

	if(a.getNumberOfRows() != a.getNumberOfColumns())
		throw "Error: Matrix is not square";

	n = a.getNumberOfRows();

	/*
	*  Find biggest element on or below diagonal.
	*  This will be the pivot row.
	*/

	ipvt = jcol;
	big = fabs(a(ipvt,ipvt));
	for (i = ipvt+1; i<n; i++) {
		anext = fabs(a(i,jcol));
		if (anext>big) {
			big = anext;
			ipvt = i;
		}
	}

	temp = fabs(big);
	if(temp <= TINY)
		throw "Matrix is singular";
	
	/*
	*   Interchange pivot row (ipvt) with current row (jcol).
	*/
	
	if (ipvt==jcol) return 0;
	a.swaprows(jcol,ipvt);
	i = order[jcol];
	order[jcol] = order[ipvt];
	order[ipvt] = i;
	return 1;
}

template <class D>
double detv2(Matrix<D>& a)
{
	int n, flag;
	int *ipvt;
	double det;

	if(a.getNumberOfRows() != a.getNumberOfColumns())
		throw "Error: Matrix is not square";

	n = a.getNumberOfRows();

	ipvt = new int[n];

	flag = ludcmp(a,ipvt);

	/* Calculate determinant */
	det = flag;
	for (int i=0; i<n; i++) det *= a(i,i);

	return det;
}

template <class D>
double det(Matrix<D>& op)
{
	int n, flag;
	int *ipvt;
	double det;

	Matrix<D> a = op;

	if(a.getNumberOfRows() != a.getNumberOfColumns())
		throw "Error: Matrix is not square";

	n = a.getNumberOfRows();

	ipvt = new int[n];

	flag = ludcmp(a,ipvt);

	/* Calculate determinant */
	det = flag;
	for (int i=0; i<n; i++) det *= a(i,i);

	return det;
}

template <class T>
class Eye : public Matrix<T>
{
	public:
		Eye(unsigned int rows, unsigned int columns);
		Eye(const Eye& p);
		Matrix<T>& operator=(const Matrix<T>& op2);
};

template <class T>
Eye<T>::Eye(unsigned int rows, unsigned int columns) : Matrix<T>(rows,columns)
{
	if(rows != columns)
		throw "Dimensions do NOT match!!!";

	for(int i = 0; i < rows; i++)
		(*this)(i,i) = 1;
}

template <class T>
Eye<T>::Eye(const Eye<T>& p) : Matrix<T>(p)
{
	// Do nothing but copy p.
}

template <class T>
Matrix<T>& Eye<T>::operator=(const Matrix<T>& op2)
{
	if(op2.getNumberOfColumns() != op2.getNumberOfRows())
		throw "Matrix is not square!!!";

	if(op2.getNumberOfColumns() != this->getNumberOfRows())
		throw "Dimensions do NOT match!!!";

	for(int i = 0; i < this->getNumberOfRows(); i++)
		(*this)(i,i) = op2(i,i);

	return *this;
}

template <class D>
Matrix<D> inv(const Matrix<D>& matrix)
{	
	if(typeid(matrix) == typeid(Eye<D>))
		return invIdentity((const Eye<D>&)matrix);

	else if(typeid(matrix) == typeid(Matrix<D>))
		return invMatrix(matrix);

	else
		throw "Matrix is neither an Eye nor a Normal Matrix";
}

template <class D>
Eye<D> invIdentity(const Eye<D>& eye)
{
	Eye<D> temp(eye.getNumberOfRows(), eye.getNumberOfColumns());	

	for(int i = 0; i < eye.getNumberOfRows(); i++)
		temp(i,i) = 1/eye(i,i);

	return temp;
}

template <class D>
Matrix<D> invMatrix(const Matrix<D>& op2)  
{
	if(op2.getNumberOfColumns() != op2.getNumberOfRows())
		throw "Matrix is not square!!!";

	Matrix<D> temp(op2.getNumberOfRows(), op2.getNumberOfColumns());

	int actualsize = op2.getNumberOfColumns();
	int maxsize = actualsize;

	D* data = new D[actualsize*actualsize];

	for(int i = 0; i < actualsize; i++)
		for(int j = 0; j < actualsize; j++)
			data[i*actualsize + j] = op2(i,j);

    	for (int i=1; i < actualsize; i++) 
		data[i] /= data[0]; // normalize row 0

    	for (int i=1; i < actualsize; i++)  
	{ 
      		for (int j=i; j < actualsize; j++)  // do a column of L
		{ 
        		D sum = 0.0;
        		for (int k = 0; k < i; k++)  
            			sum += data[j*maxsize+k] * data[k*maxsize+i];
        		data[j*maxsize+i] -= sum;
        	}

      		if (i == actualsize-1) continue;
      		
		for (int j=i+1; j < actualsize; j++)  // do a row of U
		{  
        		D sum = 0.0;
        		for (int k = 0; k < i; k++)
            			sum += data[i*maxsize+k]*data[k*maxsize+j];
        		data[i*maxsize+j] = (data[i*maxsize+j]-sum) / data[i*maxsize+i];
        	}
	}

    	for ( int i = 0; i < actualsize; i++ )  // invert L
      		for ( int j = i; j < actualsize; j++ )  
		{
        		D x = 1.0;
        		if ( i != j ) 
			{
          			x = 0.0;
          			for ( int k = i; k < j; k++ ) 
              				x -= data[j*maxsize+k]*data[k*maxsize+i];
          		}
        		data[j*maxsize+i] = x / data[j*maxsize+j];
        	}

    	for ( int i = 0; i < actualsize; i++ )   // invert U
      		for ( int j = i; j < actualsize; j++ )  
		{
        		if ( i == j ) continue;
        		D sum = 0.0;
        		for ( int k = i; k < j; k++ )
            			sum += data[k*maxsize+j]*( (i==k) ? 1.0 : data[i*maxsize+k] );
        		data[i*maxsize+j] = -sum;
        	}

    	for ( int i = 0; i < actualsize; i++ )   // final inversion
      		for ( int j = 0; j < actualsize; j++ )  
		{
        		D sum = 0.0;
        		for ( int k = ((i>j)?i:j); k < actualsize; k++ )  
            			sum += ((j==k)?1.0:data[j*maxsize+k])*data[k*maxsize+i];
        		data[j*maxsize+i] = sum;
        	}

	for(int i = 0; i < actualsize; i++)
		for(int j = 0; j < actualsize; j++)
			temp(i,j) = data[i*actualsize + j];

	delete [] data;

	return temp;
}

#endif
