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

	Correlation* corr = new Correlation();

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

	return 0;
}
