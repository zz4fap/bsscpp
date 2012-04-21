/*
Usage: LUDCMP, a, indx, d
Input Parameters: a  An n-by-n matrix.

Output Parameters: a  On output, a is replaced by the LU decomposition of a row-wise permutation of itself.
		   indx  The vector which records the row permutation effected by the partial pivoting. The values returned for index are needed in the call to LUBKSB.
		   d  An indicator of the number of row interchanges:
			+1	 Indicates the number was even.
			-1	 Indicates the number was odd.
*/
template <class D>
void ludcmp(Matrix<D>& a, int *indx, D *d)
{
	int i,imax,j,k, n;
	D big,dum,sum,temp;
	D *vv;

	if(a.getNumberOfRows() != a.getNumberOfColumns())
		throw "Error: Matrix is not square";

	n = a.getNumberOfColumns();
   
  	vv = new D[n];

 	*d=1.0;

 	for (i=0;i<n;i++)
	{
  		big=0.0;
  		for (j=0;j<n;j++)
   			if ((temp=fabs(a(i,j))) > big) 
				big = temp;
  			if (big == 0.0) 
				throw "Singular matrix in routine ludcmp";
  			vv[i] = 1.0/big;
 	}

 	for (j=0;j<n;j++)
	{
  		for (i=0;i<(j-1);i++) 
		{
   			sum = a(i,j);
   			for (k=0;k<(i-1);k++) 
				sum -= a(i,k)*a(k,j);
   			a(i,j)=sum;
  		}
  		big=0.0;

  		for (i=j;i<n;i++) 
		{
   			sum=a(i,j);
   			for (k=0;k<(j-1);k++)
    				sum -= a(i,k)*a(k,j);

   			a(i,j)=sum;
   			if ( (dum=vv[i]*fabs(sum)) >= big) 
			{
    				big=dum;
    				imax=i;
   			}
  		}

  		if (j != imax) 
		{
   			for (k=0;k<n;k++) 
			{
    				dum=a(imax,k);
    				a(imax,k)=a(j,k);
    				a(j,k)=dum;
  			}
   			*d = -(*d);
   			vv[imax]=vv[j];
  		}
  		indx[j]=imax;
  		if (a(j,j) == 0.0) 
			a(j,j)=TINY;
  		if (j != n) 
		{
   			dum=1.0/(a(j,j));
   			for (i=j;i<n;i++) 
				a(i,j) *= dum;
  		}
 	}
 	delete vv;
}

template <class D>
double calculateGet(Matrix<D>& lu, double d)
{
	double dd = d;
	int n = 0;

	if(lu.getNumberOfRows() != lu.getNumberOfColumns())
		throw "Error: Matrix is not square";

	n = lu.getNumberOfColumns();

	for(int i = 0; i < n; i++)
		dd *= lu(i,i);

	return dd;
}

template <class D>
double getDet(Matrix<D>& a)
{
	D d;
	int* indx = NULL;

	if(a.getNumberOfRows() != a.getNumberOfColumns())
		throw "Error: Matrix is not square";
	
	indx = new int[a.getNumberOfRows()];

	ludcmp(a, indx, &d);

	return calculateGet(a, d);
}

		printf("res: %e\n",res);

		cout << res << endl;
