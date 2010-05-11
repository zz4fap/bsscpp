#include <math.h>
#include <iostream>
#include <string>
#include <string.h>
#include <fstream>
#include <time.h>
#include "Matrix.hh"

using namespace std;

int main()
{
	Matrix<float> *m = NULL;
	Matrix<float> *mm = NULL;
	Matrix<float> *ans = NULL;
	float sum = 0;
	bool result = true;

	// Testing operator() ---------------------
	cout << "Testing operator(int i, int j)" << endl;
	m = new Matrix<float>(3,3);
	float array[3][3];

	for(int i=0; i < 3; i++)
	{
		for(int j=0; j < 3; j++)
		{
			array[i][j] = rand() % 10 + 1;
			(*m)(i,j) = array[i][j];
		}
	}

	sum = 0;
	for(int i=0; i < 3; i++)
		for(int j=0; j < 3; j++)
			sum += (array[i][j] - (*m)(i,j));

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

	delete m;
	m = NULL;
	// Testing operator() ---------------------

	// Testing getInv() -----------------------

	// Testing getInv() -----------------------

	// Testing getNumberOfRows() --------------
	cout << "Testing getNumberOfRows()" << endl;
	m = new Matrix<float>(5,7);
	if(m->getNumberOfRows() == 5)
		cout << "Everything is fine!" << endl;
	else
		cout << "Something went wrong!!!!!" << endl;

	if(m->getNumberOfColumns() == 7)
	{
		cout << "Everything is fine!" << endl;
		result &= true;
	}
	else
	{
		cout << "Something went wrong!!!!!" << endl;
		result &= false;
	}
	delete m;
	m = NULL;
	// Testing getNumberOfRows() --------------

	// Testing operator+() --------------------
	cout << "Testing operator+(Matrix op2)" << endl;
	m = new Matrix<float>(3,3);
	mm = new Matrix<float>(3,3);
	ans = new Matrix<float>(3,3);

	for(int i=0; i < 3; i++)
		for(int j=0; j < 3; j++)
			(*m)(i,j) = 2;

	for(int i=0; i < 3; i++)
		for(int j=0; j < 3; j++)
			(*mm)(i,j) = 3;

	(*ans) = ((*m) + (*mm));

	sum = 0;
	for(int i=0; i < 3; i++)
		for(int j=0; j < 3; j++)
			sum += (*ans)(i,j) - 5;

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

	delete ans;
	delete mm;
	delete m;
	m = NULL;
	mm = NULL;
	ans = NULL;
	// Testing operator+() --------------------

	// Testing operator+() --------------------
	cout << "Testing operator+(int op2)" << endl;
	m = new Matrix<float>(4,4);

	for(int i=0; i < 4; i++)
		for(int j=0; j < 4; j++)
			(*m)(i,j) = 2;

	(*m) = ((*m) + 1);

	sum = 0;
	for(int i=0; i < 4; i++)
		for(int j=0; j < 4; j++)
			sum += (*m)(i,j) - 3;

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

	delete m;
	m = NULL;
	// Testing operator+() --------------------

	// Testing operator-() --------------------
	cout << "Testing operator-(Matrix op2)" << endl;
	m = new Matrix<float>(4,4);
	mm = new Matrix<float>(4,4);
	ans = new Matrix<float>(4,4);

	for(int i=0; i < 4; i++)
		for(int j=0; j < 4; j++)
			(*m)(i,j) = 4;

	for(int i=0; i < 4; i++)
		for(int j=0; j < 4; j++)
			(*mm)(i,j) = 1;

	(*ans) = ((*m) - (*mm));

	sum = 0;
	for(int i=0; i < 4; i++)
		for(int j=0; j < 4; j++)
			sum += (*ans)(i,j) - 3;

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

	delete m;
	delete mm;
	delete ans;
	m = NULL;
	mm = NULL;
	ans = NULL;
	// Testing operator-() --------------------

	// Testing operator-() --------------------
	cout << "Testing operator-(int op2)" << endl;
	m = new Matrix<float>(15,15);

	for(int i=0; i < 15; i++)
		for(int j=0; j < 15; j++)
			(*m)(i,j) = 19;

	(*m) = ((*m) - 1);

	sum = 0;
	for(int i=0; i < 15; i++)
		for(int j=0; j < 15; j++)
			sum += (*m)(i,j) - 18;

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

	delete m;
	m = NULL;
	// Testing operator-() --------------------

	// Testing operator*() --------------------
	cout << "Testing operator*(Matrix op2)" << endl;
	m = new Matrix<float>(4,4);
	mm = new Matrix<float>(4,3);

	float vector[][3] = {{43,80,144},{79,160,295},{144,381,394},{177,660,399}};

	(*m)(0,0) = 1;
	(*m)(0,1) = 2;
	(*m)(0,2) = 3;
	(*m)(0,3) = 4;
	(*m)(1,0) = 6;
	(*m)(1,1) = 7;
	(*m)(1,2) = 1;
	(*m)(1,3) = 3;
	(*m)(2,0) = 10;
	(*m)(2,1) = 8;
	(*m)(2,2) = 9;
	(*m)(2,3) = 2;
	(*m)(3,0) = 21;
	(*m)(3,1) = 4;
	(*m)(3,2) = 12;
	(*m)(3,3) = 7;

	(*mm)(0,0) = 3;
	(*mm)(0,1) = 21;
	(*mm)(0,2) = 4;
	(*mm)(1,0) = 7;
	(*mm)(1,1) = 2;
	(*mm)(1,2) = 32;
	(*mm)(2,0) = 6;
	(*mm)(2,1) = 17;
	(*mm)(2,2) = 8;
	(*mm)(3,0) = 2;
	(*mm)(3,1) = 1;
	(*mm)(3,2) = 13;

	(*mm) = ((*m) * (*mm));

	sum = 0;
	for(int i=0; i < 4; i++)
		for(int j=0; j < 3; j++)
			sum += ((*mm)(i,j) - vector[i][j]);

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

	delete m;
	delete mm;
	delete ans;
	m = NULL;
	mm = NULL;
	ans = NULL;
	// Testing operator*() --------------------

	// Testing operator*(int op2) --------------------
	cout << "Testing operator*(int op2)" << endl;
	m = new Matrix<float>(55,67);

	for(int i=0; i < 55; i++)
		for(int j=0; j < 67; j++)
			(*m)(i,j) = 34;

	(*m) = ((*m) * 2);

	sum = 0;
	for(int i=0; i < 55; i++)
		for(int j=0; j < 67; j++)
			sum += (*m)(i,j) - 68;

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

	delete m;
	m = NULL;
	// Testing operator*(int op2) --------------------

	// Testing operator/(int op2) --------------------
	cout << "Testing operator/(int op2)" << endl;
	m = new Matrix<float>(155,267);

	for(int i=0; i < 155; i++)
		for(int j=0; j < 267; j++)
			(*m)(i,j) = 44;

	(*m) = ((*m) / 3);

	sum = 0;
	for(int i=0; i < 155; i++)
		for(int j=0; j < 267; j++)
			sum += sqrt(pow(((*m)(i,j) - 14.66666666667),2));

	sum /= (155*267);

	if(sum > 0.0000004)
	{
		cout << "Something went wrong!!!!!" << endl;
		result &= false;
	}
	else
	{
		cout << "Everything is fine!" << endl;
		result &= true;
	}

	delete m;
	m = NULL;
	// Testing operator/(int op2) --------------------

	// Testing Eye------------------------------------
	cout << "Testing Eye" << endl;
	Eye<float>* eye = new Eye<float>(5,5);

	try
	{
		Eye<float> eye2(5,5);
		eye2 = eye2 * 5;

		sum = 0;
		for(int i = 0; i < 5; i++)
			for(int j = 0; j < 5; j++)
			{
				if(i == j)
					sum += eye2(i,j) - 5;
				else
					sum += eye2(i,j);
			}
	}
	catch(char* str)
	{
		cout << str << endl;
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
	// Testing Eye------------------------------------

	// Testing Eye------------------------------------
	cout << "Testing Eye" << endl;

	try
	{
		Eye<float> eye(128,128);

		sum = 0;
		for(int i = 0; i < 128; i++)
			for(int j = 0; j < 128; j++)
			{
				if(i == j)
					sum += eye(i,j) - 1;
				else
					sum += eye(i,j);
			}
	}
	catch(char* str)
	{
		cout << str << endl;
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
	// Testing Eye------------------------------------

	// Testing Eye------------------------------------
	cout << "Testing Eye" << endl;

	try
	{
		Eye<float> eye3(128,128);
		eye3 = eye3 * 78;

		Eye<float> eye4(128,128);

		eye4 = eye3;

		sum = 0;
		for(int i = 0; i < 128; i++)
			for(int j = 0; j < 128; j++)
			{
				if(i == j)
					sum += eye4(i,j) - 78;
				else
					sum += eye4(i,j);
			}
	}
	catch(char* str)
	{
		cout << str << endl;
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
	// Testing Eye------------------------------------

	// Testing Eye------------------------------------
	cout << "Testing Eye" << endl;

	try
	{
		Eye<float> eye5(3,3);

		eye5(0,0) = 1;
		eye5(0,1) = 2;
		eye5(0,2) = 3;

		eye5(1,0) = 4;
		eye5(1,1) = 5;
		eye5(1,2) = 6;

		eye5(2,0) = 7;
		eye5(2,1) = 8;
		eye5(2,2) = 9;

		sum = 0;
		for(int i = 0; i < 3; i++)
			for(int j = 0; j < 3; j++)
				sum += eye5(i,j) - (i*3+j+1);
	}
	catch(char* str)
	{
		cout << str << endl;
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
	// Testing Eye------------------------------------

	// Testing friend inv ----------------------------
	cout << "Testing friend function inv" << endl;
	Eye<float> eye6(13,13);
	Eye<float> eye7(13,13);

	for(int i = 0; i < 13; i++)
		eye6(i,i) = 0.1;

	eye7 = inv(eye6);

	sum = 0;
	for(int i = 0; i < 13; i++)
		for(int j = 0; j < 13; j++)
		{
			if(i == j)
				sum += eye7(i,j) - 10;
			else
				sum += eye7(i,j);
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
	// Testing friend inv ----------------------------

	if(result)
		cout << "FINAL RESULT: OK!" << endl;
	else
		cout << "FINAL RESULT: NOK!!!!!" << endl;

	return 0;
}
