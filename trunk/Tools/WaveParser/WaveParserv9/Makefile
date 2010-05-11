CC = g++
CFLAGS =
OBJECTS = WaveParserUnitTest.o WaveParser.o

WaveParserUnitTest : $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o WaveParserUnitTest

%.o : %.c
	$(CC) $(CFLAGS) -c $<

clean :
	rm *.o

