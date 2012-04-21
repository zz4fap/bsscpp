CC = g++
CFLAGS =
FFT_INCLUDE = /home/zz4fap/Desktop/BSSCpp/Tools/FFT/FFTv7/
CORR_INCLUDE = /home/zz4fap/Desktop/BSSCpp/Tools/Correlation/Corrv5/
MATRIX_INCLUDE = /home/zz4fap/Desktop/BSSCpp/Tools/Matrix/Matrixv5/
OBJECTS = CrossCorrelationMatrixUnitTest.o

CrossCorrelationMatrixUnitTest : $(OBJECTS)
	$(CC) -lpthread $(CFLAGS) $(OBJECTS) -o CrossCorrelationMatrixUnitTest

CrossCorrelationMatrixUnitTest.o : $(MATRIX_INCLUDE)Matrix.hh $(FFT_INCLUDE)Fourier.hh $(CORR_INCLUDE)Correlation.hh CrossCorrelationMatrix.hh CrossCorrelationMatrixUnitTest.cc
	$(CC) -lpthread -I$(CORR_INCLUDE) -I$(FFT_INCLUDE) -I$(MATRIX_INCLUDE) -c CrossCorrelationMatrixUnitTest.cc

clean :
	rm *.o

