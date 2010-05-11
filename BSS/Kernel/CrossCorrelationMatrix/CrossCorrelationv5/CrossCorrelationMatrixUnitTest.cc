#include <math.h>
#include <iostream>
#include <string>
#include <string.h>
#include <fstream>
#include <iomanip>
#include <stdlib.h>
#include "Fourier.hh"
#include "Correlation.hh"
#include "CrossCorrelationMatrix.hh"
#include "Matrix.hh"

using namespace std;

int main(int argc, char* argv[])
{
	CrossCorrelationMatrix<float>* ccm;
	unsigned int D, N;
	bool res = true;

	//-------------------------------------------------------------------
	{
		cout << "Test 1" << endl;
		D = 4;
		N = 8;
		float y1[] = {0.0, 0.0, 0.0, 1.0, 2.0, 3.0, 2.0, 1.0, 1.0, 3.0, 2.0};
		float ans[] = {33.0,26.0,17.0,15.0,26.0,29.0,20.0,15.0,17.0,20.0,20.0,17.0,15.0,15.0,17.0,19.0};

		Matrix<float> CMM(D,D);

		ccm = new CrossCorrelationMatrix<float>(D, N);

		ccm->getCrossCorrelationMatrix(y1,y1,CMM);

		float sum = 0;
		for(int i = 0; i < D; i++)
		{
			for(int j = 0; j < D; j++)
				sum += sqrt(pow((CMM(i,j) - ans[i*D + j]),2));
		}

		if(sum > 0.000006)
		{
			cout << "Test 1: Something went wrong!!!!!" << endl;
			res &= false;
		}
		else
		{
			cout << "Test 1: Everything went OK!" << endl;
			res &= true;
		}

		delete ccm;
	}
	//-------------------------------------------------------------------

	//-------------------------------------------------------------------
	{
		cout << "Test 2" << endl;
		D = 4;
		N = 8;
		float y2[] = {0.0, 0.0, 0.0, 2.0, 1.0, 2.0, 3.0, 3.0, 2.0, 1.0, 2.0};
		float y3[] = {0.0, 0.0, 0.0, 5.0, 2.0, 6.0, 7.0, 8.0, 1.0, 4.0, 9.0};
		float ans[] = {93.0,73.0,58.0,56.0,68.0,75.0,65.0,56.0,68.0,59.0,71.0,64.0,63.0,50.0,51.0,69.0};

		Matrix<float> CMM(D,D);

		ccm = new CrossCorrelationMatrix<float>(D, N);

		ccm->getCrossCorrelationMatrix(y2,y3,CMM);

		float sum = 0;
		for(int i = 0; i < D; i++)
		{
			for(int j = 0; j < D; j++)
				sum += sqrt(pow((CMM(i,j) - ans[i*D + j]),2));
		}

		if(sum > 0.00005)
		{
			cout << "Test 2: Something went wrong!!!!!" << endl;
			res &= false;
		}
		else
		{
			cout << "Test 2: Everything went OK!" << endl;
			res &= true;
		}

		delete ccm;
	}
	//-------------------------------------------------------------------

	//-------------------------------------------------------------------
	{
		cout << "Test 3" << endl;
		N = 16;
		D = 3;
		float y4[] = {0.0, 0.0, 9.0, 7.0, 5.0, 3.0, 1.0, 13.0, 11.0, 10.0, 4.0, 2.0, 3.0, 12.0, 15.0, 6.0, 3.0, 8.0};
		float y5[] = {0.0, 0.0, 21.0, 5.0, 27.0, 8.0, 2.0, 16.0, 6.0, 10.0, 17.0, 33.0, 8.0, 11.0, 6.0, 12.0, 4.0, 9.0};
		float ans[] = {1295.0,1061.0,1224.0,1138.0,1223.0,1029.0,1340.0,1111.0,1211.0};

		Matrix<float> CMM(D,D);

		ccm = new CrossCorrelationMatrix<float>(D, N);

		ccm->getCrossCorrelationMatrix(y4,y5,CMM);

		float sum = 0;
		for(int i = 0; i < D; i++)
		{
			for(int j = 0; j < D; j++)
				sum += sqrt(pow((CMM(i,j) - ans[i*D + j]),2));
		}

		if(sum > 0.0005)
		{
			cout << "Test 3: Something went wrong!!!!!" << endl;
			res &= false;
		}
		else
		{
			cout << "Test 3: Everything went OK!" << endl;
			res &= true;
		}

		delete ccm;
		ccm = NULL;
	}
	//-------------------------------------------------------------------

	//-------------------------------------------------------------------
	{
		cout << "Test 4" << endl;
		N = 16;
		D = 4;
		float y6[] = {0.0, 0.0, 0.0, 9.0, 7.0, 5.0, 3.0, 1.0, 13.0, 11.0, 10.0, 4.0, 2.0, 3.0, 12.0, 15.0, 6.0, 3.0, 8.0};
		float y7[] = {0.0, 0.0, 0.0, 21.0, 5.0, 27.0, 8.0, 2.0, 16.0, 6.0, 10.0, 17.0, 33.0, 8.0, 11.0, 6.0, 12.0, 4.0, 9.0};
		float ans[] = {1295.0,1061.0,1224.0,1461.0,1138.0,1223.0,1029.0,1128.0,1340.0,1111.0,1211.0,993.0,1133.0,1286.0,1087.0,1139.0};

		Matrix<float> CMM(D,D);

		ccm = new CrossCorrelationMatrix<float>(D, N);

		ccm->getCrossCorrelationMatrix(y6,y7,CMM);

		float sum = 0;
		for(int i = 0; i < D; i++)
		{
			for(int j = 0; j < D; j++)
				sum += sqrt(pow((CMM(i,j) - ans[i*D + j]),2));
		}

		if(sum > 0.0007)
		{
			cout << "Test 4: Something went wrong!!!!!" << endl;
			res &= false;
		}
		else
		{
			cout << "Test 4: Everything went OK!" << endl;
			res &= true;
		}

		delete ccm;
		ccm = NULL;
	}
	//-------------------------------------------------------------------

	//-------------------------------------------------------------------
	{
		cout << "Test 5" << endl;
		D = 4;
		N = 8;
		float y8[] = {0.0, 0.0, 0.0, 2, 1, 2, 3, 3, 2, 1, 2};
		float y9[] = {0.0, 0.0, 0.0, 5, 2, 6, 7, 8, 1, 4, 9};
		float ans[] = {93.0,73.0,58.0,56.0,68.0,75.0,65.0,56.0,68.0,59.0,71.0,64.0,63.0,50.0,51.0,69.0};

		Matrix<float> CMM(D,D);

		CrossCorrelationMatrix<float>* ccmm = new CrossCorrelationMatrix<float>(D, N);

		ccmm->getCrossCorrelationMatrix(y8,y9,CMM);

		float sum = 0;
		for(int i = 0; i < D; i++)
		{
			for(int j = 0; j < D; j++)
				sum += sqrt(pow((CMM(i,j) - ans[i*D + j]),2));
		}

		if(sum > 0.00005)
		{
			cout << "Test 5: Something went wrong!!!!!" << endl;
			res &= false;
		}
		else
		{
			cout << "Test 5: Everything went OK!" << endl;
			res &= true;
		}
	
		delete ccmm;
	}
	//-------------------------------------------------------------------

	//-------------------------------------------------------------------
	if(1)
	{
		cout << "Test 6" << endl;
		N = 8;
		D = 4;
		float y[] = {2,3,4,5,6,7,8,9,10,11,12};
		float ans[] = {620, 552, 484, 416, 552, 492, 432, 372, 484, 432, 380, 328, 416, 372, 328, 284};

		Matrix<float> CMM(D,D);

		ccm = new CrossCorrelationMatrix<float>(D, N);

		ccm->getCrossCorrelationMatrix(y,y,CMM);

		float sum = 0;
		for(int i = 0; i < D; i++)
		{
			for(int j = 0; j < D; j++)
				sum += sqrt(pow((CMM(i,j) - ans[i*D + j]),2));
		}

		if(sum > 0.0007)
		{
			cout << "Test 6: Something went wrong!!!!!" << endl;
			res &= false;
		}
		else
		{
			cout << "Test 6: Everything went OK!" << endl;
			res &= true;
		}

		delete ccm;
	}
	//-------------------------------------------------------------------

	//-------------------------------------------------------------------
	if(1)
	{
		cout << "Test 7" << endl;
		N = 16;
		D = 4;
		float y6[] = {0.0, 0.0, 0.0, 9.0, 7.0, 5.0, 3.0, 1.0, 13.0, 11.0, 10.0, 4.0, 2.0, 3.0, 12.0, 15.0, 6.0, 3.0, 8.0};
		float y7[] = {0.0, 0.0, 0.0, 21.0, 5.0, 27.0, 8.0, 2.0, 16.0, 6.0, 10.0, 17.0, 33.0, 8.0, 11.0, 6.0, 12.0, 4.0, 9.0};
		float ans[] = {1295.0,1061.0,1224.0,1461.0,1138.0,1223.0,1029.0,1128.0,1340.0,1111.0,1211.0,993.0,1133.0,1286.0,1087.0,1139.0};

		Matrix<float> CMM(D,D);

		ccm = new CrossCorrelationMatrix<float>(D, N);

		ccm->getCrossCorrelationMatrix(y6,y7,CMM);

		float sum = 0;
		for(int i = 0; i < D; i++)
		{
			for(int j = 0; j < D; j++)
				sum += sqrt(pow((CMM(i,j) - ans[i*D + j]),2));
		}

		if(sum > 0.0007)
		{
			cout << "Test 7: Something went wrong!!!!!" << endl;
			res &= false;
		}
		else
		{
			cout << "Test 7: Everything went OK!" << endl;
			res &= true;
		}

		delete ccm;
	}
	//-------------------------------------------------------------------

	//-------------------------------------------------------------------
	if(1)
	{
		cout << "Test 8" << endl;
		N = 16;
		D = 4;
		float y[] = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
		float ans[] = {16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16};

		Matrix<float> CMM(D,D);

		ccm = new CrossCorrelationMatrix<float>(D, N);

		ccm->getCrossCorrelationMatrix(y,y,CMM);

		//CMM.dumpMatrixContents();

		float sum = 0;
		for(int i = 0; i < D; i++)
		{
			for(int j = 0; j < D; j++)
				sum += sqrt(pow((CMM(i,j) - ans[i*D + j]),2));
		}

		if(sum > 0.0007)
		{
			cout << "Test 8: Something went wrong!!!!!" << endl;
			res &= false;
		}
		else
		{
			cout << "Test 8: Everything went OK!" << endl;
			res &= true;
		}

		delete ccm;
	}
	//-------------------------------------------------------------------

	cout << "FINAL RESULT: " << (res?"OK!":"NOK!!!!!!!!!") << endl;

	return 0;
}
