#include <iostream>
#include <math.h>
#include "Fourier.hh"

using namespace std;

#define PI 4*atan((double)1)

#define EPS 1.0e-3
#define NP 32
#define WIDTH 50.0

/* warning: programmed assuming data starts
at a[1]; with zero offset array, call with &a[-1] */

int testRealFT(Fourier<float>& fft);
void correlv2(float data1[], float data2[], unsigned long n, float ans[], Fourier<float>& obj);
void correl(float data1[], float data2[], unsigned long n, float ans[], Fourier<float>& obj);
int testCorrel(Fourier<float>& fft);

int main(int argc, char* argv[])
{
	double sum = 0.0;

	{
		//Creates a Fourier object
		//The CFourier class has the FFT function and a couple of usefull variables
		Fourier<float> fft;

		//sample rate of the signal (must be 2^n)
		long N = 8192; //2^13

		//number of samples you want to send for processing in the fft (any)
		//for example 100 samples
		long captured_samples = 500;

		//frequency of the signal (has to be smaller than sample_rate/2)
		//for example 46
		int frequency = 250;

		float data[5000];

		//example of a sin signal(you can try to add some more signals to see the
		//fourier change

		for(int i = 0; i < captured_samples; i++)
		{
			double temp=(double)(2*PI*frequency*((float)i/N));		
			data[i]=((double)30*sin(temp));	
		}

		//apply the FFT to the signal
		fft.fft(data, captured_samples, N, 1);

		float *vector;
		vector = new float[2*N];
		fft.getFFTVector(vector, N);

		float absResult[N];

		for(int i = 0; i < N; i++)
		{
			absResult[i] = (float)sqrt(pow((float)vector[2*i],2) + pow((float)vector[2*i+1],2));
		}

	  	FILE * pFile;
	  	pFile = fopen ("fourier7.dat" , "wb" );

		for(int i=0;i<2*N;i++)
		{
			//cout << fft.vector[i] << endl;
			//cout << absResult[i] << endl;
	  		fwrite(&vector[i], sizeof(float) , 1, pFile);
		}

	  	fclose (pFile);

		pFile = NULL;
	  	pFile = fopen ("fourier7.dat" , "rb" );
		if(pFile <= 0 || ferror(pFile) || feof(pFile))
			cout << "ERROR!!!!!!!!!!!!!!!!!!!" << endl;
		
		float value;
		sum = 0.0;
		for(int i=0;i<2*N;i++)
		{
	  		fread(&value, sizeof(float) , 1, pFile);
			sum += (value - vector[i]);
		}

		if(sum != 0.0)
			cout << "ERROR!!!!!! - SUM: " << sum << endl;
		else
			cout << "Everything went fine!" << endl;

	  	fclose (pFile);
	}
	
	{
		long N = 8;

		float data[] = {1.0, 2.0, 3.0, 2.0, 1.0, 1.0, 3.0, 2.0};

		float ans[2*N];

		Fourier<float> *fft2 = new Fourier<float>();

		fft2->fft(data, N, N, 1);
		fft2->getFFTVector(ans, N);

		fft2->fft(ans, N, N, -1);
		memset(ans,0,2*N*sizeof(float));
		fft2->getFFTVector(ans, N);

		sum = 0;
		for(int i = 0; i < 2*N; i+=2)
		{
			sum += data[i/2] - ans[i];
			sum += sqrt(pow(ans[i+1],2));
		}
		if(sum >= 0.000000000000001)
			cout << "IFFT: NOK!!!!!" << endl;
		else
			cout << "IFFT: OK!" << endl;

		delete fft2;
	}

	{
		long N = 8;

		float data[] = {1.0, 2.0, 3.0, 2.0, 1.0, 1.0, 3.0, 2.0};
		float ans[] = {15.0000,0,0.707107,0.707107,-4.0000,-1.0000,-0.707107,0.707107,1.0000,0,-0.707107,-0.707107,-4.0000,1.0000,0.707107,-0.707107};
		float vector[2*N];

		Fourier<float> *fft2 = new Fourier<float>();

		fft2->fft(data, N, N, 1);
		fft2->getFFTVector(vector, N);

		sum = 0;
		for(int i = 0; i < 2*N; i++)
			sum += sqrt(pow((vector[i] - ans[i]),2));

		sum = sum/2*N;

		if(sum >= 0.00007)
			cout << "FFT: NOK!!!!!" << endl;
		else
			cout << "FFT: OK!" << endl;


		delete fft2;
	}

/* warning: programmed assuming data starts
at a[1]; with zero offset array, call with &a[-1] */
	if(1)
	{
		long N = 8;
		float data[] = {1.0, 2.0, 3.0, 2.0, 1.0, 1.0, 3.0, 2.0};
		float fftans[] = {15,1,0.707107,0.707107,-4,-1,-0.707107,0.707107};
		float ifftans[] = {1.0, 2.0, 3.0, 2.0, 1.0, 1.0, 3.0, 2.0};
		double sum = 0;

		Fourier<float> fft3;

		fft3.realft(&data[-1], N, 1);
		
		sum = 0;
		for(int i = 0; i < N; i++)
			sum += sqrt(pow((data[i] - fftans[i]),2));
		
		if(sum > 9.6e-07)
			cout << "FFT: NOK!!!!!" << endl;
		else
			cout << "FFT: OK!" << endl;

		fft3.realft(&data[-1], N, -1);

		sum = 0;
		for(int i = 0; i < N; i++)
			sum += sqrt(pow(((data[i]/(N/2)) - ifftans[i]),2));

		if(sum != 0)
			cout << "IFFT: NOK!!!!!" << endl;
		else
			cout << "IFFT: OK!" << endl;
	}

/* warning: programmed assuming data starts
at a[1]; with zero offset array, call with &a[-1] */
	if(1)
	{
		long N = 8;
		float data1[] = {1.0, 2.0, 3.0, 2.0, 1.0, 1.0, 3.0, 2.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
		float data2[] = {2.0, 1.0, 2.0, 3.0, 3.0, 2.0, 1.0, 2.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
		float ans12[] = {0, 28.0, 25.0, 26.0, 22.0, 15.0, 9.0, 8.0, 4.0, 0.0, 2.0, 5.0, 10.0, 14.0, 19.0, 24.0, 29.0};
		float ans[2*N+1];
		double sum = 0;

		Fourier<float> fft4;
		correl(&data1[-1], &data2[-1], 2*N, ans, fft4);

		sum = 0;
		for(int i=0;i<2*N+1;i++)
		{
			sum += sqrt(pow((ans[i]-ans12[i]),2));
		}

		if(sum > 9.6e-07)
			cout << "correl: NOK!!!!!" << endl;
		else
			cout << "correl: OK!" << endl;
	}
                           

	return 0;
}

//It doesn't work!!!!!!!!!!!!!
void correlv2(float data1[], float data2[], unsigned long n, float ans[], Fourier<float>& obj)
{
 	unsigned long no2,i;
 	float tmp, temp[n];

	for(int i = 0; i < n; i++)
	{
		ans[i]=data1[i];
		temp[i]=data2[i];
	}

	obj.realft(ans,n,1);

	obj.realft(temp,n,1);

 	no2=n>>1;
	for (i=2;i<n;i+=2) 
	{
		tmp = ans[i];
  		ans[i]=(ans[i]*temp[i]+ans[i+1]*temp[i+1])/no2;
  		ans[i+1]=(ans[i+1]*temp[i]-tmp*temp[i+1])/no2;
 	}

 	ans[0]=ans[0]*temp[0]/no2;
 	ans[1]=ans[1]*temp[1]/no2;
 	obj.realft(ans,n,-1);
}

void correl(float data1[], float data2[], unsigned long n, float ans[], Fourier<float>& obj)
{
 	unsigned long no2,i;
 	float dum,*fft;
   
 	fft=(float*)malloc((n<<1)*sizeof(float));
 	obj.twofft(data1,data2,fft,ans,n);

 	no2=n>>1;
 	for (i=2;i<=n+2;i+=2) 
	{
  		ans[i-1]=(fft[i-1]*(dum=ans[i-1])+fft[i]*ans[i])/no2;
  		ans[i]=(fft[i]*dum-fft[i-1]*ans[i])/no2;
 	}

 	ans[2]=ans[n+1];
 	obj.realft(ans,n,-1);

 	free(fft);
}
   
int testCorrel(Fourier<float>& fft)
{
	long N  = 64;
	long N2 = (2*N);
 	unsigned long i,j;
 	float cmp,*data1,*data2,*ans;
   
 	data1=(float*)malloc(N*sizeof(float));
	memset(data1,0,N*sizeof(float));

 	data2=(float*)malloc(N*sizeof(float));
	memset(data2,0,N*sizeof(float));

 	ans=(float*)malloc(N2*sizeof(float));
	memset(ans,0,N2*sizeof(float));

 	for (i=1;i<=N;i++) 
	{
  		if ((i > N/2-N/8) && (i < N/2+N/8))
   			data1[i]=1.0;
  		else
   			data1[i]=0.0;
  		data2[i]=data1[i];
 	}

	correl(data1,data2,N,ans,fft);

 	/* Calculate directly */
 	printf("%3s %14s %18s\n","n","CORREL","direct calc.");
 	for (i=0;i<=16;i++) 
	{
  		cmp=0.0;
  		for (j=1;j<=N;j++)
   			cmp += data1[((i+j-1) % N)+1]*data2[j];

  		printf("%3ld %15.6f %15.6f\n",i,ans[i+1],cmp);
 	}

 	free(ans);
 	free(data2);
 	free(data1);
}

int testRealFT(Fourier<float>& fft)
{
 	int i,j,n=NP/2,nlim;
 	float big,per,scal,small,*data,*size;
   
 	data=(float*)malloc(NP*sizeof(float));
	memset(data,0,sizeof(float)*NP);
 	size=(float*)malloc((NP/2+1)*sizeof(float));
	memset(size,0,sizeof(float)*(NP/2+1));

 	for (;;) 
	{
  		printf("Period of sinusoid in channels (2-%2d)\n",NP);
  		scanf("%f",&per);
  		if (per <= 0.0) 
			break;
  		for (i=1;i<=NP;i++)
   			data[i]=cos(2.0*PI*(i-1)/per);
  		fft.realft(data,NP,1);
  		big = -1.0e10;
  		for (i=2;i<=n;i++) 
		{
   			size[i]=sqrt(sqrt(data[2*i-1])+sqrt(data[2*i]));
   			if (size[i] > big) 
				big=size[i];

  		}
  		size[1]=fabs(data[1]);
  		if (size[1] > big) 
			big=size[1];
  		size[n+1]=fabs(data[2]);
  		if (size[n+1] > big) 
			big=size[n+1];
  		scal=WIDTH/big;
  		for (i=1;i<=n;i++) 
		{
   			nlim=(int) (0.5+scal*size[i]+EPS);
   			printf("%4d ",i);
   			for (j=1;j<=nlim+1;j++) 
				printf("*");
   			printf("\n");
  		}
  		printf("press RETURN to continue ...\n");
  		(void) getchar();
  		fft.realft(data,NP,-1);
  		big = -1.0e10;
  		small=1.0e10;
  		for (i=1;i<=NP;i++) 
		{
   			if (data[i] < small) 
				small=data[i];
   			if (data[i] > big) 
				big=data[i];
  		}
  		scal=WIDTH/(big-small);
  		for (i=1;i<=NP;i++) 
		{
   			nlim=(int) (0.5+scal*(data[i]-small)+EPS);
   			printf("%4d ",i);
   			for (j=1;j<=nlim+1;j++) 
				printf("*");
   			printf("\n");
  		}
 	}

 	free(size);
 	free(data);
}

