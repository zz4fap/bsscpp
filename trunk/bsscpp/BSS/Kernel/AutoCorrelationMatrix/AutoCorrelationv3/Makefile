CC = g++
CFLAGS =
FFT_INCLUDE = /home/zz4fap/Desktop/BSSCpp/Tools/FFT/FFTv5/
CORR_INCLUDE = /home/zz4fap/Desktop/BSSCpp/Tools/Correlation/Corrv3/
MATRIX_INCLUDE = /home/zz4fap/Desktop/BSSCpp/Tools/Matrix/Matrixv2/
OBJECTS = Fourier.o Correlation.o AutoCorrelationMatrix.o AutoCorrelationMatrixUnitTest.o

AutoCorrelationMatrixUnitTest : $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o AutoCorrelationMatrixUnitTest

Fourier.o : $(FFT_INCLUDE)Fourier.cc $(FFT_INCLUDE)Fourier.hh
	$(CC) -c $(FFT_INCLUDE)Fourier.cc

Correlation.o : Fourier.o $(CORR_INCLUDE)Correlation.hh $(CORR_INCLUDE)Correlation.cc
	$(CC) -I$(FFT_INCLUDE) -c $(CORR_INCLUDE)Correlation.cc

AutoCorrelationMatrix.o : $(MATRIX_INCLUDE)Matrix.hh Fourier.o Correlation.o AutoCorrelationMatrix.hh AutoCorrelationMatrix.cc
	$(CC) -I$(CORR_INCLUDE) -I$(FFT_INCLUDE) -I$(MATRIX_INCLUDE) -c AutoCorrelationMatrix.cc

AutoCorrelationMatrixUnitTest.o : $(MATRIX_INCLUDE)Matrix.hh Fourier.o Correlation.o AutoCorrelationMatrix.o AutoCorrelationMatrixUnitTest.cc
	$(CC) -I$(CORR_INCLUDE) -I$(FFT_INCLUDE) -I$(MATRIX_INCLUDE) -c AutoCorrelationMatrixUnitTest.cc

clean :
	rm *.o
