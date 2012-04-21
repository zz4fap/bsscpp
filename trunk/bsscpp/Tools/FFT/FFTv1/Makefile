CC = g++
CFLAGS =
OBJECTS = FourierUnitTest.o Fourier.o

FourierUnitTest : $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o FourierUnitTest

%.o : %.c
	$(CC) $(CFLAGS) -c $<

