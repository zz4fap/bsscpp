#include <math.h>
#include <iostream>
#include <string>
#include <string.h>
#include <fstream>
#include <stdlib.h>
#include "Fourier.hh"
#include "Correlation.hh"

using namespace std;

int main(int argc, char* argv[])
{
	int N = 8;
	float sum = 0;

	Correlation<float>* corr = new Correlation<float>();

	float data1[] = {1.0, 2.0, 3.0, 2.0, 1.0, 1.0, 3.0, 2.0};

	float data2[] = {2.0, 1.0, 2.0, 3.0, 3.0, 2.0, 1.0, 2.0};

	float data3[] = {5.0, 2.0, 6.0, 7.0, 8.0, 1.0, 4.0, 9.0};

	float ans12[] = {28.0, 25.0, 26.0, 22.0, 15.0, 9.0, 8.0, 4.0, 0.0, 2.0, 5.0, 10.0, 14.0, 19.0, 24.0, 29.0};
	
	float ans13[] = {80.0, 54.0, 58.0, 55.0, 39.0, 23.0, 19.0, 10.0, 0.0, 9.0, 22.0, 36.0, 40.0, 43.0, 59.0, 83.0};

	float ans32[] = {93.0, 68.0, 68.0, 63.0, 52.0, 24.0, 17.0, 18.0, 0.0, 10.0, 9.0, 24.0, 39.0, 56.0, 58.0, 73.0};
	
	float ans11[] = {33.0, 26.0, 17.0, 15.0, 16.0, 13.0, 7.0, 2.0, 0.0, 2.0, 7.0, 13.0, 16.0, 15.0, 17.0, 26.0};

	float ans[4*N];

	//---------------------------------------------------
	corr->doCorrelation(data1, data2, N, 2*N, ans);
	sum = 0.0;
	for(int i=0;i<2*N;i++)
		sum += sqrt(pow((N*ans[2*i] - ans12[i]),2));
	if(sum >= 0.0003)
		cout << "SOMETHING WENT WRONG!!!!!" << endl;
	else
		cout << "CORR(y1,y2): OK!" << endl;
	//---------------------------------------------------	
	
	//---------------------------------------------------
	memset(ans,0,4*N);
	corr->doCorrelation(data1, data3, N, 2*N, ans);
	sum = 0.0;
	for(int i=0;i<2*N;i++)
		sum += sqrt(pow((N*ans[2*i] - ans13[i]),2));
	if(sum >= 0.0003)
		cout << "SOMETHING WENT WRONG!!!!!" << endl;
	else
		cout << "CORR(y1,y3): OK!" << endl;
	//---------------------------------------------------	

	//---------------------------------------------------
	memset(ans,0,4*N);
	corr->doCorrelation(data3, data2, N, 2*N, ans);
	sum = 0.0;
	for(int i=0;i<2*N;i++)
		sum += sqrt(pow((N*ans[2*i] - ans32[i]),2));
	if(sum >= 0.0003)
		cout << "SOMETHING WENT WRONG!!!!!" << endl;
	else
		cout << "CORR(y3,y2): OK!" << endl;
	//---------------------------------------------------	

	//---------------------------------------------------	
	memset(ans,0,4*N);
	corr->doAutoCorrelation(data1, N, 2*N, ans);
	sum = 0.0;
	for(int i=0;i<2*N;i++)
		sum += sqrt(pow((N*ans[2*i] - ans11[i]),2));
	if(sum >= 0.0003)
		cout << "SOMETHING WENT WRONG!!!!!" << endl;
	else
		cout << "CORR(y1,y1): OK!" << endl;
	//---------------------------------------------------

	//---------------------------------------------------
	{
		int N = 8;
		double y1[] = {0,0,0,0,3,4,5,6};
		double y2[] = {0,0,9,8,7,6,5,4};
		double ans[4*N];
		double ans12[] = {94, 112, 130, 118, 93, 54, 8.88178e-15, 1.42109e-14, 1.42109e-14, 7.10543e-15, 2.84217e-14, 1.42109e-14, 1.42109e-14, 12, 31, 58};
		double sum;

		Correlation<double>* corr = new Correlation<double>();

		memset(ans,0,4*N);
		corr->doCorrelation(y1, y2, N, 2*N, ans);

		sum = 0.0;
		for(int i=0;i<2*N;i++)
			sum += sqrt(pow((N*ans[2*i] - ans12[i]),2));

		if(sum >= 0.0003)
			cout << "SOMETHING WENT WRONG!!!!!" << endl;
		else
			cout << "CORR(y1,y2): OK!" << endl;
	}
	//---------------------------------------------------

	//---------------------------------------------------
	{
		int N = 32;
		double y1[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
		double y2[] = {0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
		double ans[4*N];
		double sum;
		double ans12[] = {16, 16, 16, 16, 16, 16, 16, 16, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 8.88178e-16, 1.77636e-15, 3.55271e-15, 3.55271e-15, 2.66454e-15, 3.55271e-15, 3.55271e-15, 2.66454e-15, 2.66454e-15, 4.44089e-15, 3.55271e-15, 4.44089e-15, 3.55271e-15, 2.66454e-15, 1.77636e-15, 3.55271e-15, 8.88178e-16, 3.55271e-15, 4.44089e-15, 5.32907e-15, 3.55271e-15, 5.32907e-15, 5.32907e-15, 5.32907e-15, 1.77636e-15, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};

		Correlation<double>* corr = new Correlation<double>();

		memset(ans,0,4*N);
		corr->doCorrelation(y1, y2, N, 2*N, ans);

		sum = 0.0;
		for(int i=0;i<2*N;i++)
			sum += sqrt(pow((N*ans[2*i] - ans12[i]),2));
		if(sum >= 0.0003)
			cout << "SOMETHING WENT WRONG!!!!!" << endl;
		else
			cout << "CORR(y1,y2): OK!" << endl;
	}
	//---------------------------------------------------	

	//---------------------------------------------------
	{
		int N = 16;
		double y1[] = {0,0,0,0,0,0,0,0,1,2,3,4,5,6,7,8};
		double y2[] = {0,0,0,0,9,8,7,6,5,4,3,2,1,12,7,14};
		double ans[4*N];
		double sum;
		double ans12[] = {268, 196, 180, 120, 156, 182, 197, 200, 190, 166, 127, 72, 2.84217e-14, 4.26326e-14, 8.52651e-14, 8.52651e-14, 5.68434e-14, 5.68434e-14, 5.68434e-14, 2.84217e-14, 2.84217e-14, 2.84217e-14, 4.26326e-14, 5.68434e-14, 5.68434e-14, 14, 35, 68, 102, 138, 177, 220};

		Correlation<double>* corr = new Correlation<double>();

		memset(ans,0,4*N);
		corr->doCorrelation(y1, y2, N, 2*N, ans);

		sum = 0.0;
		for(int i=0;i<2*N;i++)
			sum += sqrt(pow((N*ans[2*i] - ans12[i]),2));
		if(sum >= 0.0003)
			cout << "SOMETHING WENT WRONG!!!!!" << endl;
		else
			cout << "CORR(y1,y2): OK!" << endl;
	}
	//---------------------------------------------------	

	//---------------------------------------------------
	{
		int N = 8;
		double y1[] = {0,0,0,2,3,4,5,6};
		double y2[] = {0,0,0,0,3,4,5,6};
		double ans[4*N];
		double sum;
		double ans12[] = {86, 62, 39, 18, 8.88178e-16, 1.77636e-15, 0, 0, 0, 1.42109e-14, 0, 1.24345e-14, 12, 28, 47, 68};

		Correlation<double>* corr = new Correlation<double>();

		memset(ans,0,4*N);
		corr->doCorrelation(y1, y2, N, 2*N, ans);

		sum = 0.0;
		for(int i=0;i<2*N;i++)
			sum += sqrt(pow((N*ans[2*i] - ans12[i]),2));
		if(sum >= 0.0003)
			cout << "SOMETHING WENT WRONG!!!!!" << endl;
		else
			cout << "CORR(y1,y2): OK!" << endl;
	}
	//---------------------------------------------------

	//---------------------------------------------------
	{
		int N = 8;
		double y1[] = {0,0,0,2,3,4,5,6};
		double y2[] = {0,0,0,0,2,3,4,5};
		double ans[4*N];
		double sum;
		double ans12[] = {68, 47, 28, 12, 1.77636e-15, 3.55271e-15, 3.55271e-15, 7.10543e-15, 1.42109e-14, 1.06581e-14, 5.32907e-15, 9.76996e-15, 10, 23, 38, 54};

		Correlation<double>* corr = new Correlation<double>();

		memset(ans,0,4*N);
		corr->doCorrelation(y1, y2, N, 2*N, ans);

		sum = 0.0;
		for(int i=0;i<2*N;i++)
			sum += sqrt(pow((N*ans[2*i] - ans12[i]),2));
		if(sum >= 0.0003)
			cout << "SOMETHING WENT WRONG!!!!!" << endl;
		else
			cout << "CORR(y1,y2): OK!" << endl;
	}
	//---------------------------------------------------

	//---------------------------------------------------
	{
		int N = 16;
		double y1[] = {0,0,0,0,0,2,3,4,5,6,7,8,9,10,11,12};
		double y2[] = {0,0,0,0,0,0,0,0,5,6,7,8,9,10,11,12};
		double ans[4*N];
		double sum;
		double ans12[] = {620, 532, 445, 360, 278, 200, 127, 60, 7.81597e-14, 4.26326e-14, 7.10543e-14, 8.52651e-14, 1.13687e-13, 8.52651e-14, 1.42109e-13, 1.7053e-13, 5.68434e-14, 5.68434e-14, 5.68434e-14, 5.68434e-14, 2.84217e-14, 5.68434e-14, 24, 58, 101, 152, 210, 274, 343, 416, 484, 552};

		Correlation<double>* corr = new Correlation<double>();

		memset(ans,0,4*N);
		corr->doCorrelation(y1, y2, N, 2*N, ans);

		sum = 0.0;
		for(int i=0;i<2*N;i++)
			sum += sqrt(pow((N*ans[2*i] - ans12[i]),2));
		if(sum >= 0.0003)
			cout << "SOMETHING WENT WRONG!!!!!" << endl;
		else
			cout << "CORR(y1,y2): OK!" << endl;
	}
	//---------------------------------------------------

	//---------------------------------------------------
	{
		int N = 16;
		double y1[] = {0,0,0,0,0,2,3,4,5,6,7,8,9,10,11,12};
		double y2[] = {0,0,0,0,0,0,0,0,4,5,6,7,8,9,10,11};
		double ans[4*N];
		double sum;
		double ans12[] = {552, 469, 388, 310, 236, 167, 104, 48, 4.9738e-14, 2.84217e-14, 5.68434e-14, 2.84217e-14, 5.68434e-14, 8.52651e-14, 1.42109e-13, 1.42109e-13, 1.13687e-13, 5.68434e-14, 5.68434e-14, 5.68434e-14, 2.84217e-14, 4.26326e-14, 22, 53, 92, 138, 190, 247, 308, 372, 432, 492};

		Correlation<double>* corr = new Correlation<double>();

		memset(ans,0,4*N);
		corr->doCorrelation(y1, y2, N, 2*N, ans);

		sum = 0.0;
		for(int i=0;i<2*N;i++)
			sum += sqrt(pow((N*ans[2*i] - ans12[i]),2));
		if(sum >= 0.0003)
			cout << "SOMETHING WENT WRONG!!!!!" << endl;
		else
			cout << "CORR(y1,y2): OK!" << endl;
	}
	//---------------------------------------------------

	//---------------------------------------------------
	{
		int N = 16;
		double y1[] = {0,0,0,0,0,2,3,4,5,6,7,8,9,10,11,12};
		double y2[] = {0,0,0,0,0,0,0,0,3,4,5,6,7,8,9,10};
		double ans[4*N];
		double sum;
		double ans12[] = {484, 406, 331, 260, 194, 134, 81, 36, 4.9738e-14, 3.55271e-14, 8.52651e-14, 5.68434e-14, 8.52651e-14, 8.52651e-14, 8.52651e-14, 1.13687e-13, 8.52651e-14, 5.68434e-14, 8.52651e-14, 5.68434e-14, 4.26326e-14, 2.84217e-14, 20, 48, 83, 124, 170, 220, 273, 328, 380, 432};

		Correlation<double>* corr = new Correlation<double>();

		memset(ans,0,4*N);
		corr->doCorrelation(y1, y2, N, 2*N, ans);

		sum = 0.0;
		for(int i=0;i<2*N;i++)
			sum += sqrt(pow((N*ans[2*i] - ans12[i]),2));
		if(sum >= 0.0003)
			cout << "SOMETHING WENT WRONG!!!!!" << endl;
		else
			cout << "CORR(y1,y2): OK!" << endl;
	}
	//---------------------------------------------------

	//---------------------------------------------------
	{
		int N = 16;
		double y1[] = {0,0,0,0,0,2,3,4,5,6,7,8,9,10,11,12};
		double y2[] = {0,0,0,0,0,0,0,0,2,3,4,5,6,7,8,9};
		double ans[4*N];
		double sum;
		double ans12[] = {416, 343, 274, 210, 152, 101, 58, 24, 4.9738e-14, 2.84217e-14, 5.68434e-14, 4.26326e-14, 4.26326e-14, 5.68434e-14, 8.52651e-14, 8.52651e-14, 2.84217e-14, 8.52651e-14, 5.68434e-14, 4.26326e-14, 1.42109e-14, 4.9738e-14, 18, 43, 74, 110, 150, 193, 238, 284, 328, 372};

		Correlation<double>* corr = new Correlation<double>();

		memset(ans,0,4*N);
		corr->doCorrelation(y1, y2, N, 2*N, ans);

		sum = 0.0;
		for(int i=0;i<2*N;i++)
			sum += sqrt(pow((N*ans[2*i] - ans12[i]),2));
		if(sum >= 0.0003)
			cout << "SOMETHING WENT WRONG!!!!!" << endl;
		else
			cout << "CORR(y1,y2): OK!" << endl;
	}
	//---------------------------------------------------

	//---------------------------------------------------
	{
		int N = 16;
		double y1[] = {0,0,0,0,0,0,0,0,9,7,5,3,1,13,11,10};
		double y2[] = {0,0,0,0,0,0,0,0,21,5,27,8,2,16,6,10};
		double ans[4*N];
		double sum;
		double ans12[] = {759, 523, 433, 527, 463, 598, 281, 210, 4.9738e-14, 5.68434e-14, 5.68434e-14, 1.13687e-13, 7.10543e-14, 1.27898e-13, 1.42109e-13, 1.42109e-13, 5.68434e-14, 1.13687e-13, 5.68434e-14, 1.13687e-13, 5.68434e-14, 1.13687e-13, 2.84217e-14, 9.9476e-14, 6.39488e-14, 90, 124, 236, 190, 194, 493, 484};

		Correlation<double>* corr = new Correlation<double>();

		memset(ans,0,4*N);
		corr->doCorrelation(y1, y2, N, 2*N, ans);

		sum = 0.0;
		for(int i=0;i<2*N;i++)
			sum += sqrt(pow((N*ans[2*i] - ans12[i]),2));
		if(sum >= 0.0003)
			cout << "SOMETHING WENT WRONG!!!!!" << endl;
		else
			cout << "CORR(y1,y2): OK!" << endl;
	}
	//---------------------------------------------------

	//---------------------------------------------------
	{
		int N = 16;
		double y1[] = {0,0,0,0,0,0,0,0,9,7,5,3,1,13,11,10};
		double y2[] = {0,0,0,0,0,0,0,0,0,21,5,27,8,2,16,6};
		double ans[4*N];
		double sum;
		double ans12[] = {523, 433, 527, 463, 598, 281, 210, -2.17847e-14, 4.9738e-14, 1.77636e-14, 1.13687e-13, 5.68434e-14, 9.9476e-14, 5.68434e-14, 8.52651e-14, 1.13687e-13, 0, 5.68434e-14, 5.68434e-14, 2.84217e-14, 5.68434e-14, 8.52651e-14, 7.10543e-14, 7.86281e-14, 6.39488e-14, 54, 186, 160, 184, 363, 374, 659};

		Correlation<double>* corr = new Correlation<double>();

		memset(ans,0,4*N);
		corr->doCorrelation(y1, y2, N, 2*N, ans);

		sum = 0.0;
		for(int i=0;i<2*N;i++)
			sum += sqrt(pow((N*ans[2*i] - ans12[i]),2));
		if(sum >= 0.0003)
			cout << "SOMETHING WENT WRONG!!!!!" << endl;
		else
			cout << "CORR(y1,y2): OK!" << endl;
	}
	//---------------------------------------------------

	//---------------------------------------------------
	{
		int N = 16;
		double y1[] = {0,0,0,0,0,0,0,0,9,7,5,3,1,13,11,10};
		double y2[] = {0,0,0,0,0,0,0,0,0,0,21,5,27,8,2,16};
		double ans[4*N];
		double sum;
		double ans12[] = {433, 527, 463, 598, 281, 210, 6.98684e-14, 5.09498e-14, 4.26326e-14, 7.10543e-14, 5.68434e-14, 1.27898e-13, 1.13687e-13, 8.52651e-14, 1.7053e-13, 1.7053e-13, 5.68434e-14, 5.68434e-14, 8.52651e-14, 1.13687e-13, 2.84217e-14, 4.26326e-14, -1.3025e-14, 3.43154e-14, 1.42109e-14, 144, 130, 166, 357, 296, 593, 463};

		Correlation<double>* corr = new Correlation<double>();

		memset(ans,0,4*N);
		corr->doCorrelation(y1, y2, N, 2*N, ans);

		sum = 0.0;
		for(int i=0;i<2*N;i++)
			sum += sqrt(pow((N*ans[2*i] - ans12[i]),2));
		if(sum >= 0.0003)
			cout << "SOMETHING WENT WRONG!!!!!" << endl;
		else
			cout << "CORR(y1,y2): OK!" << endl;
	}
	//---------------------------------------------------

	//---------------------------------------------------
	{
		int N = 16;
		double y1[] = {0,0,0,0,0,0,0,0,9,7,5,3,1,13,11,10};
		double y2[] = {0,0,0,0,0,0,0,0,0,0,0,21,5,27,8,2};
		double ans[4*N];
		double sum;
		double ans12[] = {527, 463, 598, 281, 210, -2.60267e-14, 3.27782e-14, 1.93167e-14, 6.03961e-14, 3.19744e-14, 1.06581e-13, 5.68434e-14, 8.52651e-14, 5.68434e-14, 8.52651e-14, 1.13687e-13, 5.68434e-14, 5.68434e-14, 5.68434e-14, 8.52651e-14, 4.26326e-14, -2.39496e-15, -4.35652e-15, 3.75268e-14, 5.32907e-14, 18, 86, 309, 280, 385, 287, 273};

		Correlation<double>* corr = new Correlation<double>();

		memset(ans,0,4*N);
		corr->doCorrelation(y1, y2, N, 2*N, ans);

		sum = 0.0;
		for(int i=0;i<2*N;i++)
			sum += sqrt(pow((N*ans[2*i] - ans12[i]),2));
		if(sum >= 0.0003)
			cout << "SOMETHING WENT WRONG!!!!!" << endl;
		else
			cout << "CORR(y1,y2): OK!" << endl;
	}
	//---------------------------------------------------

	return 0;
}
