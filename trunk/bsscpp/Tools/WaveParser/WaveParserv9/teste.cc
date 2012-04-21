//g++ WaveParserUnitTest.cc WaveParser.cc -o WaveParserUnitTest

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <iomanip>

using namespace std;

int main()
{
	char vector[3][3] = {{'A','B','E'},{'C','D','J'},{'Z','X','K'}};

	for(int i=0; i<3; i++)
		for(int j=0; j<3; j++)
	printf("Pointer: %p\n",&vector[i][j]);

	printf("\n\n\nPointer: %p\n",&vector[0][0]);
	printf("\n\n\nPointer: %p\n",&vector[1][0]);
	printf("\n\n\nPointer: %p\n\n\n\n\n\n",&vector[2][0]);


	printf("\n\n\nPointer: %p\n",vector);
	printf("\n\n\nPointer: %p\n",vector[0]);
	printf("\n\n\nPointer: %p\n",vector[1]);
	printf("\n\n\nPointer: %p\n",vector[2]);

	//cout << vector[1] << endl;



	/*FILE* arquivo = NULL;

	if( (arquivo = fopen( "felipeLeftFloat.dat", "rb" )) == NULL )
		cout << "The file was not opened" << endl;
	else
		cout << "The file was opened" << endl;


	float buffer[100];
	if (fread(buffer, sizeof(float), 100, arquivo) != 100) 
		cout <<  "Erro de leitura" << endl;

	for(int i=0; i<100; i++)
		cout << buffer[i] << endl;*/



	//short t = 0xAABBCC;
	//cout << hex << setfill('0') << setw(2) << t << endl;



	return 0;
}
