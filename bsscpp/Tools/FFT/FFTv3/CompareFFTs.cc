#include <iostream>
#include <math.h>

using namespace std;

int main(int argc, char* argv[])
{
	long N = 8192;

	FILE * pFile1;
  	pFile1 = fopen ("/home/zz4fap/Desktop/ReadingWaveFiles/FFT/FFTv3/fourier3.dat" , "rb" );

	FILE * pFile2;
  	pFile2 = fopen ("/home/zz4fap/Desktop/ReadingWaveFiles/FFT/FFTv2/fourier2.dat" , "rb" );

	FILE * pFile3;
  	pFile3 = fopen ("/home/zz4fap/Desktop/ReadingWaveFiles/FFT/FFTMatlab.dat" , "rb" );

	float value1, value2, res;
	for(int i=0;i<2*N+1;i++)
	{
		if(i >= 1)
	  		fread(&value1, sizeof(float) , 1, pFile1);

		fread(&value2, sizeof(float) , 1, pFile2);
		//if(i >= 1)
			//cout << "value1: " << value1 << " - value2: " << value2 << " - value1 - value2: " << value1 - value2 << endl;
		res = (value1-value2);
		res = (float)sqrt((float)pow(res,2));
		if(res > 0.0005 && i >= 1)
			cout << "OpA!!! Olha isso!!" << (value1 - value2) << " index: " << i << endl;
	}

	fclose(pFile1);
  	pFile1 = fopen ("/home/zz4fap/Desktop/ReadingWaveFiles/FFT/FFTv3/fourier3.dat" , "rb" );

	float vector[2*N];

	for(int i=0; i<2*N;i++)
	{
		fread(&vector[i], sizeof(float) , 1, pFile1);
	}

	float absResult[N];

	for(int i=0; i<N;i++)
	{
		absResult[i] = (float)sqrt(pow((float)vector[2*i],2) + pow((float)vector[2*i+1],2));
	}

	float value;
	for(int i=0;i<N;i++)
	{
		fread(&value, sizeof(float) , 1, pFile3);

		res = value - absResult[i];
		res = (float)sqrt((float)pow(res,2));
		if(res > 0.0005)
			cout << "OpA!!! Olha isso!!" << (value - absResult[i]) << " index: " << i << endl;

	}

	return 0;
}
