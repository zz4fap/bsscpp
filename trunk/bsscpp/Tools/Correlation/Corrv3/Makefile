CC = g++
CFLAGS =
INCLUDE = /home/zz4fap/Desktop/BSSCpp/Tools/FFT/FFTv5/
OBJECTS = Fourier.o Correlation.o CorrelationUnitTest.o

CorrelationUnitTest : $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o CorrelationUnitTest

Fourier.o : $(INCLUDE)Fourier.cc $(INCLUDE)Fourier.hh
	$(CC) -c $(INCLUDE)Fourier.cc

Correlation.o : Fourier.o Correlation.hh Correlation.cc
	$(CC) -I$(INCLUDE) -c Correlation.cc

CorrelationUnitTest.o : Fourier.o Correlation.o CorrelationUnitTest.cc
	$(CC) -I$(INCLUDE) -c CorrelationUnitTest.cc

clean :
	rm *.o
