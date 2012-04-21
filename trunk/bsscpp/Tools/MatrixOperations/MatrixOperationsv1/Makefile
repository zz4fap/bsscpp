CC = g++
CFLAGS =
MATRIX_INCLUDE = /home/zz4fap/Desktop/BSSCpp/Tools/Matrix/Matrixv2/
OBJECTS = MatrixOperationsUnitTest.o

MatrixOperationsUnitTest : $(OBJECTS)
	$(CC) -fbounds-check $(CFLAGS) $(OBJECTS) -o MatrixOperationsUnitTest

MatrixOperationsUnitTest.o : $(MATRIX_INCLUDE)Matrix.hh MatrixOperations.hh MatrixOperationsUnitTest.cc 
	$(CC) -I$(MATRIX_INCLUDE) -c MatrixOperationsUnitTest.cc

clean :
	rm *.o

