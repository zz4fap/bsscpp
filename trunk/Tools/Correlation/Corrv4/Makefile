CC = g++
CFLAGS =
FFT_INCLUDE = /home/zz4fap/Desktop/BSSCpp/Tools/FFT/FFTv6/
OBJECTS = CorrelationUnitTest.o

CorrelationUnitTest : $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o CorrelationUnitTest

CorrelationUnitTest.o : $(FFT_INCLUDE)Fourier.hh Correlation.hh CorrelationUnitTest.cc
	$(CC) -I$(FFT_INCLUDE) -c CorrelationUnitTest.cc

clean :
	rm *.o
