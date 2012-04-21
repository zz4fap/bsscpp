function [h11 h12 h21 h22] = GenerateFilterWithImageMethod(fs, src1, src2, rm, mic1, mic2, r, n)

h11=rir(fs, mic1, n, r, rm, src1);
h12=rir(fs, mic2, n, r, rm, src1);
h21=rir(fs, mic1, n, r, rm, src2);
h22=rir(fs, mic2, n, r, rm, src2);

sizeh11 = length(h11);
sizeh12 = length(h12);

if(sizeh11 > sizeh12)
    minsize = sizeh12;
else
    minsize = sizeh11;
end

h11=h11(1:minsize,1);
h12=h12(1:minsize,1);
h21=h21(1:minsize,1);
h22=h22(1:minsize,1);

h11=h11';
h12=h12';
h21=h21';
h22=h22';
