#include <math.h>
#include <iostream>
#include <typeinfo>
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
	try
	{
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
	}
	catch(const char* str)
	{
		cout << str << endl;
		result &= false;
	}
	// Testing friend inv ----------------------------

	// Testing operator*() --------------------
	try
	{
		cout << "Testing operator*(Matrix op2)" << endl;
		m = new Matrix<float>(1,4);
		mm = new Matrix<float>(4,3);
		ans = new Matrix<float>(1,3);

		float vector[][3] = {{30,70,110}};

		(*m)(0,0) = 1;
		(*m)(0,1) = 2;
		(*m)(0,2) = 3;
		(*m)(0,3) = 4;

		(*mm)(0,0) = 1;
		(*mm)(0,1) = 5;
		(*mm)(0,2) = 9;
		(*mm)(1,0) = 2;
		(*mm)(1,1) = 6;
		(*mm)(1,2) = 10;
		(*mm)(2,0) = 3;
		(*mm)(2,1) = 7;
		(*mm)(2,2) = 11;
		(*mm)(3,0) = 4;
		(*mm)(3,1) = 8;
		(*mm)(3,2) = 12;

		(*ans) = ((*m) * (*mm));

		sum = 0;
		for(int i=0; i < 1; i++)
			for(int j=0; j < 3; j++)
				sum += ((*ans)(i,j) - vector[i][j]);

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
		m = NULL;
		mm = NULL;
		ans = NULL;
	}
	catch(const char* str)
	{
		cout << str << endl;
		result &= false;
	}
	// Testing operator*() --------------------

	// Testing operator+() --------------------
	try	
	{
		cout << "Testing operator+(Matrix op2)" << endl;
		m = new Matrix<float>(1,4);
		mm = new Matrix<float>(1,4);

		for(int i=0; i < 1; i++)
			for(int j=0; j < 4; j++)
				(*m)(i,j) = j+1;

		for(int i=0; i < 1; i++)
			for(int j=0; j < 4; j++)
				(*mm)(i,j) = j+1;

		(*mm) = ((*m) + (*mm));

		sum = 0;
		for(int i=0; i < 1; i++)
			for(int j=0; j < 4; j++)
				sum += (*mm)(i,j) - 2*(j+1);

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

		delete mm;
		delete m;
		m = NULL;
		mm = NULL;
	}
	catch(const char* str)
	{
		cout << str << endl;
		result &= false;
	}
	// Testing operator+() --------------------

	//Testing friend det ----------------------------
	try
	{
		Matrix<float> *m = new Matrix<float>(3,3);

		(*m)(0,0) = 1;
		(*m)(0,1) = 2;
		(*m)(0,2) = 3;

		(*m)(1,0) = 4;
		(*m)(1,1) = 5;
		(*m)(1,2) = 6;

		(*m)(2,0) = 7;
		(*m)(2,1) = 8;
		(*m)(2,2) = 1;

		if(det((*m)) != 24)
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

	}
	catch(const char* str)
	{
		cout << str << endl;
		result &= false;
		delete m;
	}
	//Testing friend det ----------------------------

	//Testing friend det ----------------------------
	try
	{
		Matrix<float> *m = new Matrix<float>(5,5);

		(*m)(0,0) = 1;
		(*m)(0,1) = 10;
		(*m)(0,2) = 32;
		(*m)(0,3) = 35;
		(*m)(0,4) = 89;

		(*m)(1,0) = 12;
		(*m)(1,1) = 14;
		(*m)(1,2) = 64;
		(*m)(1,3) = 76;
		(*m)(1,4) = 2;

		(*m)(2,0) = 46;
		(*m)(2,1) = 7;
		(*m)(2,2) = 9;
		(*m)(2,3) = 32;
		(*m)(2,4) = 47;

		(*m)(3,0) = 100;
		(*m)(3,1) = 22;
		(*m)(3,2) = 8;
		(*m)(3,3) = 74;
		(*m)(3,4) = 34;

		(*m)(4,0) = 309;
		(*m)(4,1) = 17;
		(*m)(4,2) = 3;
		(*m)(4,3) = 5;
		(*m)(4,4) = 76;

		if(det((*m)) != -421126408)
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

	}
	catch(const char* str)
	{
		cout << str << endl;
		result &= false;
		delete m;
	}
	//Testing friend det ----------------------------

	//Testing friend det ----------------------------
	try
	{
		Matrix<float> *m = new Matrix<float>(4,4);

		(*m)(0,0) = 1;
		(*m)(0,1) = 10;
		(*m)(0,2) = 32;
		(*m)(0,3) = 35;

		(*m)(1,0) = 12;
		(*m)(1,1) = 14;
		(*m)(1,2) = 64;
		(*m)(1,3) = 76;

		(*m)(2,0) = 46;
		(*m)(2,1) = 7;
		(*m)(2,2) = 9;
		(*m)(2,3) = 32;

		(*m)(3,0) = 100;
		(*m)(3,1) = 22;
		(*m)(3,2) = 8;
		(*m)(3,3) = 74;

		if(det((*m)) != 188580)
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

	}
	catch(const char* str)
	{
		cout << str << endl;
		result &= false;
		delete m;
	}
	//Testing friend det ----------------------------


	//Matrix<D> diag(const Matrix<D>& a)
	//Testing friend diag ----------------------------
	try
	{
		Matrix<float> *m = new Matrix<float>(3,3);

		(*m)(0,0) = 1;
		(*m)(0,1) = 2;
		(*m)(0,2) = 3;

		(*m)(1,0) = 4;
		(*m)(1,1) = 5;
		(*m)(1,2) = 6;

		(*m)(2,0) = 7;
		(*m)(2,1) = 8;
		(*m)(2,2) = 1;

		Matrix<float> temp(1,3);
		temp(0,0) = 1;
		temp(0,1) = 5;
		temp(0,2) = 1;

		Matrix<float> ans = diag((*m));

		sum = 0;
		for(int i = 0; i < 3; i++)
			sum += ans(0,i) - temp(0,i);

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

	}
	catch(const char* str)
	{
		cout << str << endl;
		result &= false;
		delete m;
	}
	//Testing friend diag ----------------------------

	//Testing friend diag ----------------------------
	try
	{
		Matrix<float> *m = new Matrix<float>(5,5);

		(*m)(0,0) = 1;
		(*m)(0,1) = 10;
		(*m)(0,2) = 32;
		(*m)(0,3) = 35;
		(*m)(0,4) = 89;

		(*m)(1,0) = 12;
		(*m)(1,1) = 14;
		(*m)(1,2) = 64;
		(*m)(1,3) = 76;
		(*m)(1,4) = 2;

		(*m)(2,0) = 46;
		(*m)(2,1) = 7;
		(*m)(2,2) = 9;
		(*m)(2,3) = 32;
		(*m)(2,4) = 47;

		(*m)(3,0) = 100;
		(*m)(3,1) = 22;
		(*m)(3,2) = 8;
		(*m)(3,3) = 74;
		(*m)(3,4) = 34;

		(*m)(4,0) = 309;
		(*m)(4,1) = 17;
		(*m)(4,2) = 3;
		(*m)(4,3) = 5;
		(*m)(4,4) = 76;

		Matrix<float> temp(1,5);
		temp(0,0) = 1;
		temp(0,1) = 14;
		temp(0,2) = 9;
		temp(0,3) = 74;
		temp(0,4) = 76;

		Matrix<float> ans = diag((*m));

		sum = 0;
		for(int i = 0; i < 5; i++)
			sum += ans(0,i) - temp(0,i);

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

	}
	catch(const char* str)
	{
		cout << str << endl;
		result &= false;
		delete m;
	}
	//Testing friend diag ----------------------------

	//Testing friend diag ----------------------------
	try
	{
		Matrix<float> *m = new Matrix<float>(4,4);

		(*m)(0,0) = 1;
		(*m)(0,1) = 10;
		(*m)(0,2) = 32;
		(*m)(0,3) = 35;

		(*m)(1,0) = 12;
		(*m)(1,1) = 14;
		(*m)(1,2) = 64;
		(*m)(1,3) = 76;

		(*m)(2,0) = 46;
		(*m)(2,1) = 7;
		(*m)(2,2) = 9;
		(*m)(2,3) = 32;

		(*m)(3,0) = 100;
		(*m)(3,1) = 22;
		(*m)(3,2) = 8;
		(*m)(3,3) = 74;

		Matrix<float> temp(1,4);
		temp(0,0) = 1;
		temp(0,1) = 14;
		temp(0,2) = 9;
		temp(0,3) = 74;

		Matrix<float> ans = diag((*m));

		sum = 0;
		for(int i = 0; i < 4; i++)
			sum += ans(0,i) - temp(0,i);

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

	}
	catch(const char* str)
	{
		cout << str << endl;
		result &= false;
		delete m;
	}
	//Testing friend diag ----------------------------

	// friend double var(const Matrix<D>& arr)
	//Testing friend var ----------------------------
	try
	{
		Matrix<float> *m = new Matrix<float>(1,3);

		(*m)(0,0) = 30;
		(*m)(0,1) = 70;
		(*m)(0,2) = 110;

		double res = var((*m));

		if(res != 1600)
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

	}
	catch(const char* str)
	{
		cout << str << endl;
		result &= false;
		delete m;
	}
	//Testing friend var ----------------------------

	//Testing friend var ----------------------------
	try
	{
		Matrix<float> *m = new Matrix<float>(1,4);

		(*m)(0,0) = 2;
		(*m)(0,1) = 4;
		(*m)(0,2) = 6;
		(*m)(0,3) = 8;

		double res = var((*m));

		sum = sqrt(pow((6.666666667 - res),2));

		if(sum >= 0.0000000005)
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

	}
	catch(const char* str)
	{
		cout << str << endl;
		result &= false;
		delete m;
	}
	//Testing friend var ----------------------------

	//Testing friend var ----------------------------
	try
	{
		Matrix<float> *m = new Matrix<float>(1,5);

		(*m)(0,0) = 1;
		(*m)(0,1) = 14;
		(*m)(0,2) = 9;
		(*m)(0,3) = 74;
		(*m)(0,4) = 76;

		double res = var((*m));

		sum = sqrt(pow((1368.70000000 - res),2));

		if(sum >= 0.0000000005)
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

	}
	catch(const char* str)
	{
		cout << str << endl;
		result &= false;
		delete m;
	}
	//Testing friend var ----------------------------

	//Testing friend var ----------------------------
	try
	{
		Eye<float> eye(100,100);

		Matrix<float> dia = diag(eye);

		double res = var(dia);

		sum = sqrt(pow((0.0 - res),2));

		if(sum >= 0.0000000005)
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

	}
	catch(const char* str)
	{
		cout << str << endl;
		result &= false;
		delete m;
	}
	//Testing friend var ----------------------------

	//Testing friend var ----------------------------
	try
	{
		Matrix<float> *m = new Matrix<float>(1,100);
		
		for(int i = 0; i < 100; i++)
			(*m)(0,i) = (float)(i+1)/100;

		double res = var((*m));

		sum = sqrt(pow((0.084167 - res),2));

		if(sum >= 0.0000004)
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

	}
	catch(const char* str)
	{
		cout << str << endl;
		result &= false;
		delete m;
	}
	//Testing friend var ----------------------------

	//Testing friend transpose ----------------------------
	try
	{		
		Matrix<float> *m = new Matrix<float>(3,5);

		(*m)(0,0) = 1;
		(*m)(0,1) = 10;
		(*m)(0,2) = 32;
		(*m)(0,3) = 35;
		(*m)(0,4) = 89;

		(*m)(1,0) = 12;
		(*m)(1,1) = 14;
		(*m)(1,2) = 64;
		(*m)(1,3) = 76;
		(*m)(1,4) = 2;

		(*m)(2,0) = 46;
		(*m)(2,1) = 7;
		(*m)(2,2) = 9;
		(*m)(2,3) = 32;
		(*m)(2,4) = 47;

		Matrix<float> temp(5,3);

		(temp)(0,0) = 1;
		(temp)(0,1) = 12;
		(temp)(0,2) = 46;

		(temp)(1,0) = 10;
		(temp)(1,1) = 14;
		(temp)(1,2) = 7;

		(temp)(2,0) = 32;
		(temp)(2,1) = 64;
		(temp)(2,2) = 9;

		(temp)(3,0) = 35;
		(temp)(3,1) = 76;
		(temp)(3,2) = 32;

		(temp)(4,0) = 89;
		(temp)(4,1) = 2;
		(temp)(4,2) = 47;

		Matrix<float> ans = transpose((*m));
		
		sum = 0;
		for(int i=0; i < 5; i++)
			for(int j=0; j < 3; j++)
				sum += (ans(i,j) - (temp)(i,j));

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

	}
	catch(const char* str)
	{
		cout << str << endl;
		result &= false;
		delete m;
	}
	//Testing friend transpose ----------------------------

	//Testing friend transpose ----------------------------
	try
	{		
		Matrix<float> *m = new Matrix<float>(2,3);

		(*m)(0,0) = 1;
		(*m)(0,1) = 10;
		(*m)(0,2) = 32;

		(*m)(1,0) = 12;
		(*m)(1,1) = 14;
		(*m)(1,2) = 64;

		Matrix<float> temp(3,2);

		(temp)(0,0) = 1;
		(temp)(0,1) = 12;

		(temp)(1,0) = 10;
		(temp)(1,1) = 14;

		(temp)(2,0) = 32;
		(temp)(2,1) = 64;;

		Matrix<float> ans = transpose((*m));
		
		sum = 0;
		for(int i=0; i < 3; i++)
			for(int j=0; j < 2; j++)
				sum += (ans(i,j) - (temp)(i,j));

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

	}
	catch(const char* str)
	{
		cout << str << endl;
		result &= false;
		delete m;
	}
	//Testing friend transpose ----------------------------

	//Testing friend transpose ----------------------------
	try
	{		
		Matrix<float> *m = new Matrix<float>(5,5);

		(*m)(0,0) = 1;
		(*m)(0,1) = 10;
		(*m)(0,2) = 32;
		(*m)(0,3) = 35;
		(*m)(0,4) = 89;

		(*m)(1,0) = 12;
		(*m)(1,1) = 14;
		(*m)(1,2) = 64;
		(*m)(1,3) = 76;
		(*m)(1,4) = 2;

		(*m)(2,0) = 46;
		(*m)(2,1) = 7;
		(*m)(2,2) = 9;
		(*m)(2,3) = 32;
		(*m)(2,4) = 47;

		(*m)(3,0) = 37;
		(*m)(3,1) = 48;
		(*m)(3,2) = 2;
		(*m)(3,3) = 13;
		(*m)(3,4) = 15;

		(*m)(4,0) = 16;
		(*m)(4,1) = 69;
		(*m)(4,2) = 41;
		(*m)(4,3) = 12;
		(*m)(4,4) = 17;

		Matrix<float> temp(5,5);

		(temp)(0,0) = 1;
		(temp)(0,1) = 12;
		(temp)(0,2) = 46;
		(temp)(0,3) = 37;
		(temp)(0,4) = 16;

		(temp)(1,0) = 10;
		(temp)(1,1) = 14;
		(temp)(1,2) = 7;
		(temp)(1,3) = 48;
		(temp)(1,4) = 69;

		(temp)(2,0) = 32;
		(temp)(2,1) = 64;
		(temp)(2,2) = 9;
		(temp)(2,3) = 2;
		(temp)(2,4) = 41;

		(temp)(3,0) = 35;
		(temp)(3,1) = 76;
		(temp)(3,2) = 32;
		(temp)(3,3) = 13;
		(temp)(3,4) = 12;

		(temp)(4,0) = 89;
		(temp)(4,1) = 2;
		(temp)(4,2) = 47;
		(temp)(4,3) = 15;
		(temp)(4,4) = 17;

		Matrix<float> ans = transpose((*m));
		
		sum = 0;
		for(int i=0; i < 5; i++)
			for(int j=0; j < 5; j++)
				sum += (ans(i,j) - (temp)(i,j));

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

	}
	catch(const char* str)
	{
		cout << str << endl;
		result &= false;
		delete m;
	}
	//Testing friend transpose ----------------------------

	//Testing friend transpose ----------------------------
	try
	{		
		Matrix<float> *m = new Matrix<float>(1,5);

		(*m)(0,0) = 1;
		(*m)(0,1) = 10;
		(*m)(0,2) = 32;
		(*m)(0,3) = 35;
		(*m)(0,4) = 89;

		Matrix<float> temp(5,1);

		(temp)(0,0) = 1;
		(temp)(1,0) = 10;
		(temp)(2,0) = 32;
		(temp)(3,0) = 35;
		(temp)(4,0) = 89;

		Matrix<float> ans = transpose((*m));
		
		sum = 0;
		for(int i=0; i < 5; i++)
			for(int j=0; j < 1; j++)
				sum += (ans(i,j) - (temp)(i,j));

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

	}
	catch(const char* str)
	{
		cout << str << endl;
		result &= false;
		delete m;
	}
	//Testing friend transpose ----------------------------

	//Testing friend transpose ----------------------------
	try
	{		
		Matrix<float> *m = new Matrix<float>(5,1);

		(*m)(0,0) = 1;
		(*m)(1,0) = 12;
		(*m)(2,0) = 46;
		(*m)(3,0) = 37;
		(*m)(4,0) = 16;

		Matrix<float> temp(1,5);

		(temp)(0,0) = 1;
		(temp)(0,1) = 12;
		(temp)(0,2) = 46;
		(temp)(0,3) = 37;
		(temp)(0,4) = 16;

		Matrix<float> ans = transpose((*m));
		
		sum = 0;
		for(int i=0; i < 1; i++)
			for(int j=0; j < 5; j++)
				sum += (ans(i,j) - (temp)(i,j));

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

	}
	catch(const char* str)
	{
		cout << str << endl;
		result &= false;
		delete m;
	}
	//Testing friend transpose ----------------------------

	// Testing friend inv ----------------------------
	try
	{
		cout << "Testing friend function inv" << endl;
		Eye<float> eye6(3,3);
		Eye<float> eye7(3,3);

		for(int i = 0; i < 3; i++)
			eye6(i,i) = 0.25;

		eye7 = inv(eye6);

		sum = 0;
		for(int i = 0; i < 3; i++)
			for(int j = 0; j < 3; j++)
			{
				if(i == j)
					sum += eye7(i,j) - 4;
				else
					sum += eye7(i,j);
			}

		eye7.dumpMatrixContents();

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
	catch(const char* str)
	{
		cout << str << endl;
		result &= false;
	}
	// Testing friend inv ----------------------------

	// Testing friend inv ----------------------------
	try
	{
		cout << "Testing friend function inv" << endl;
		Matrix<float> eye6(5,5);
		Matrix<float> eye7(5,5);

		(eye6)(0,0) = 1;
		(eye6)(0,1) = 10;
		(eye6)(0,2) = 32;
		(eye6)(0,3) = 35;
		(eye6)(0,4) = 89;

		(eye6)(1,0) = 12;
		(eye6)(1,1) = 14;
		(eye6)(1,2) = 64;
		(eye6)(1,3) = 76;
		(eye6)(1,4) = 2;

		(eye6)(2,0) = 46;
		(eye6)(2,1) = 7;
		(eye6)(2,2) = 9;
		(eye6)(2,3) = 32;
		(eye6)(2,4) = 47;

		(eye6)(3,0) = 100;
		(eye6)(3,1) = 22;
		(eye6)(3,2) = 8;
		(eye6)(3,3) = 74;
		(eye6)(3,4) = 34;

		(eye6)(4,0) = 309;
		(eye6)(4,1) = 17;
		(eye6)(4,2) = 3;
		(eye6)(4,3) = 5;
		(eye6)(4,4) = 76;

		eye7 = inv(eye6);

		eye7.dumpMatrixContents();

		float ans[] = {-0.00676349, 0.00219877, 0.0110495, -0.00402873, 0.00283163, 0.0980999, -0.0233463, -0.269185, 0.0932753, 0.0104759, 0.00813899, 0.0151845, -0.0164859, -0.0127176, 0.00595392, -0.0240364, 0.00444575, 0.0614119, -0.00574175, -0.00737892, 0.00681562, -0.00460943, 0.0118982, -0.00360449, -0.000447799};

		sum = 0;
		for(int i = 0; i < 5; i++)
			for(int j = 0; j < 5; j++)
				sum += sqrt(pow((eye7(i,j) - ans[i*5+j]),2));

		if(sum > 8e-07)
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
	catch(const char* str)
	{
		cout << str << endl;
		cout << "Something went wrong!!!!!" << endl;
		result &= false;
	}
	// Testing friend inv ----------------------------

	// Testing friend inv ----------------------------
	try
	{
		cout << "Testing friend function inv" << endl;
		Matrix<float> eye6(2,2);
		Matrix<float> eye7(2,2);

		eye6(0,0) = 1;
		eye6(0,1) = 2;

		eye6(1,0) = 3;
		eye6(1,1) = 4;

		eye7 = inv(eye6);

		eye7.dumpMatrixContents();

		float ans[] = {-2, 1, 1.5, -0.5};

		sum = 0;
		for(int i = 0; i < 2; i++)
			for(int j = 0; j < 2; j++)
				sum += eye7(i,j) - ans[i*2+j];

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
	catch(const char* str)
	{
		cout << str << endl;
		cout << "Something went wrong!!!!!" << endl;
		result &= false;
	}
	// Testing friend inv ----------------------------

	if(result)
		cout << "FINAL RESULT: OK!" << endl;
	else
		cout << "FINAL RESULT: NOK!!!!!" << endl;

	return 0;
}
