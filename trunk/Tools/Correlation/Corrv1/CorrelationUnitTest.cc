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

	float data[] = {1.0, 2.0, 3.0, 2.0, 1.0, 1.0, 3.0, 2.0};

	float ans[2*N];

	corr->doCorrelation(data, data, N, N, ans);




	

	return 0;
}
