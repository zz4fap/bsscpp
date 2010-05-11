CC = g++
CFLAGS =
OBJECTS = FourierUnitTest.o fourier1.o

FourierUnitTest : $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o FourierUnitTest

%.o : %.c
	$(CC) $(CFLAGS) -c $<

