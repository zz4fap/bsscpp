% Quando n??o se desejar a totalidade de amostras dos sinais originais,
% deve-se especificar o numero desejado de amostras (numSamples), se
% numSamples for igual a zero(0) isto quer dizer que se deseja todas as
% amostras.

% RecordingsPath = '/home/zz4fap/MATLAB_WORKSPACE/base/treino/wav/'
% SavingPath = '/home/zz4fap/Desktop/Recordings/OutrosAudios/YnogutiDataBase/'
% filename1 = 'F221610.wav';
% filename2 = 'M241309.wav';
% duration = 0;

function [s1 s2 bits] = GeneratingSpeechSignalsv3(duration, fs, speakerRecording, noiseRecording, SpeakersRecordingsPath, NoisesRecordingsPath, equalizeVectorsToGreaterLentg)

currentDir=pwd;

% Mudando para o diretorio com as locucoes
SpeakersRecordingsPath = strcat(SpeakersRecordingsPath, sprintf('%d',fs));
cd(SpeakersRecordingsPath);

% Obtendo os sinais originais
[s1 fs1 bits1] = wavread(speakerRecording);

% Mudando para o diretorio com os ruidos
NoisesRecordingsPath = strcat(NoisesRecordingsPath, sprintf('%d',fs));
cd(NoisesRecordingsPath);

% Obtendo os sinais originais
fprintf(1,'Noise file: %s\n',noiseRecording);
[s2 fs2 bits2] = wavread(noiseRecording);

if(fs1 ~= fs2 || bits1 ~=  bits2)
    error('Freq 1 is different from Freq 2 or bits 1 is different of bits 2');
else
    if(fs1 ~= fs)
        error('The Sampling Frequency of the wav file is different from the one set in the script');
    end
    bits = bits1;
end

% Retornando ao diretorio de trabalho
cd(currentDir);

if(equalizeVectorsToGreaterLentg)
    %Fazendo com que as vetores colunas tenham o mesmo numero de elementos
    %do maior vetor.
    if (size(s1,1) > size(s2,1))
        smallersignal = size(s2,1);
        biggersignal = size(s1,1);
        r = biggersignal - smallersignal;
        if(r > smallersignal)
            for i=1:1:floor(r/smallersignal)
                s2 = [s2; s2(1:smallersignal)];
            end
            fullfill = biggersignal - size(s2,1);
            s2 = [s2; s2(1:fullfill)];
        else
            s2 = [s2; s2(1:r)];
        end

    else
        smallersignal = size(s1,1);
        biggersignal = size(s2,1);
        r = biggersignal - smallersignal;
        if(r > smallersignal)
            for i=1:1:floor(r/smallersignal)
                s1 = [s1; s1(1:smallersignal)];
            end
            fullfill = biggersignal - size(s1,1);
            s1 = [s1; s1(1:fullfill)];
        else
            s1 = [s1; s1(1:r)];
        end
    end

    if(duration > 0)
        numberOfSamples = floor(duration*fs);
        s1 = s1(1:numberOfSamples,:);
        s2 = s2(1:numberOfSamples,:);
    end
else
    %Fazendo com que as vetores colunas tenham o mesmo numero de elementos
    %do menor vetor.
    if (size(s1,1) > size(s2,1))
        s1 = s1(1:length(s2));
    else
        s2 = s2(1:length(s1));
    end
end

s1 = s1';
s2 = s2';
%---------------------


