%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                   Tests started on xx_11_2011
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%------- Testing separation between aurora's data base noise and Ynoguti's data base speaker--------

% Script 2 -> 121 to 240
% fs = 8000
% Learning method: Fixed Step Size
% Noise: babble
% Number of epochs: 500

clear all;
clc;

fs = [8000]; % Sampling frequency is given in Hz

noises = struct('noisefile',{'babble.wav'});
speakers = GetSpeakersListFromFile('recordings_2');

filename = 'SimulationsInformation.txt';
equalizeEnergyToGreaterOne = true;

stepsize = [0.002 0.004 0.006 0.008 0.01 0.03];

N = [256 256 512 512];
L = [64 128 128 256];

[result nextN nextL nextStepsize recordingCounter] = ResumeSimulationv2(filename, N, L, stepsize, equalizeEnergyToGreaterOne);

if(result>=0)
    actualN = [nextN];
    actualL = [nextL];
    actualStepSize = [nextStepsize];

    speaker = struct('speakerrecording', {});
    
    j = 1;
    for i=recordingCounter:1:length(speakers)
        speaker(j).speakerrecording=speakers(recordingCounter).speakerrecording;
        j=j+1;
    end
    
    fprintf(1,'Next N: %d\n',nextN);
    fprintf(1,'Next L: %d\n',nextL);
    fprintf(1,'Step Size: %1.3f\n',nextStepsize);
    fprintf(1,'recordingCounter: %d\n',recordingCounter);
    fprintf(1,'Recording: %s\n',speaker(1).speakerrecording);

    ConfigurationScriptv3(fs, speaker, noises, actualStepSize, actualN, actualL, equalizeEnergyToGreaterOne);
else
    ConfigurationScriptv3(fs, speakers, noises, stepsize, N, L, equalizeEnergyToGreaterOne);
end
