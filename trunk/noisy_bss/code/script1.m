%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                   Tests started on xx_11_2011
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%------- Testing the separation between aurora's noise data base and Ynoguti's speaker data base --------

% Script 1 -> 1 to 120
% fs = 8000
% Learning method: Fixed Step Size
% Noise: babble
% Number of epochs: 500

clear all;
clc;

NumberOfEpochs = 500;

fs = [8000]; % Sampling frequency is given in Hz

noises = struct('noisefile',{'babble.wav'});
speakers = GetSpeakersListFromFile('recordings_1');
numberOfRecordings = length(speakers);

filename = 'SimulationsInformationForRecordings_1.txt';
equalizeEnergyToGreaterOne = true;

% Location of the speech and noise files
speakersPath = '/Users/zz4fap/Desktop/BSS+ASR/data_base/speech/todos/treino/wav/';
noisesPath = '/Users/zz4fap/Desktop/BSS+ASR/data_base/noises/wav/short/';

if(equalizeEnergyToGreaterOne)
    savingPath = '/Users/zz4fap/Desktop/BSS+ASR/simulations/EqualizedEnergy/';
else
    savingPath = '/Users/zz4fap/Desktop/BSS+ASR/simulations/EnergyNotEqualized/';
end

stepsize = [0.002 0.004 0.006 0.008 0.01 0.03];

N = [256 256 512 512];
L = [64 128 128 256];

StartAndOrResumeSimulations(fs, speakers, noises, filename, N, L, stepsize, NumberOfEpochs, equalizeEnergyToGreaterOne, numberOfRecordings, speakersPath, noisesPath, savingPath);
