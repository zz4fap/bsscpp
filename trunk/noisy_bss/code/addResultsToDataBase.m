function [SIR_final, SIR_inicial, averagetime, speakerRecording, N, L, j, durationForStr, stepsize, k, simulation_number]=addResultsToDataBase(path)

RECORDINGS_FILE = '/Users/zz4fap/Documents/workspace/bsscpp/noisy_bss/code/recordings_';
FINISHED_SIMULATIONS_RESULTS = 'finished_simulations_results.txt';

simulation_counter = 1;

command=sprintf('ls %s',path)
[status, result] = system(command);

[filename, count, errmsg, nextindex]=sscanf(result, '%s',1);
while(strcmp(filename,'')==0)

    if(strfind(filename, 'SimulationsInformationForRecordings_')==1)
        break;
    end

    filename=strcat(path,filename);
    load(filename);

    SIR_final = 10*log10((OutputSIR(1,1,(numEpocas+1)) + OutputSIR(1,2,(numEpocas+1)))/2);
    SIR_inicial = 10*log10((OutputSIR(1,1,1) + OutputSIR(1,2,1))/2);

    fileNumber = -1;
    for fileCounter=1:1:10
        file = sprintf('%s%d',RECORDINGS_FILE,fileCounter);
        fid = fopen(file,'rb');
        counter = 1;
        while(1)
            aux = fscanf(fid,'%s', 1);
            if((feof(fid)==1) || (isequal(aux,[])==1))
                break;
            end
            if(strcmp(aux,speakerRecording)==1)
                fileNumber = fileCounter;
                break;
            end

            counter = counter +1;
        end
        if(fileNumber~=-1)
            break;
        end
    end

    if(fileNumber==-1)
        error('File was not found!!!');
    end

    simulation_number = ((fileNumber-1)*2880 + ((counter-1)*24 + ((k-1)*4+j)));
    fprintf(1,'%s - %d - file: %d - simulation: %d - counter: %d\n',aux,counter,fileCounter,simulation_counter,simulation_number);

    %WRITE TO results.txt THE SIMULATIONS RESULTS: simulations number, averagetime SIR_final durationForStr SIR_inicial
    fid = fopen(FINISHED_SIMULATIONS_RESULTS,'ab');
    fprintf(fid,'simulation_number=%d,duration=%1.4f,initial_sir=%1.4f,sir_final=%1.4f,average_time=%3.4f\n',simulation_number,durationForStr,SIR_inicial,SIR_final,averagetime);

    simulation_counter = simulation_counter+1;

    result = result((nextindex):length(result));
    [filename, count, errmsg, nextindex]=sscanf(result, '%s',1);
end

%Add Resutlts from results.txt to database.
cmd = sprintf('java -jar bsscpp_dbaccess.jar %s add_results',server);
[status, result] = system(cmd);
if(status<0)
    msg = 'Error adding results to data base!';
    disp(msg);
    error(result);
end