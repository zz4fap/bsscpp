function [result nextN nextL nextStepsize recordingCounter] = ResumeSimulationv2(filename, N, L, stepsize, equalizeEnergyToGreaterOne)

if(equalizeEnergyToGreaterOne)
    savingPath = '/Users/zz4fap/Desktop/BSS+ASR/simulations/EqualizedEnergy/';
else
    savingPath = '/Users/zz4fap/Desktop/BSS+ASR/simulations/EnergyNotEqualized/';
end

filename = strcat(savingPath,filename);
% Open the file for reading.
fid = fopen(filename, 'rb');

if(fid < 0)
    result = -1;
else
    while(1)
        aux = fscanf(fid,'%d %d', 2);
        if(feof(fid)~=1)
            number = aux;
        else
            break;
        end
    end
    recordingCounter=number(1);    
    result = number(2)

    NLPosition = mod(result,length(N));
    if(NLPosition==0)
        NLPosition=length(N);
    end
    stepsizePosition = ceil(result/length(N));
    
    if(result==(length(N)*length(stepsize)))
        nextN = N(1);
        nextL = L(1);
        nextStepsize = stepsize(1);
        recordingCounter=recordingCounter+1;
    else
        if(NLPosition+1>length(N))
            nextN = N(1);
            nextL = L(1);
            nextStepsize = stepsize(ceil(stepsizePosition+1));
        else
            nextN = N(NLPosition+1);
            nextL = L(NLPosition+1);
            nextStepsize = stepsize(stepsizePosition);
        end
    end
%     result = 0;
end
