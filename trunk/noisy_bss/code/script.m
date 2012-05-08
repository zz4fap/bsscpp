%------- Assess the separation between aurora's noise data base and Ynoguti's speaker data base --------
%------- This script retrives availbale simulations from a SQL server and run them until there is no more available simulations ------

clear all;
clc;

%----- user-defined values ------
equalizeEnergyToGreaterOne = true;

user = char(java.lang.System.getProperty('user.name'));
if(strcmp('zz4fap',user)==1)
    % Location of the speech and noise files
    speakersPath = '/Users/zz4fap/Desktop/BSS+ASR/data_base/speech/todos/treino/wav/';
    noisesPath = '/Users/zz4fap/Desktop/BSS+ASR/data_base/noises/wav/short/';

    if(equalizeEnergyToGreaterOne)
        savingPath = '/Users/zz4fap/Desktop/';
        %savingPath = '/Users/zz4fap/Desktop/BSS+ASR/simulations/EqualizedEnergy/';
    else
        savingPath = '/Users/zz4fap/Desktop/BSS+ASR/simulations/EnergyNotEqualized/';
    end
elseif(strcmp('ynoguti',user)==1)
    % Location of the speech and noise files
    speakersPath = 'c:\\users\\ynoguti\\documents\\matlab\\ynoguti\\data_base\\speech\\todos\\treino\\wav\\';
    noisesPath = 'c:\\users\\ynoguti\\documents\\matlab\\ynoguti\\data_base\\noises\\wav\\short\\';

    if(equalizeEnergyToGreaterOne)
        savingPath = 'c:\\users\\ynoguti\\documents\\matlab\\ynoguti\\simulations\\EqualizedEnergy\\';
    else
        savingPath = 'c:\\users\ynoguti\\documents\\matlab\\ynoguti\\simulations\\EnergyNotEqualized\\';
    end
else
    error('You must define some paths before running the simulations!!!!');
end

%----- pre-defined values (DO NOT CHANGE THE VALUES BELOW) ------
filename = 'SimulationsInformationForRecordings.txt';

fs = [8000]; % Sampling frequency is given in Hz

table = 'simulations';
server = 'server2';
where = 'status=0';

AVAILABLE_SIMULATIONS_FILE = 'available_simulations.txt';
FINISHED_SIMULATIONS_FILE = 'finished_simulations.txt';
RESULTS_FILE = 'results.txt';

%-------- ALGORITHM --------
bsscpp_info('Simulation has started.');
while 1
    % Check if the file available_simulations.txt exists
    fid = fopen(AVAILABLE_SIMULATIONS_FILE, 'rb');
    if(fid==-1)
        %Get available simulations from SQL Server. OBS.: '*' can be substituted by 'all_columns' in case something goes wrong with *.
        cmd = sprintf('java -jar bsscpp_dbaccess.jar %s retrieve %s all_columns %s %d',server, table, where, 1);
        [status, result] = system(cmd);
        if(status==0)
            bsscpp_info('No more available simulations. They are either finished or running.');
            break;
        elseif(status<0)
            if(status==-2)
                bsscpp_info('Server is down... Excution has stopped!');
            else
                bsscpp_info('Error retrieving available simulations. Excution has stopped!');
            end
            error(result);
            sendEmailThroughGmail('zz4fap@gmail.com','Error',result);
        end
        %tweet status
        msg = sprintf('%d simulations will be executed.', status);
        bsscpp_info(msg);
    end

    % Use the number of finished simulations to figure out the remaining ones.
    numberOfFinishedSimulations = GetNumberOfFinishedSimulations(FINISHED_SIMULATIONS_FILE);
    notRunSimulations = GetNotRunSimulations(AVAILABLE_SIMULATIONS_FILE, numberOfFinishedSimulations);

    if(length(notRunSimulations)==0)
        bsscpp_info('No more available simulations. Update status in the server, delete files and try to get new simulations.');
        %update status in the SQL database with finished simuations.
        cmd = sprintf('java -jar bsscpp_dbaccess.jar %s update_status_of_file %s %d',server, table, 2);
        [status, result] = system(cmd);
        if(status<0)
            bsscpp_info('Error updating simulations status!');
            sendEmailThroughGmail('zz4fap@gmail.com','Error',result);
        end
        %Add Resutlts from results.txt to database.
        cmd = sprintf('java -jar bsscpp_dbaccess.jar %s add_results',server);
        [status, result] = system(cmd);
        if(status<0)
            msg = 'Error adding results to data base!';
            bsscpp_info(msg);
            msg = sprintf('%s\n%s',msg,result);
            sendEmailThroughGmail('zz4fap@gmail.com','Error',msg);
        end
        %Delete both files.
        cmd = sprintf('rm -rf %s %s %s',AVAILABLE_SIMULATIONS_FILE,FINISHED_SIMULATIONS_FILE, RESULTS_FILE);
        system(cmd);
    else
        for i=1:1:length(notRunSimulations)
            counter = notRunSimulations(i).simulation_number;
            speaker = notRunSimulations(i).wav;
            noise = notRunSimulations(i).noise;
            stepsize = notRunSimulations(i).stepsize;
            N = notRunSimulations(i).N;
            L = notRunSimulations(i).L;
            numberOfEpochs = notRunSimulations(i).epochs;
            [averagetime SIR_final durationForStr SIR_inicial]=RunAvailableSimulation(numberOfEpochs, speaker, noise, fs, stepsize, N, L, filename, equalizeEnergyToGreaterOne, speakersPath, noisesPath, savingPath);
            %Update finished_simulations file.
            fid = fopen(FINISHED_SIMULATIONS_FILE,'ab');
            fprintf(fid,'simulation_number=%d,speaker=%s,noise=%s,stepsize=%e,N=%d,L=%d,epochs=%d\n',counter,speaker,noise,stepsize,N,L,numberOfEpochs);
            msg = sprintf('Simulation number: %d has finished running.',counter);
            bsscpp_info(msg);
            %WRITE TO results.txt THE SIMULATIONS RESULTS: simulations number, averagetime SIR_final durationForStr SIR_inicial
            fid = fopen(RESULTS_FILE,'ab');
            fprintf(fid,'simulation_number=%d,duration=%1.4f,initial_sir=%1.4f,sir_final=%1.4f,average_time=%3.4f\n',counter,durationForStr,SIR_inicial,SIR_final,averagetime);
        end
    end
end
