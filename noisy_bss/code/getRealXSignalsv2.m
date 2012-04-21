function [x11 x12 x21 x22 x11Energy x12Energy x21Energy x22Energy MaxEigValue EigenValues] = getRealXSignalsv2(a11, a12, a21, a22)

a11Energy = 10*log10(Energy(a11));
a12Energy = 10*log10(Energy(a12));

if(a11Energy > a12Energy)
    x11 = a11; %Higher Energy
    x12 = a12; %Lower Energy
    x11Energy = a11Energy;
    x12Energy = a12Energy;
    fprintf(1,'Wav1: Channel 1 Energy > Channel 2 Energy\n');
else
    error('There was an error: Wav1: Channel 2 Energy > Channel 1 Energy, i.e. x12 > x11!!');
end

%--------------------------------------------------------------------------

a21Energy = 10*log10(Energy(a21));
a22Energy = 10*log10(Energy(a22));

if(a22Energy > a21Energy)
    x22 = a22; %Higher Energy
    x21 = a21; %Lower Energy
    x22Energy = a22Energy;
    x21Energy = a21Energy;
    fprintf(1,'Wav2: Channel 1 Energy > Channel 2 Energy\n');
else
    error('There was an error: Wav2: Channel 2 Energy > Channel 1 Energy, i.e. x21 > x22!!');
end

[MaxEigValue EigenValues] = GetEigenValues(x11, x12, x21, x22);
