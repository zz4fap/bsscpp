clear all;clc

recordingPath='C:\Users\felipe-augusto\Desktop\Recordings\Rodrigo&Lana\wav\';
FS = 16000;
duration = -1;
wav1 = 'LanaLista19Normal.wav';
wav2 = 'RodrigoLista4Normal.wav';

speaker1='Lana';
speaker2='Rodrigo';

varyingSignalAtMic1 = 'x21';
varyingSignalAtMic2 = 'x12';

minerror = 0.01;
bits = 16;

DesiredSIRs = 1;
newSIR = DesiredSIRs;

savingPath = 'C:\Users\felipe-augusto\Desktop\Recordings\Rodrigo&Lana\wav\';
savingPath = strcat(savingPath,sprintf('%d\\',FS));

[x11 x12 x21 x22 InitialSIRatMic1 InitialSIRatMic2 AverageSIR x11Energy x12Energy x21Energy x22Energy] = GetInitialSIRAtMics(recordingPath, FS, duration, wav1, wav2);

if(strcmp(varyingSignalAtMic1, 'x11'))
    newx11 = x11./(10^((InitialSIRatMic1-DesiredSIRs)/20));
    newx21 = x21;
elseif(strcmp(varyingSignalAtMic1, 'x21'))
    newx21 = x21./(10^((DesiredSIRs-InitialSIRatMic1)/20));
    newx11 = x11;
else
    error('Wrong Signal name!!!!');
end

newSIRatMic1 = 10*log10(Energy(newx11)/Energy(newx21));
if((newSIRatMic1 > (newSIR+minerror)) || (newSIRatMic1 < (newSIR-minerror)))
    fprintf(1,'newSIRatMic1: %1.2f\n',newSIRatMic1);
    error('newSIRatMic1 ~= newSIR !!!');
end

if(strcmp(varyingSignalAtMic2, 'x22'))
    newx22 = x22./(10^((InitialSIRatMic2-DesiredSIRs)/20));
    newx12 = x12;
elseif(strcmp(varyingSignalAtMic2, 'x12'))
    newx12 = x12./(10^((DesiredSIRs-InitialSIRatMic2)/20));
    newx22 = x22;
else
    error('Wrong Signal name!!!!');
end

newSIRatMic2 = 10*log10(Energy(newx22)/Energy(newx12));
if((newSIRatMic2 > (newSIR+minerror)) || (newSIRatMic2 < (newSIR-minerror)))
    fprintf(1,'newSIRatMic2: %1.2f\n',newSIRatMic2)
    error('newSIRatMic2 ~= newSIR !!!');
end

%     filename = sprintf('%ddBAtBothMics.mat',newSIR);
%     filename = strcat(savingPath,filename);
%     save(filename,'newx11','newx12','newx21','newx22');

%     max1 = max([max(newx11) max(newx21)]);
%     newx11 = newx11/max1;
%     newx21 = newx21/max1;
%     10*log10(Energy(newx11))
%     10*log10(Energy(newx21))
%     newSIRatMic1 = 10*log10(Energy(newx11)/Energy(newx21));
%
%     max2 = max([max(newx22) max(newx12)]);
%     newx22 = newx22/max2;
%     newx12 = newx12/max2;
%     newSIRatMic2 = 10*log10(Energy(newx22)/Energy(newx12));

% x1 = newx11 + newx21;
% max1 = sqrt(max(x1)^2);
% x1 = x1./max1;

% sound(x1(1,1:40000),FS,bits)
% pause


% x2 = newx12 + newx22;
% max2 = sqrt(max(x2)^2);
% x2 = x2./max2;

% sound(x2(1,1:40000),FS,bits)
% pause



speaker1Wav = [newx11' newx12'];
speaker2Wav = [newx22' newx21'];


% x = [x1' x2'];

filename = sprintf('%sWith%ddBAtBothMics.wav',speaker1,newSIR);
filename = strcat(savingPath,filename);
wavwrite(speaker1Wav,FS,filename);

filename = sprintf('%sWith%ddBAtBothMics.wav',speaker2,newSIR);
filename = strcat(savingPath,filename);
wavwrite(speaker2Wav,FS,filename);
