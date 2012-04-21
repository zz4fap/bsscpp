#include <string.h>
#include <iostream>
#include <string>
#include <fstream>
#include <stdlib.h>
#include <vector>
#include <math.h>

using namespace std;

int main()
{
	FILE* arquivo = NULL;
	double value = 0;

	if( (arquivo = fopen( "deltaW4.dat", "rb" )) == NULL )
		cout << "The file was not opened" << endl;
	else
		cout << "The file was opened" << endl;

	do
	{
		if (fread(&value, sizeof(double), 1, arquivo) != 1) 
			cout <<  "Erro de leitura" << endl;
		if(value != 0)
			cout << value << endl;

	}while(!feof(arquivo) && !ferror(arquivo));
	fclose(arquivo);

	cout << sizeof(long double) << endl;

	return 0;
}
