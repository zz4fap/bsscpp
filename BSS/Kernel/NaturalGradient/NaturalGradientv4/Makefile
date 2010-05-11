CC = g++
CFLAGS =
MATRIX_INCLUDE = /home/zz4fap/Desktop/BSSCpp/Tools/Matrix/Matrixv5/
AUTO_CORR_INCLUDE = /home/zz4fap/Desktop/BSSCpp/BSS/Kernel/AutoCorrelationMatrix/AutoCorrelationv4/
CROSS_CORR_INCLUDE = /home/zz4fap/Desktop/BSSCpp/BSS/Kernel/CrossCorrelationMatrix/CrossCorrelationv3/
FOURIER_INCLUDE = /home/zz4fap/Desktop/BSSCpp/Tools/FFT/FFTv6/
CORR_INCLUDE = /home/zz4fap/Desktop/BSSCpp/Tools/Correlation/Corrv4/
OBJECTS = BlockNaturalGradientWithSCUnitTest.o

BlockNaturalGradientWithSC : $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o BlockNaturalGradientWithSCUnitTest

%.o : %.cc $(MATRIX_INCLUDE)Matrix.hh $(AUTO_CORR_INCLUDE)AutoCorrelationMatrix.hh $(CROSS_CORR_INCLUDE)CrossCorrelationMatrix.hh $(FOURIER_INCLUDE)Fourier.hh $(CORR_INCLUDE)Correlation.hh BlockNaturalGradientWithSC.hh BlockNaturalGradientWithSCUnitTest.cc
	$(CC) -I$(MATRIX_INCLUDE) -I$(AUTO_CORR_INCLUDE) -I$(CROSS_CORR_INCLUDE) -I$(FOURIER_INCLUDE) -I$(CORR_INCLUDE) $(CFLAGS) -c $<

clean :
	rm *.o

