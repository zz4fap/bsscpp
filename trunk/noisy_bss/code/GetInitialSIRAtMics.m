% clear all;clc
% 
% recordingPath='/home/zz4fap/Desktop/Recordings/Rodrigo&Lana/wav/';
% FS = 8000;
% duration = -1;
% wav1 = 'LanaLista19Amplified.wav';
% wav2 = 'RodrigoLista4Amplified.wav';
% 
% [x11 x12 x21 x22 InitialSIRatMic1 InitialSIRatMic2 AverageSIR] = GetInitialSIRAtMics(recordingPath, FS, duration, wav1, wav2);


function [x11 x12 x21 x22 InitialSIRatMic1 InitialSIRatMic2 AverageSIR x11Energy x12Energy x21Energy x22Energy] = GetInitialSIRAtMics(recordingPath, FS, duration, wav1, wav2)

%--------------------------------------------------------------------------
%Reading the speech signals of the two people, these signals will be
%x11,x12,x21 and x22.
%--------------------------------------------------------------------------

wavFilesPath = strcat(recordingPath, sprintf('%d',FS));

[x11x12, fs, bits] = GetSpeechSignal(wav1, wavFilesPath);
if(fs ~= FS)
    error('The Sampling Frequency of the wav file is different from the one set in the script');
end

if(duration >= 0)
    numberOfSamples = duration*FS;
else
    numberOfSamples = length(x11x12);
end

channel1Energy = 10*log10(Energy(x11x12(1,:)));
channel2Energy = 10*log10(Energy(x11x12(2,:)));

if(channel1Energy > channel2Energy)
    x11 = x11x12(1,1:numberOfSamples); %Higher Energy
    x12 = x11x12(2,1:numberOfSamples); %Lower Energy
    x11Energy = channel1Energy;
    x12Energy = channel2Energy;
    fprintf(1,'Wav1: Channel 1 Energy > Channel 2 Energy\n');
else
    x11 = x11x12(2,1:numberOfSamples); %Higher Energy
    x12 = x11x12(1,1:numberOfSamples); %Lower Energy
    x11Energy = channel2Energy;
    x12Energy = channel1Energy;
    fprintf(1,'Wav1: Channel 2 Energy > Channel 1 Energy\n');
end

%--------------------------------------------------------------------------

[x21x22, fs, bits] = GetSpeechSignal(wav2, wavFilesPath);
if(fs ~= FS)
    error('The Sampling Frequency of the wav file is different from the one set in the script');
end

channel1Energy = 10*log10(Energy(x21x22(1,:)));
channel2Energy = 10*log10(Energy(x21x22(2,:)));

if(channel1Energy > channel2Energy)
    x22 = x21x22(1,1:numberOfSamples); %Higher Energy
    x21 = x21x22(2,1:numberOfSamples); %Lower Energy
    x22Energy = channel1Energy;
    x21Energy = channel2Energy;
    fprintf(1,'Wav2: Channel 1 Energy > Channel 2 Energy\n');
else
    x22 = x21x22(2,1:numberOfSamples); %Higher Energy
    x21 = x21x22(1,1:numberOfSamples); %Lower Energy
    x22Energy = channel2Energy;
    x21Energy = channel1Energy;
    fprintf(1,'Wav2: Channel 2 Energy > Channel 1 Energy\n');
end

%--------------------------------------------------------------------------

if(x11Energy > x21Energy)
    SIRAtMic1 = Energy(x11)/Energy(x21);
    InitialSIRatMic1 = 10*log10(SIRAtMic1);
else
    error('x11Energy <= x21Energy');
end

if(x22Energy > x12Energy)
    SIRAtMic2 = Energy(x22)/Energy(x12);
    InitialSIRatMic2 = 10*log10(SIRAtMic2);
else
    error('x22Energy <= x12Energy');
end

AverageSIR = 10*log10((SIRAtMic1 + SIRAtMic2)/2);
