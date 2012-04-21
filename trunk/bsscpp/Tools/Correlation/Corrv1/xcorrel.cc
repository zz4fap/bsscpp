/* Driver for routine correl */
#include <math.h>
#include <iostream>
#include <string>
#include <string.h>
#include <fstream>
#include <stdlib.h>
#include "Fourier.hh"
#include "Correlation.hh"
   
#define N 64
#define N2 (2*N)
   
int main(void)
{
	unsigned long i, j;
 	float cmp, *data1, *data2, *ans;
   
 	data1 = new float[N];
 	data2 = new float[N];
 	ans = new float[N2];

 	for (i = 1; i <= N; i++) 
	{
		if ((i > N/2-N/8) && (i < N/2+N/8))
	   		data1[i] = 1.0;
	  	else
	   		data1[i] = 0.0;
	  	data2[i] = data1[i];
 	}

 	correl(data1,data2,N,ans);
 
	/* Calculate directly */
 	printf("%3s %14s %18s\n","n","CORREL","direct calc.");
 	for (i=0;i<=16;i++) 
	{
  		cmp = 0.0;
  		for (j=1;j<=N;j++)
   			cmp += data1[((i+j-1) % N)+1]*data2[j];

  		printf("%3ld %15.6f %15.6f\n",i,ans[i+1],cmp);
 	}

	delete [] data1;
	delete [] data2;
	delete [] ans;

	return 0;
}
#undef NRANSI


