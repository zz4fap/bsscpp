CC = g++
CFLAGS =
INCLUDE = /home/zz4fap/Desktop/BSSCpp/Tools/FFT/FFTv5/
OBJECTS = Fourier.o Convolution.o ConvolutionUnitTest.o

ConvolutionUnitTest : $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o ConvolutionUnitTest

Fourier.o : $(INCLUDE)Fourier.cc $(INCLUDE)Fourier.hh
	$(CC) -c $(INCLUDE)Fourier.cc

Convolution.o : Fourier.o Convolution.hh Convolution.cc
	$(CC) -I$(INCLUDE) -c Convolution.cc

ConvolutionUnitTest.o : Fourier.o Convolution.o ConvolutionUnitTest.cc
	$(CC) -I$(INCLUDE) -c ConvolutionUnitTest.cc

clean :
	rm *.o
