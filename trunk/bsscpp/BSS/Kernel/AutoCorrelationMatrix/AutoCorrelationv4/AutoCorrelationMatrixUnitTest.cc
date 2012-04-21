#include <math.h>
#include <iostream>
#include <string>
#include <string.h>
#include <fstream>
#include <iomanip>
#include <stdlib.h>
#include "Fourier.hh"
#include "Correlation.hh"
#include "AutoCorrelationMatrix.hh"
#include "Matrix.hh"

using namespace std;

int main(int argc, char* argv[])
{
	unsigned int D, N;
	float sum = 0;
	bool res = true;

	AutoCorrelationMatrix<float>* ccm = NULL;

	{
		D = 4;
		N = 8;

		ccm = new AutoCorrelationMatrix<float>(D, N);

		Matrix<float> CMM(D,D);

		float y1[] = {0.0, 0.0, 0.0, 1.0, 2.0, 3.0, 2.0, 1.0, 1.0, 3.0, 2.0};
		float ans[] = {33.0,26.0,17.0,15.0,26.0,29.0,20.0,15.0,17.0,20.0,20.0,17.0,15.0,15.0,17.0,19.0};

		ccm->getAutoCorrelationMatrix(y1, CMM);

		sum = 0;
		for(int i = 0; i < D; i++)
			for(int j = 0; j < D; j++)
				sum += sqrt(pow((CMM(i,j) - ans[i*D+j]),2));

		sum = sum/(D*D);

		if(sum > 0.0000004)
		{
			cout << "Something went wrong!!!" << endl;
			res &= false;
		}
		else
		{
			cout << "Everything went fine!" << endl;
			res &= true;
		}

		delete ccm;
	}

	{
		D = 5;
		N = 8;

		ccm = new AutoCorrelationMatrix<float>(D, N);

		Matrix<float> CMM(D,D);

		float y2[] = {0.0, 0.0, 0.0, 0.0, 2.0, 1.0, 2.0, 3.0, 3.0, 2.0, 1.0, 2.0};
		float ans[] = {36.0,29.0,26.0,22.0,16.0,29.0,32.0,27.0,22.0,16.0,26.0,27.0,31.0,25.0,19.0,22.0,22.0,25.0,27.0,19.0,16.0,16.0,19.0,19.0,18.0};

		ccm->getAutoCorrelationMatrix(y2, CMM);

		sum = 0;
		for(int i = 0; i < D; i++)
			for(int j = 0; j < D; j++)
				sum += sqrt(pow((CMM(i,j) - ans[i*D+j]),2));

		sum = sum/(D*D);

		if(sum > 0.0000004)
		{
			cout << "Something went wrong!!!" << endl;
			res &= false;
		}
		else
		{
			cout << "Everything went fine!" << endl;
			res &= true;
		}

		delete ccm;
	}

	{
		D = 3;
		N = 8;

		ccm = new AutoCorrelationMatrix<float>(D, N);

		Matrix<float> CMM(D,D);

		float y3[] = {0.0, 0.0, 5.0, 2.0, 6.0, 7.0, 8.0, 1.0, 4.0, 9.0};
		float ans[] = {276.0,168.0,140.0,168.0,195.0,132.0,140.0,132.0,179.0};

		ccm->getAutoCorrelationMatrix(y3, CMM);

		sum = 0;
		for(int i = 0; i < D; i++)
			for(int j = 0; j < D; j++)
				sum += sqrt(pow((CMM(i,j) - ans[i*D+j]),2));

		sum = sum/(D*D);

		if(sum > 0.0000004)
		{
			cout << "Something went wrong!!!" << endl;
			res &= false;
		}
		else
		{
			cout << "Everything went fine!" << endl;
			res &= true;
		}

		delete ccm;
	}

	{
		D = 9;
		N = 16;

		ccm = new AutoCorrelationMatrix<float>(D, N);

		Matrix<float> CMM(D,D);

		float y4[] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 9.0, 7.0, 5.0, 3.0, 1.0, 13.0, 11.0, 10.0, 4.0, 2.0, 3.0, 12.0, 15.0, 6.0, 3.0, 8.0};
		float ans[] = {1062.0,784.0,561.0,498.0,545.0,633.0,607.0,472.0,307.0,784.0,998.0,760.0,513.0,378.0,449.0,609.0,591.0,440.0,561.0,760.0,989.0,742.0,468.0,342.0,440.0,603.0,579.0,498.0,513.0,742.0,953.0,652.0,396.0,324.0,428.0,579.0,545.0,378.0,468.0,652.0,728.0,472.0,351.0,294.0,368.0,633.0,449.0,342.0,396.0,472.0,584.0,436.0,327.0,246.0,607.0,609.0,440.0,324.0,351.0,436.0,575.0,430.0,315.0,472.0,591.0,603.0,428.0,294.0,327.0,430.0,571.0,422.0,307.0,440.0,579.0,579.0,368.0,246.0,315.0,422.0,555.0};

		ccm->getAutoCorrelationMatrix(y4, CMM);

		sum = 0;
		for(int i = 0; i < D; i++)
			for(int j = 0; j < D; j++)
				sum += sqrt(pow((CMM(i,j) - ans[i*D+j]),2));

		sum = sum/(D*D);

		if(sum > 0.00004)
		{
			cout << "Something went wrong!!!" << endl;
			res &= false;
		}
		else
		{
			cout << "Everything went fine!" << endl;
			res &= true;
		}

		delete ccm;
	}

	{
		D = 7;
		N = 16;

		ccm = new AutoCorrelationMatrix<float>(D, N);

		Matrix<float> CMM(D,D);

		float y5[] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 21.0, 5.0, 27.0, 8.0, 2.0, 16.0, 6.0, 10.0, 17.0, 33.0, 8.0, 11.0, 6.0, 12.0, 4.0, 9.0};
		float ans[] = {3495.0,1965.0,2204.0,1807.0,1713.0,1500.0,1612.0,1965.0,3414.0,1929.0,2096.0,1753.0,1614.0,1428.0,2204.0,1929.0,3398.0,1881.0,2072.0,1709.0,1582.0,1807.0,2096.0,1881.0,3254.0,1809.0,1940.0,1613.0,1713.0,1753.0,2072.0,1809.0,3218.0,1743.0,1892.0,1500.0,1614.0,1709.0,1940.0,1743.0,3097.0,1655.0,1612.0,1428.0,1582.0,1613.0,1892.0,1655.0,3033.0};

		ccm->getAutoCorrelationMatrix(y5, CMM);

		sum = 0;
		for(int i = 0; i < D; i++)
			for(int j = 0; j < D; j++)
				sum += sqrt(pow((CMM(i,j) - ans[i*D+j]),2));

		sum = sum/(D*D);

		if(sum > 0.00004)
		{
			cout << "Something went wrong!!!" << endl;
			res &= false;
		}
		else
		{
			cout << "Everything went fine!" << endl;
			res &= true;
		}

		delete ccm;
	}

	// Testing getOutputPowerMatrix --------------------------------------------------------------------------
	{
		D = 4;
		N = 8;

		ccm = new AutoCorrelationMatrix<float>(D, N);

		Matrix<float> CMM(D,D);

		float y1[] = {0.0, 0.0, 0.0, 1.0, 2.0, 3.0, 2.0, 1.0, 1.0, 3.0, 2.0};
		float ans[] = {33.0,0.0,0.0,0.0,0.0,33.0,0.0,0.0,0.0,0.0,33.0,0.0,0.0,0.0,0.0,33.0};

		ccm->getOutputPowerMatrix(y1, (float)10.0, CMM);

		sum = 0;
		for(int i = 0; i < D; i++)
			for(int j = 0; j < D; j++)
				sum += sqrt(pow((CMM(i,j) - ans[i*D+j]),2));

		if(sum > 0.0000004)
		{
			cout << "Something went wrong!!!" << endl;
			res &= false;
		}
		else
		{
			cout << "Everything went fine!" << endl;
			res &= true;
		}

		delete ccm;
	}

	{
		D = 5;
		N = 8;

		ccm = new AutoCorrelationMatrix<float>(D, N);

		Matrix<float> CMM(D,D);

		float y2[] = {0.0, 0.0, 0.0, 0.0, 2.0, 1.0, 2.0, 3.0, 3.0, 2.0, 1.0, 2.0};
		float ans[] = {36.0,0.0,0.0,0.0,0.0,0.0,36.0,0.0,0.0,0.0,0.0,0.0,36.0,0.0,0.0,0.0,0.0,0.0,36.0,0.0,0.0,0.0,0.0,0.0,36.0};

		ccm->getOutputPowerMatrix(y2, (float)10.0, CMM);

		sum = 0;
		for(int i = 0; i < D; i++)
			for(int j = 0; j < D; j++)
				sum += sqrt(pow((CMM(i,j) - ans[i*D+j]),2));

		sum = sum/(D*D);

		if(sum > 0.0000004)
		{
			cout << "Something went wrong!!!" << endl;
			res &= false;
		}
		else
		{
			cout << "Everything went fine!" << endl;
			res &= true;
		}

		delete ccm;
	}

	{
		D = 3;
		N = 8;

		ccm = new AutoCorrelationMatrix<float>(D, N);

		Matrix<float> CMM(D,D);

		float y3[] = {0.0, 0.0, 5.0, 2.0, 6.0, 7.0, 8.0, 1.0, 4.0, 9.0};
		float ans[] = {276.0,0.0,0.0,0.0,276.0,0.0,0.0,0.0,276.0};

		ccm->getOutputPowerMatrix(y3, (float)10.0, CMM);

		sum = 0;
		for(int i = 0; i < D; i++)
			for(int j = 0; j < D; j++)
				sum += sqrt(pow((CMM(i,j) - ans[i*D+j]),2));

		sum = sum/(D*D);

		if(sum > 0.0000004)
		{
			cout << "Something went wrong!!!" << endl;
			res &= false;
		}
		else
		{
			cout << "Everything went fine!" << endl;
			res &= true;
		}

		delete ccm;
	}

	{
		D = 9;
		N = 16;

		ccm = new AutoCorrelationMatrix<float>(D, N);

		Matrix<float> CMM(D,D);

		float y4[] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 9.0, 7.0, 5.0, 3.0, 1.0, 13.0, 11.0, 10.0, 4.0, 2.0, 3.0, 12.0, 15.0, 6.0, 3.0, 8.0};
		float ans[] = {1062.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,1062.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,1062.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,1062.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,1062.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,1062.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,1062.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,1062.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,1062.0};

		ccm->getOutputPowerMatrix(y4, (float)10.0, CMM);

		sum = 0;
		for(int i = 0; i < D; i++)
			for(int j = 0; j < D; j++)
				sum += sqrt(pow((CMM(i,j) - ans[i*D+j]),2));

		sum = sum/(D*D);

		if(sum > 0.00004)
		{
			cout << "Something went wrong!!!" << endl;
			res &= false;
		}
		else
		{
			cout << "Everything went fine!" << endl;
			res &= true;
		}

		delete ccm;
	}

	{
		D = 7;
		N = 16;

		ccm = new AutoCorrelationMatrix<float>(D, N);

		Matrix<float> CMM(D,D);

		float y5[] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 21.0, 5.0, 27.0, 8.0, 2.0, 16.0, 6.0, 10.0, 17.0, 33.0, 8.0, 11.0, 6.0, 12.0, 4.0, 9.0};
		float ans[] = {3495.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,3495.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,3495.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,3495.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,3495.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,3495.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,3495.0};

		ccm->getOutputPowerMatrix(y5, (float)10.0, CMM);

		sum = 0;
		for(int i = 0; i < D; i++)
			for(int j = 0; j < D; j++)
				sum += sqrt(pow((CMM(i,j) - ans[i*D+j]),2));

		sum = sum/(D*D);

		if(sum > 0.00004)
		{
			cout << "Something went wrong!!!" << endl;
			res &= false;
		}
		else
		{
			cout << "Everything went fine!" << endl;
			res &= true;
		}

		delete ccm;
	}

	if(res)
		cout << "FINAL RESULT: OK!" << endl;
	else
		cout << "FINAL RESULT: NOK!!!!!" << endl;
}
