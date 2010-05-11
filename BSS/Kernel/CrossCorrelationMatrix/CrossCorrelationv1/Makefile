CC = g++
CFLAGS =
FFT_INCLUDE = /home/zz4fap/Desktop/BSSCpp/Tools/FFT/FFTv5/
CORR_INCLUDE = /home/zz4fap/Desktop/BSSCpp/Tools/Correlation/Corrv3/
OBJECTS = Fourier.o Correlation.o CrossCorrelationMatrix.o CrossCorrelationMatrixUnitTest.o

CrossCorrelationMatrixUnitTest : $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o CrossCorrelationMatrixUnitTest

Fourier.o : $(FFT_INCLUDE)Fourier.cc $(FFT_INCLUDE)Fourier.hh
	$(CC) -c $(FFT_INCLUDE)Fourier.cc

Correlation.o : Fourier.o $(CORR_INCLUDE)Correlation.hh $(CORR_INCLUDE)Correlation.cc
	$(CC) -I$(FFT_INCLUDE) -c $(CORR_INCLUDE)Correlation.cc

CrossCorrelationMatrix.o : Fourier.o Correlation.o CrossCorrelationMatrix.hh CrossCorrelationMatrix.cc
	$(CC) -I$(CORR_INCLUDE) -I$(FFT_INCLUDE) -c CrossCorrelationMatrix.cc

CrossCorrelationMatrixUnitTest.o : Fourier.o Correlation.o CrossCorrelationMatrix.o CrossCorrelationMatrixUnitTest.cc
	$(CC) -I$(CORR_INCLUDE) -I$(FFT_INCLUDE) -c CrossCorrelationMatrixUnitTest.cc

clean :
	rm *.o
