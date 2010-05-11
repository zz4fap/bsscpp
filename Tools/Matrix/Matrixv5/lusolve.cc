/*\file  lusolve.cc
*
*   Solve linear equations by LU decomposition
*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "matrix.h"
#include <string>
using namespace std;

char text[128];
void print_vector(double *v, int n);


void lusolve(Matrix &mx, FILE *in);
int ludcmp(Matrix &a, int order[]);
void solvlu(const Matrix &a, const Vec &b, Vec &x, const int order[]);

int main(int argc, char *argv[])
{
	Matrix mx;
	FILE *in;
	char *filename;

	if (argc>1) {
		filename = argv[1];
		printf("opening: %s\n",filename);
	}
	else {
		printf("This program requires a data file.\n");
		printf("Enter filename: ");
		gets_s(text,72);
		if (*text==0 || *text=='\n') return -1;
		filename = text;
	}
	errno_t nerr = fopen_s(&in, filename,"rt");
	if (nerr) {
		perror(filename);
		return -1;
	}

	string title;
	while (read_matrix(mx,title,in)==0) {
		printf("\n%s\n",title.c_str());
		printf("\nMatrix A\n");
		mx.print();
		lusolve(mx, in);
	}
	return 0;
}

void lusolve(Matrix &mx, FILE *in)
{
	int i, j, k, n, flag, index, nrhs;
	int *ipvt;
	double sum;
	Vec b, x, save;
	double det;
	/*
	*   Create matrix a as a copy of input matrix.
	*/
	assert(mx.rows()==mx.cols()); // input must be square matrix
	n = mx.rows();

	Matrix a = mx;
	ipvt = ivector(n);

	flag = ludcmp(a,ipvt);

	/* Calculate determinant */

	det = flag;
	for (i=0; i<n; i++) det *= a[i][i];

	printf("\ndeterminant: %g\n",det);
	printf("pivot vector: ");
	for (i=0; i<n; i++) printf("%3d",ipvt[i]);
	printf("\n");

	/* print LU Matrix */

	Matrix lower = a;
	Matrix upper = a;
	Matrix product(n, n);

	for (i=0; i<n; i++) {
		for (j=i+1; j<n; j++) lower[i][j] = 0.0;
		for (j=0; j<i; j++) upper[i][j] = 0.0;
		upper[i][i] = 1.0;
	}
	printf("\nLower triangular Matrix\n");
	lower.print();
	printf("\nUpper triangular Matrix\n");
	upper.print();

	/* Multiply the lower and upper matrices */

	for (i=0; i<n; i++) {
		for (j=0; j<n; j++) {
			sum = 0.0;
			for (k=0; k<n; k++) sum += lower[i][k] * upper[k][j];
			product[i][j] = sum;
		}
	}
	printf("\n\nProduct of L U\n\n");
	product.print();

	/*
	*   Read in the right-hand side vectors.
	*/
	fscanf_s(in," %d",&nrhs);
	fgets(text,72,in);
	x.create(n);
	b.create(n);
	index = 0;
	while (nrhs--) {
		for (i=0; i<n; i++) fscanf_s(in," %lf", &b[i]);
		if (feof(in)) break;
		printf("\nRight-hand-side number %d\n\n",++index);
		b.print();
		save = b;

		if (fabs(det)<1e-10) {
			printf("\nCoefficient Matrix is singular.\n");
			continue;
		}
		solvlu(a,b,x,ipvt);
		printf("\nSolution vector\n\n");
		x.print();
				
		k = 0;
		for (i=0; i<n; i++) {
			sum = 0.0;
			for (j=0; j<n; j++) sum += mx[i][j]*x[j];
			if (fabs(sum-save[i])>1e-8) k++;
		}
		printf("\nsolution %s.\n",(k? "does not check": "checks"));
	}
	free(ipvt);
}

void print_vector(double *v, int n)
{
	while (n--) printf("%10.4f", *v++);
	printf("\n");
}

