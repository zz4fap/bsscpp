/*\file ludcmp.cc
*
*!  Finds solution to set of linear equations A x = b by LU decomposition.
*
*  Chapter 2, Programs 3-5, Fig. 2.8-2.10
*  Gerald/Wheatley, APPLIED NUMERICAL ANALYSIS (fourth edition)
*  Addison-Wesley, 1989
*
*/
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>

int ludcmp(Matrix &a, int order[]);
void solvlu(const Matrix &a, const Vec &b, Vec &x, const int order[]);
static int pivot(Matrix &a, int order[], int jcol);

#define TINY 1e-20
//! finds LU decomposition of Matrix
/*!
*   The function ludcmp computes the lower L and upper U triangular
*   matrices equivalent to the A Matrix, such that L U = A.  These
*   matrices are returned in the space of A, in compact form.
*   The U Matrix has ones on its diagonal.  Partial pivoting is used
*   to give maximum valued elements on the diagonal of L.  The order of
*   the rows after pivoting is returned in the integer vector "order".
*   This should be used to reorder the right-hand-side vectors before
*   solving the system A x = b.
*
*
* \param       a     - n by n Matrix of coefficients
* \param      order - integer vector holding row order after pivoting.
*
*/

int ludcmp(Matrix &a, int order[])
{
	int i, j, k, n, nm1;
	int flag = 1;    /* changes sign with each row interchange */
	double sum, diag;

	n = a.rows();
	assert(a.cols()==n);

	/* establish initial ordering in order vector */
	
	for (i=0; i<n; i++) order[i] = i;

	/* do pivoting for first column and check for singularity */

	if (pivot(a,order,0)) flag = -flag;
	diag = 1.0/a[0][0];
	for (i=1; i<n; i++) a[0][i] *= diag;
	
	/* 
	*  Now complete the computing of L and U elements.
	*  The general plan is to compute a column of L's, then
	*  call pivot to interchange rows, and then compute
	*  a row of U's.
	*/
	
	nm1 = n - 1;
	for (j=1; j<nm1; j++) {
		/* column of L's */
		for (i=j; i<n; i++) {
			sum = 0.0;
			for (k=0; k<j; k++) sum += a[i][k]*a[k][j];
			a[i][j] -= sum;
		}
		/* pivot, and check for singularity */
		if (pivot(a,order,j)) flag = -flag;
		/* row of U's */
		diag = 1.0/a[j][j];
		for (k=j+1; k<n; k++) {
			sum = 0.0;
			for (i=0; i<j; i++) sum += a[j][i]*a[i][k];
			a[j][k] = (a[j][k]-sum)*diag;
		}
	}

	/* still need to get last element in L Matrix */

	sum = 0.0;
	for (k=0; k<nm1; k++) sum += a[nm1][k]*a[k][nm1];
	a[nm1][nm1] -= sum;
	return flag;
}

//!  Find pivot element
/*!
*   The function pivot finds the largest element for a pivot in "jcol"
*   of Matrix "a", performs interchanges of the appropriate
*   rows in "a", and also interchanges the corresponding elements in
*   the order vector.
*
*
*  \param     a      -  n by n Matrix of coefficients
*  \param   order  - integer vector to hold row ordering
*  \param    jcol   - column of "a" being searched for pivot element
*
*/
int pivot(Matrix &a, int order[], int jcol)
{
	int i, ipvt,n;
	double big, anext;
	n = a.rows();

	/*
	*  Find biggest element on or below diagonal.
	*  This will be the pivot row.
	*/

	ipvt = jcol;
	big = fabs(a[ipvt][ipvt]);
	for (i = ipvt+1; i<n; i++) {
		anext = fabs(a[i][jcol]);
		if (anext>big) {
			big = anext;
			ipvt = i;
		}
	}
	assert(fabs(big)>TINY); // otherwise Matrix is singular
	
	/*
	*   Interchange pivot row (ipvt) with current row (jcol).
	*/
	
	if (ipvt==jcol) return 0;
	a.swaprows(jcol,ipvt);
	i = order[jcol];
	order[jcol] = order[ipvt];
	order[ipvt] = i;
	return 1;
}

//!  This function is used to find the solution to a system of equations,
/*!   A x = b, after LU decomposition of A has been found.
*    Within this routine, the elements of b are rearranged in the same way
*    that the rows of a were interchanged, using the order vector.
*    The solution is returned in x.
*
*
*  \param  a     - the LU decomposition of the original coefficient Matrix.
*  \param  b     - the vector of right-hand sides
*  \param       x     - the solution vector
*  \param    order - integer array of row order as arranged during pivoting
*
*/
void solvlu(const Matrix &a, const Vec &b, Vec &x, const int order[])
{
	int i,j,n;
	double sum;
	n = a.rows();

	/* rearrange the elements of the b vector. x is used to hold them. */

	for (i=0; i<n; i++) {
		j = order[i];
		x[i] = b[j];
	}

	/* do forward substitution, replacing x vector. */

	x[0] /= a[0][0];
	for (i=1; i<n; i++) {
		sum = 0.0;
		for (j=0; j<i; j++) sum += a[i][j]*x[j];
		x[i] = (x[i]-sum)/a[i][i];
	}

	/* now get the solution vector, x[n-1] is already done */

	for (i=n-2; i>=0; i--) {
		sum = 0.0;
		for (j=i+1; j<n; j++) sum += a[i][j] * x[j];
		x[i] -= sum;
	}
}

