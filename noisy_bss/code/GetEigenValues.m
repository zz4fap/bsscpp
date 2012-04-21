function [MaxEigValue EigenValues] = GetEigenValues(x11, x12, x21, x22)

x1 = x11 + x12;
x2 = x21 + x22;

%Remove o n??vel DC
x1 = x1 - mean(x1);
x2 = x2 - mean(x2);

R11 = (1/size(x1,2))*(x1*x1');
R22 = (1/size(x2,2))*(x2*x2');
R12 = (1/size(x2,2))*(x1*x2');
R21 = R12;

Scross = [R11 R12;R21 R22];

[EigenVectors,EigenValues] = eig(Scross);

if(EigenValues(1,1) > EigenValues(2,2))
    MaxEigValue = EigenValues(1,1);
else
    MaxEigValue = EigenValues(2,2);
end