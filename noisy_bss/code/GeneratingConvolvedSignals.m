function []=GeneratingConvolvedSignals()

clc;

SpeakersRecordingsPath = '/Users/zz4fap/Desktop/BSS+ASR/data_base/speech/todos/treino/wav/';
NoisesRecordingsPath = '/Users/zz4fap/Desktop/BSS+ASR/data_base/noises/wav/short/';

convolvedSignalsPath = '/Users/zz4fap/Desktop/BSS+ASR/data_base/mixed/wav/8000/EqualizedEnergies/filterset5/';

fs = 8000; % Hz
duration = 0;

equalizeVectorsToGreaterLentg = false;

equalizeEnergyToGreaterOne = true;

filterset='filterset5';
[h11 h12 h21 h22] = GetFiltersForSimulation(filterset);

noises = struct('noisefile',{'babble.wav','airport.wav','restaurant.wav','street.wav','train.wav'});

speakers = GetArrayWithAllSpeakerRecordings(SpeakersRecordingsPath,1200,fs);

for speakerCounter=1:1:length(speakers)

    for noisesCounter=1:1:length(noises)

        speakerRecording = speakers(speakerCounter).speakerrecording;

        noiseRecording = noises(noisesCounter).noisefile;

        [x11 x12 x21 x22 bits x11Energy x12Energy x21Energy x22Energy MaxEigValue EigenValues] = GetXSignalsv5(SpeakersRecordingsPath, NoisesRecordingsPath, fs, duration, speakerRecording, noiseRecording, equalizeVectorsToGreaterLentg, equalizeEnergyToGreaterOne, h11, h12, h21, h22);

        % Valores das sa?das dos filtros de mixagem (h11, h12, h21, h22)
        x1 = x11 + x21;
        x2 = x12 + x22;

        maxx1 = sqrt(max(x1)^2);
        x1 = x1./maxx1;

        maxx2 = sqrt(max(x2)^2);
        x2 = x2./maxx2;

        genericFilename = strcat(speakerRecording,'_');
        genericFilename = strcat(genericFilename,noiseRecording);
        genericFilename = strcat(genericFilename,sprintf('_%i_',fs));
        genericFilename = strcat(genericFilename,filterset);
        filename_x1 = strcat(genericFilename,'_x1.wav');
        filename_x2 = strcat(genericFilename,'_x2.wav');

        % write signals to wav file.
        wavwrite(x1,fs,strcat(convolvedSignalsPath,filename_x1));
        wavwrite(x2,fs,strcat(convolvedSignalsPath,filename_x2));

    end
end

