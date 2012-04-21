CC = g++
CFLAGS =
OBJECTS = /home/zz4fap/Desktop/ReadingWaveFiles/FFT/FFTv4/FourierUnitTest.o /home/zz4fap/Desktop/ReadingWaveFiles/FFT/FFTv4/Fourier.o

FourierUnitTest : $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o FourierUnitTest

%.o : %.c
	$(CC) $(CFLAGS) -c $<

clean :
	rm *.o

