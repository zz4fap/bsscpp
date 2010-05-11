CC = g++
CFLAGS =
OBJECTS = MatrixUnitTest.o

MatrixUnitTest : $(OBJECTS)
	$(CC) -fbounds-check $(CFLAGS) $(OBJECTS) -o MatrixUnitTest

MatrixUnitTest.o : Matrix.hh MatrixUnitTest.cc
	$(CC) -c MatrixUnitTest.cc

clean :
	rm *.o

