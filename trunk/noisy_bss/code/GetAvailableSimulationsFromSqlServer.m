
% GetAvailableSimulationsFromSqlServer(AVAILABLE_SIMULATIONS_FILE)

function []=GetAvailableSimulationsFromSqlServer(filename)

fid = fopen(filename, 'rb');
if(fid>0)
    
    simulations = struct('wav', {}, 'stepsize', {}, 'N', {}, 'L', {});
    
    while 1
        tline = fgetl(fid);
        if ~ischar(tline),   break,   end

        params = regexp(tline, '\,', 'split');
        if(length(params)~=9)
            error('Wrong number of parameters!!!');
        end

        for i=1:1:9
            value = regexp(char(params(i)), '\=', 'split');
            param = char(value);
            if(strcmp('wav_filename',param)==1)
               simulations(1).wav = param;
            end
             str2num('10')
            
        end
    end
    fclose(fid);
else
    error('Error opening file %s',filename);
end

%[counter, wav, noise, stepsize, N, L, method, epochs, status ] = textread(AVAILABLE_SIMULATIONS_FILE,'%d %s %s %f %d %d %s %d %d', 1)



%simulation_number=1,wav_filename=test.wav,noise_filename=babble.wav,stepsize=0.0040,N=45,L=12,learning_method=fixedstepsize,epochs=5500,status=22