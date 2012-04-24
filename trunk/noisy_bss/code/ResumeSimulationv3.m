function [result nextN nextL nextStepsize recordingCounter NPos ssPos] = ResumeSimulationv3(filename, N, L, stepsize, equalizeEnergyToGreaterOne, savingPath)

filename = strcat(savingPath,filename);
% Open the file for reading.
fid = fopen(filename, 'rb');

if(fid < 0)
    result = -1; nextN = -1; nextL = -1; nextStepsize = -1; recordingCounter = -1; NPos = -1; ssPos = -1;
else
    while(1)
        aux = fscanf(fid,'%d %d', 2);
        if(isequal(aux,[])==1)
            result = -1; nextN = -1; nextL = -1; nextStepsize = -1; recordingCounter = -1; NPos = -1; ssPos = -1;
            return;
        end
        
        if(feof(fid)~=1)
            number = aux;
        else
            break;
        end
    end
    recordingCounter=number(1);    
    result = number(2);

    NLPosition = mod(result,length(N));
    if(NLPosition==0)
        NLPosition=length(N);
    end
    stepsizePosition = ceil(result/length(N));
    
    if(stepsizePosition > length(stepsize))
       error('stepsizePosition > length(stepsize)'); 
    end
    
    if(result==(length(N)*length(stepsize)))
        nextN = N(1);
        nextL = L(1);
        NPos = 1;
        nextStepsize = stepsize(1);
        ssPos = 1;
        recordingCounter=recordingCounter+1;
        result = 0;
    else
        if(NLPosition+1>length(N))
            nextN = N(1);
            nextL = L(1);
            NPos = 1;
            nextStepsize = stepsize(ceil(stepsizePosition+1));
            ssPos = ceil(stepsizePosition+1);
        else
            nextN = N(NLPosition+1);
            nextL = L(NLPosition+1);
            NPos = NLPosition+1;
            nextStepsize = stepsize(stepsizePosition);
            ssPos = stepsizePosition;
        end
    end
%     result = 0;
end
