CC = g++
CFLAGS =
OBJECTS = FourierUnitTest.o

FourierUnitTest : $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o FourierUnitTest

FourierUnitTest.o : Fourier.hh FourierUnitTest.cc
	$(CC) -c FourierUnitTest.cc

clean :
	rm *.o

