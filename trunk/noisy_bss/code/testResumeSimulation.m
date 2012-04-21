clc;clear all;

% filename = 'test2.txt';
% savingPath = '/Users/zz4fap/Desktop/';

filename = 'SimulationsInformation.txt';
equalizeEnergyToGreaterOne = true;

stepsize = [0.002 0.004 0.006 0.008 0.01 0.03];

N = [256 256 512 512]; %N = 1024;%1000;%1000;%6000; %220,330, 660, 1200, 2400, 4800, 9600, 19200
L = [64 128 128 256]; %L = 512;%400;%95;%100;%3, 6, 12Numero de taps dos filtros de de-mixagem(ou seja, a ordem dos filtros)

%[result NLPosition stepsizePosition] = ResumeSimulation(filename,savingPath, N, L, stepsize);

[result nextN nextL nextStepsize recordingCounter] = ResumeSimulationv2(filename, N, L, stepsize, equalizeEnergyToGreaterOne);

fprintf(1,'Next N: %d\n',nextN);
fprintf(1,'Next L: %d\n',nextL);
fprintf(1,'Step Size: %1.3f\n',nextStepsize);
fprintf(1,'recordingCounter: %d\n',recordingCounter);


% a=[1,2,3,4,5,6,7,8,9];
%
% for i=20:1:20
%
%    fprintf(1,'%d\n',(mod(i,4)));
%
%    aux =  ceil(i/4);
%
%    %fprintf(1,'%1.2f\n',aux);
%
%    %fprintf(1,'%d\n',a(aux));
%
% end