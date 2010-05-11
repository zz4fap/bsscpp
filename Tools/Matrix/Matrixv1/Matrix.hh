#ifndef MATRIX_HH
#define MATRIX_HH

#include <math.h>
#include <iostream>
#include <string>
#include <string.h>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

template <class T>
class Matrix
{
	protected:
		T** matrix;
		unsigned int rows;
		unsigned int columns;

	public:
		Matrix(unsigned int rows, unsigned int columns);
		~Matrix();
		unsigned int getNumberOfRows() const;
		unsigned int getNumberOfColumns() const;
		Matrix& operator=(const Matrix& op2);
		T& operator()(unsigned int row, unsigned int col);
		T operator()(unsigned int row, unsigned int col) const;
		Matrix operator+(const Matrix& op2);
		Matrix operator+(int op2);
		Matrix operator-(const Matrix& op2);
		Matrix operator-(int op2);
		Matrix operator*(const Matrix& op2);
		Matrix operator*(int op2);
		Matrix operator/(Matrix op2);
		Matrix operator/(int op2);
};

template <class T>
Matrix<T>::Matrix(unsigned int rows, unsigned int columns)
{
	this->rows = rows;
	this->columns = columns;

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
Matrix<T>::~Matrix()
{
	for(int i = 0; i < this->rows; i++)
		free(this->matrix[i]);

	free(this->matrix);
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
Matrix<T> Matrix<T>::operator+(const Matrix& op2)
{
	Matrix temp(this->rows,this->columns);

	if(this->columns != op2.getNumberOfRows() || this->rows != op2.getNumberOfColumns())
		throw "Dimensions do NOT match!!!";

	for(int i = 0; i < rows; i++)
		for(int j = 0; j < columns; j++)
			temp(i,j) = this->matrix[i][j] + op2(i,j);

	return temp;
}

template <class T>
Matrix<T> Matrix<T>::operator+(int op2)
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

	if(this->columns != op2.getNumberOfRows() || this->rows != op2.getNumberOfColumns())
		throw "Dimensions do NOT match!!!";

	for(int i = 0; i < rows; i++)
		for(int j = 0; j < columns; j++)
			temp(i,j) = this->matrix[i][j] - op2(i,j);

	return temp;
}

template <class T>
Matrix<T> Matrix<T>::operator-(int op2)
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
Matrix<T> Matrix<T>::operator*(int op2)
{
	Matrix temp(this->rows,this->columns);

	for(int i = 0; i < rows; i++)
		for(int j = 0; j < columns; j++)
			temp(i,j) = this->matrix[i][j] * op2;

	return temp;
}

template <class T>
Matrix<T> Matrix<T>::operator/(int op2)
{
	Matrix temp(this->rows,this->columns);

	for(int i = 0; i < rows; i++)
		for(int j = 0; j < columns; j++)
			temp(i,j) = this->matrix[i][j] / op2;

	return temp;
}

template <class T>
class Eye : public Matrix<T>
{
	public:
		Eye(unsigned int rows, unsigned int columns);
		Matrix<T>& operator=(const Matrix<T>& op2);
		T& operator()(unsigned int row, unsigned int col);
		void getInv(T** invMatrix);
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

//TODO: Only the main diagonal should be updated with values while all other values MUST be zero!!!!
template <class T>
T& Eye<T>::operator()(unsigned int row, unsigned int col)
{
	if (row >= this->getNumberOfRows() || col >= this->getNumberOfColumns())
     		throw "Matrix subscript out of bounds";
   
	//TODO: Only the main diagonal should be updated with values all the other values MUST be zero!!!!
	//if(row != col)
		//throw "Setting value out of the main diagonal";

	return this->matrix[row][col];
}

//TODO: Implement the method which inverts identity matrices
template <class T>
void Eye<T>::getInv(T** invMatrix)
{

}

#endif
