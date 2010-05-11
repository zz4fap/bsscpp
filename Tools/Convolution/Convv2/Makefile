CC = g++
CFLAGS =
FFT_INCLUDE = /home/zz4fap/Desktop/BSSCpp/Tools/FFT/FFTv7/
OBJECTS = ConvolutionUnitTest.o

ConvolutionUnitTest : $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o ConvolutionUnitTest

ConvolutionUnitTest.o : $(FFT_INCLUDE)Fourier.hh Convolution.hh ConvolutionUnitTest.cc
	$(CC) -I$(FFT_INCLUDE) -c ConvolutionUnitTest.cc

clean :
	rm *.o
