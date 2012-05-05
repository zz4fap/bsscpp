function [notRunsimulations]=GetNotRunSimulations(filename, numberOfFinishedSimulations)

notRunsimulations = struct('simulation_number', {}, 'wav', {}, 'noise', {}, 'stepsize', {}, 'N', {}, 'L', {}, 'epochs', {});

fid = fopen(filename, 'rb');
if(fid>0)

    simulationNumber = 0;

    while 1
        tline = fgetl(fid);
        if ~ischar(tline),   break,   end

        params = regexp(tline, '\,', 'split');
        if(length(params)~=9)
            error('Wrong number of parameters!!!');
        end

        simulationNumber = simulationNumber+1;

        if(simulationNumber>numberOfFinishedSimulations)
            for i=1:1:9
                value = regexp(char(params(i)), '\=', 'split');
                paramName = char(value(1));
                if(strcmp('simulation_number',paramName)==1)
                    notRunsimulations(simulationNumber).simulation_number = str2num(char(value(2)));
                elseif(strcmp('wav_filename',paramName)==1)
                    notRunsimulations(simulationNumber).wav = char(value(2));
                elseif(strcmp('noise_filename',paramName)==1)  
                    notRunsimulations(simulationNumber).noise = char(value(2));
                elseif(strcmp('stepsize',paramName)==1)
                    notRunsimulations(simulationNumber).stepsize = str2num(char(value(2)));
                elseif(strcmp('N',paramName)==1)
                    notRunsimulations(simulationNumber).N = str2num(char(value(2)));
                elseif(strcmp('L',paramName)==1)
                    notRunsimulations(simulationNumber).L = str2num(char(value(2)));
                elseif(strcmp('epochs',paramName)==1)
                    notRunsimulations(simulationNumber).epochs = str2num(char(value(2)));
                end
            end
        end
    end
    fclose(fid);
else
    error('Error opening file %s',filename);
end
