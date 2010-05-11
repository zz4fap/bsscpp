CC = g++
CFLAGS = -O3
MATRIX_INCLUDE = /home/zz4fap/Desktop/BSSCpp/Tools/Matrix/Matrixv5/
AUTO_CORR_INCLUDE = /home/zz4fap/Desktop/BSSCpp/BSS/Kernel/AutoCorrelationMatrix/AutoCorrelationv4/
CROSS_CORR_INCLUDE = /home/zz4fap/Desktop/BSSCpp/BSS/Kernel/CrossCorrelationMatrix/CrossCorrelationv5/
FOURIER_INCLUDE = /home/zz4fap/Desktop/BSSCpp/Tools/FFT/FFTv7/
CORR_INCLUDE = /home/zz4fap/Desktop/BSSCpp/Tools/Correlation/Corrv5/
OBJECTS = BlockNaturalGradientWithSCUnitTestv2.o

BlockNaturalGradientWithSCv2 : $(OBJECTS)
	$(CC) -lpthread $(CFLAGS) $(OBJECTS) -o BlockNaturalGradientWithSCUnitTestv2

%.o : %.cc $(MATRIX_INCLUDE)Matrix.hh $(AUTO_CORR_INCLUDE)AutoCorrelationMatrix.hh $(CROSS_CORR_INCLUDE)CrossCorrelationMatrix.hh $(FOURIER_INCLUDE)Fourier.hh $(CORR_INCLUDE)Correlation.hh BlockNaturalGradientWithSC.hh BlockNaturalGradientWithSCUnitTestv2.cc
	$(CC) -lpthread -I$(MATRIX_INCLUDE) -I$(AUTO_CORR_INCLUDE) -I$(CROSS_CORR_INCLUDE) -I$(FOURIER_INCLUDE) -I$(CORR_INCLUDE) $(CFLAGS) -c $<

clean :
	rm *.o

