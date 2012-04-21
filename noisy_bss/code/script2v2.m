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
speakers = struct('speakerrecording',{'M020701.WAV'});
noises = struct('noisefile',{'babble.wav'});

stepsize = [0.004];

N = [512]; %N = 1024;%1000;%1000;%6000; %220,330, 660, 1200, 2400, 4800, 9600, 19200
L = [256]; %L = 512;%400;%95;%100;%3, 6, 12Numero de taps dos filtros de de-mixagem(ou seja, a ordem dos filtros)

ConfigurationScriptv2(fs, speakers, noises, stepsize, N, L)

%------- Testing separation between aurora's data base noise and Ynoguti's data base speaker--------

% Script 2 -> 121 to 240
% fs = 8000
% Learning method: Fixed Step Size
% Noise: babble
% Number of epochs: 500

clear all;
clc;

fs = [8000]; % Sampling frequency is given in Hz
speakers = struct('speakerrecording',{'M020701.WAV'});
noises = struct('noisefile',{'babble.wav'});

stepsize = [0.006 0.008 0.01 0.03];

N = [256 256 512 512]; %N = 1024;%1000;%1000;%6000; %220,330, 660, 1200, 2400, 4800, 9600, 19200
L = [64 128 128 256]; %L = 512;%400;%95;%100;%3, 6, 12Numero de taps dos filtros de de-mixagem(ou seja, a ordem dos filtros)

ConfigurationScriptv2(fs, speakers, noises, stepsize, N, L)