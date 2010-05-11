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

using namespace std;

float** allocateCMM(unsigned int D)
{
	float** CMM;

	CMM = (float**)malloc(D*sizeof(float*));
	for(int i = 0; i < D; i++)
		CMM[i] = (float*)malloc(D*sizeof(float));

	return (float**)CMM;
}

int main(int argc, char* argv[])
{
	float** CMM = NULL;
	CrossCorrelationMatrix* ccm;
	unsigned int D, N;

	//-------------------------------------------------------------------
	{
		cout << "Test 1" << endl;
		D = 4;
		N = 8;
		float y1[] = {0.0, 0.0, 0.0, 1.0, 2.0, 3.0, 2.0, 1.0, 1.0, 3.0, 2.0};
		float ans[] = {33.0,26.0,17.0,15.0,26.0,29.0,20.0,15.0,17.0,20.0,20.0,17.0,15.0,15.0,17.0,19.0};

		CMM = allocateCMM(D);

		ccm = new CrossCorrelationMatrix(D, N);

		ccm->getCrossCorrelationMatrix(y1,y1,CMM);

		float sum = 0;
		for(int i = 0; i < D; i++)
		{
			for(int j = 0; j < D; j++)
				sum += sqrt(pow((CMM[i][j] - ans[i*D + j]),2));
		}

		if(sum > 0.000006)
			cout << "Test 1: Something went wrong!!!!!" << endl;
		else
			cout << "Test 1: Everything went OK!" << endl;

		delete (void*)ccm;
		free(CMM);
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

		CMM = allocateCMM(D);

		ccm = new CrossCorrelationMatrix(D, N);

		ccm->getCrossCorrelationMatrix(y2,y3,CMM);

		float sum = 0;
		for(int i = 0; i < D; i++)
		{
			for(int j = 0; j < D; j++)
				sum += sqrt(pow((CMM[i][j] - ans[i*D + j]),2));
		}

		if(sum > 0.00005)
			cout << "Test 2: Something went wrong!!!!!" << endl;
		else
			cout << "Test 2: Everything went OK!" << endl;
	
		free(CMM);
		delete (void*)ccm;
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

		CMM = allocateCMM(D);

		ccm = new CrossCorrelationMatrix(D, N);

		ccm->getCrossCorrelationMatrix(y4,y5,CMM);

		float sum = 0;
		for(int i = 0; i < D; i++)
		{
			for(int j = 0; j < D; j++)
				sum += sqrt(pow((CMM[i][j] - ans[i*D + j]),2));
		}

		if(sum > 0.0005)
			cout << "Test 3: Something went wrong!!!!!" << endl;
		else
			cout << "Test 3: Everything went OK!" << endl;

		free(CMM);
		delete (void*)ccm;
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

		CMM = allocateCMM(D);

		ccm = new CrossCorrelationMatrix(D, N);

		ccm->getCrossCorrelationMatrix(y6,y7,CMM);

		float sum = 0;
		for(int i = 0; i < D; i++)
		{
			for(int j = 0; j < D; j++)
				sum += sqrt(pow((CMM[i][j] - ans[i*D + j]),2));
		}

		if(sum > 0.0007)
			cout << "Test 4: Something went wrong!!!!!" << endl;
		else
			cout << "Test 4: Everything went OK!" << endl;

		free(CMM);
		delete (void*)ccm;
	}
	//-------------------------------------------------------------------

	return 0;
}
