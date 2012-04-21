/* FOPEN.C: This program opens files named "data"
 * and "data2".It  uses fclose to close "data" and
 * _fcloseall to close all remaining files.
 */

#include <stdio.h>
#include <windows.h>

FILE *stream, *stream2;

void main( void )
{
   int numclosed;

   /* Open for read (will fail if file "data" does not exist) */
   if( (stream  = fopen( "newalert.wav", "r" )) == NULL )
      printf( "The file 'te' was not opened\n" );
   else
      printf( "The file 'te' was opened\n" );

   /* Open for write */
   if( (stream2 = fopen( "online.wav", "r" )) == NULL )
      printf( "The file 'online' was not opened\n" );
   else
      printf( "The file 'online' was opened\n" );

   /* Close stream */
   if( fclose( stream ) )
      printf( "The file 'data' was not closed\n" );

   /* All other files are closed: */
   numclosed = _fcloseall( );
   printf( "Number of files closed by _fcloseall: %u\n", numclosed );
}


