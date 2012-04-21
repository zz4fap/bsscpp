function []=TestGetXSignalv5()

clc;

SpeakersRecordingsPath = '/Users/zz4fap/Desktop/BSS+ASR/data_base/speech/todos/treino/wav/';
NoisesRecordingsPath = '/Users/zz4fap/Desktop/BSS+ASR/data_base/noises/wav/short/';

fs = 8000; % Hz
duration = 0;

equalizeVectorsToGreaterLentg = false;

filterset='filterset5';
[h11 h12 h21 h22] = GetFiltersForSimulation(filterset);

noises = struct('noisefile',{'babble.wav','airport.wav','restaurant.wav','street.wav','train.wav'});

speakers = GetArrayWithAllSpeakerRecordings(SpeakersRecordingsPath,1200,fs);

for speakerCounter=1:1:length(speakers)

    for noisesCounter=1:1:length(noises)
        
        speakerRecording = speakers(speakerCounter).speakerrecording;
        
        noiseRecording = noises(noisesCounter).noisefile;

        [x11 x12 x21 x22 bits x11Energy x12Energy x21Energy x22Energy MaxEigValue EigenValues] = GetXSignalsv5(SpeakersRecordingsPath, NoisesRecordingsPath, fs, duration, speakerRecording, noiseRecording, equalizeVectorsToGreaterLentg, h11, h12, h21, h22);

    end
end

