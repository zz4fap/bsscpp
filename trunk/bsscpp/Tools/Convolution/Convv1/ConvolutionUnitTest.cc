#include <math.h>
#include <iostream>
#include <string>
#include <string.h>
#include <fstream>
#include <stdlib.h>
#include "Fourier.hh"
#include "Convolution.hh"

using namespace std;

int main(int argc, char* argv[])
{
	int N = 8;
	float sum = 0;

	Convolution* conv = new Convolution();

	float data1[] = {1.0, 2.0, 3.0, 2.0, 1.0, 1.0, 3.0, 2.0};

	float data2[] = {2.0, 1.0, 2.0, 3.0, 3.0, 2.0, 1.0, 2.0};

	float data3[] = {5.0, 2.0, 6.0, 7.0, 8.0, 1.0, 4.0, 9.0};

	float ans12[] = {2.0,5.0,10.0,14.0,19.0,24.0,29.0,28.0,25.0,26.0,22.0,15.0,9.0,8.0,4.0,0.0};
	
	float ans13[] = {5.0,12.0,25.0,35.0,49.0,57.0,67.0,65.0,69.0,77.0,61.0,32.0,23.0,35.0,18.0,0.0};

	float ans32[] = {10.0,9.0,24.0,39.0,56.0,58.0,73.0,93.0,68.0,68.0,63.0,52.0,24.0,17.0,18.0,0.0};
	
	float ans11[] = {1.0,4.0,10.0,16.0,19.0,18.0,20.0,26.0,31.0,26.0,15.0,10.0,13.0,12.0,4.0,0.0};

	float ans[4*N];

	//---------------------------------------------------
	conv->doConvolution(data1, data2, N, 2*N, ans);
	sum = 0.0;
	for(int i=0;i<2*N;i++)
		sum += sqrt(pow((N*ans[2*i] - ans12[i]),2));
	if(sum >= 0.0003)
		cout << "SOMETHING WENT WRONG!!!!!" << endl;
	else
		cout << "CONV(y1,y2): OK!" << endl;
	//---------------------------------------------------	
	
	//---------------------------------------------------
	memset(ans,0,4*N);
	conv->doConvolution(data1, data3, N, 2*N, ans);
	sum = 0.0;
	for(int i=0;i<2*N;i++)
		sum += sqrt(pow((N*ans[2*i] - ans13[i]),2));
	if(sum >= 0.0003)
		cout << "SOMETHING WENT WRONG!!!!!" << endl;
	else
		cout << "CONV(y1,y3): OK!" << endl;
	//---------------------------------------------------	

	//---------------------------------------------------
	memset(ans,0,4*N);
	conv->doConvolution(data3, data2, N, 2*N, ans);
	sum = 0.0;
	for(int i=0;i<2*N;i++)
		sum += sqrt(pow((N*ans[2*i] - ans32[i]),2));
	if(sum >= 0.0003)
		cout << "SOMETHING WENT WRONG!!!!!" << endl;
	else
		cout << "CONV(y3,y2): OK!" << endl;
	//---------------------------------------------------

	//---------------------------------------------------	
	memset(ans,0,4*N);
	conv->doConvolution(data1, data1, N, 2*N, ans);
	sum = 0.0;
	for(int i=0;i<2*N;i++)
		sum += sqrt(pow((N*ans[2*i] - ans11[i]),2));
	if(sum >= 0.0003)
		cout << "SOMETHING WENT WRONG!!!!!" << endl;
	else
		cout << "CONV(y1,y1): OK!" << endl;

	//---------------------------------------------------		

	return 0;
}
