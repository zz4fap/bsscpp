CC = g++
CFLAGS =
FFT_INCLUDE = /home/zz4fap/Desktop/BSSCpp/Tools/FFT/FFTv7/
CORR_INCLUDE = /home/zz4fap/Desktop/BSSCpp/Tools/Correlation/Corrv4/
MATRIX_INCLUDE = /home/zz4fap/Desktop/BSSCpp/Tools/Matrix/Matrixv5/
OBJECTS = AutoCorrelationMatrixUnitTest.o

AutoCorrelationMatrixUnitTest : $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o AutoCorrelationMatrixUnitTest

AutoCorrelationMatrixUnitTest.o : $(MATRIX_INCLUDE)Matrix.hh $(FFT_INCLUDE)Fourier.hh $(CORR_INCLUDE)Correlation.hh AutoCorrelationMatrix.hh AutoCorrelationMatrixUnitTest.cc
	$(CC) -I$(CORR_INCLUDE) -I$(FFT_INCLUDE) -I$(MATRIX_INCLUDE) -c AutoCorrelationMatrixUnitTest.cc

clean :
	rm *.o
