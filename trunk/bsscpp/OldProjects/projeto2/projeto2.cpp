#include<stdio.h>
#include<iostream.h>
#include<windows.h>
#include<io.h>
#include<stdlib.h>

int main(void)
{	

	HFILE arq;
	long pos;

	arq=_lopen("newemail.wav", OF_READ);
	if(arq==HFILE_ERROR)
		printf( "The file was not opened\n" );
	else
		printf( "The file was opened\n" );
	
	
	pos=_llseek( arq, 0L, 2 );
	if(pos==HFILE_ERROR)
		perror( "_lseek to beginning failed" );
	else
		printf( "Position for beginning of file seek = %ld\n", pos );

	pos=_llseek(arq,0,0);
	/*if(pos==HFILE_ERROR)
		perror( "_lseek to beginning failed" );
	else
		printf( "Position for beginning of file seek = %ld\n", pos );*/

	_close(arq);
	return(0);

}
