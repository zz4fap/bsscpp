function [result NextNL NextStepsize] = ResumeSimulation(filename, N, L, stepsize, equalizeEnergyToGreaterOne)

if(equalizeEnergyToGreaterOne)
    savingPath = '/Users/zz4fap/Desktop/BSS+ASR/simulations/EqualizedEnergy/';
else
    savingPath = '/Users/zz4fap/Desktop/BSS+ASR/simulations/EnergyNotEqualized/';
end

filename = strcat(savingPath,filename);
% Open the file for reading.
fid = fopen(filename, 'rb');

if(fid < 0)
    %error('File does not exist....');
    result = -1;
else
    while(1)
        aux = fscanf(fid,'%d',1);
        if(feof(fid)~=1)
            number = aux;
        else
            break;
        end
    end
    result = number;
    fprintf(1,'result: %d\n',result);

    NLPosition = mod(result,length(N));
    if(NLPosition==0)
        NLPosition=length(N);
    end
    stepsizePosition = ceil(result/length(N));
    result = 0;

    fprintf(1,'N/L Position: %d\n',NLPosition);
    fprintf(1,'Step Size Position: %d\n',stepsizePosition);
end
