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

	Correlation* corr = new Correlation();

	/*float data[] = {1.0, 2.0, 3.0, 2.0, 1.0, 1.0, 3.0, 2.0};

	float ans[4*N];

	corr->doCorrelation(data, data, N, 2*N, ans);

	for(int i=0;i<4*N;i++)
		cout << 8*ans[i] << endl;*/

	float data1[] = {1.0, 2.0, 3.0, 2.0, 1.0, 1.0, 3.0, 2.0};

	float data2[] = {2.0, 1.0, 2.0, 3.0, 3.0, 2.0, 1.0, 2.0};

	float data3[] = {5.0, 2.0, 6.0, 7.0, 8.0, 1.0, 4.0, 9.0};

	float ans[4*N];
	
	corr->doCorrelation(data1, data3, N, 2*N, ans);

	memset(ans,0,4*N);
	corr->doAutoCorrelation(data1, N, 2*N, ans);

	for(int i=0;i<4*N;i++)
		cout << 8*ans[i] << endl;

	return 0;
}
