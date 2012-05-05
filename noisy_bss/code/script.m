%------- Assess the separation between aurora's noise data base and Ynoguti's speaker data base --------
%------- This script retrives availbale simulations from a SQL server and run them until there is no more available simulations ------

clear all;
clc;

%----- user-defined values ------
% Location of the speech and noise files
speakersPath = '/Users/zz4fap/Desktop/BSS+ASR/data_base/speech/todos/treino/wav/';
noisesPath = '/Users/zz4fap/Desktop/BSS+ASR/data_base/noises/wav/short/';

equalizeEnergyToGreaterOne = true;

if(equalizeEnergyToGreaterOne)
    savingPath = '/Users/zz4fap/Desktop/BSS+ASR/simulations/EqualizedEnergy/';
else
    savingPath = '/Users/zz4fap/Desktop/BSS+ASR/simulations/EnergyNotEqualized/';
end

savingPath = '/Users/zz4fap/Desktop/';

%----- pre-defined values ------
filename = 'SimulationsInformationForRecordings.txt';

fs = [8000]; % Sampling frequency is given in Hz

server = 'server2';
table = 'tests_table';
where = 'status=0';

AVAILABLE_SIMULATIONS_FILE = 'available_simulations.txt';
FINISHED_SIMULATIONS_FILE = 'finished_simulations.txt';

%-------- ALGORITHM --------
bsscpp_info('Simulation has started.')
while 1
    % Check if the file available_simulations.txt exists
    fid = fopen(AVAILABLE_SIMULATIONS_FILE, 'rb');
    if(fid==-1)
        %Get available simulations from SQL Server.
        cmd = sprintf('java -jar bsscpp_dbaccess.jar %s retrieve %s \\* %s %d',server, table, where, 1);
        [status, result] = system(cmd);
        if(status==0)
            bsscpp_info('There are no available simulations. All simulations are either finished or running.');
            break;
        elseif(status<0)
            bsscpp_info('There was an error when retrieving available simulations. Excution has stopped!!!');
            error(result);
        end
        %tweet status
        msg = sprintf('%d simulations will be executed.', status);
        bsscpp_info(msg)
    end

    % Use the number of finished simulations to figure out the remaining ones.
    numberOfFinishedSimulations = GetNumberOfFinishedSimulations(FINISHED_SIMULATIONS_FILE);
    notRunSimulations = GetNotRunSimulations(AVAILABLE_SIMULATIONS_FILE, numberOfFinishedSimulations);

    if(length(notRunSimulations)==0)
        bsscpp_info('There are no more available simulations. Update status in the SQL server, delete files and try to get new simulations.');
        %update status in the SQL database with finished simuations.
        cmd = sprintf('java -jar bsscpp_dbaccess.jar %s update_status_of_file %s %d',server, table, 2);
        [status, result] = system(cmd);
        %Delete both files.
        cmd = sprintf('rm -rf %s %s',AVAILABLE_SIMULATIONS_FILE,FINISHED_SIMULATIONS_FILE);
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
            RunAvailableSimulation(numberOfEpochs, speaker, noise, fs, stepsize, N, L, filename, equalizeEnergyToGreaterOne, speakersPath, noisesPath, savingPath);
            %Update finished_simulations file.
            fid = fopen(FINISHED_SIMULATIONS_FILE,'ab');
            fprintf(fid,'simulation_number=%d,speaker=%s,noise=%s,stepsize=%e,N=%d,L=%d,epochs=%d\n',counter,speaker,noise,stepsize,N,L,numberOfEpochs);
            msg = sprintf('Simulation number: %d has finished running.',counter);
            bsscpp_info(msg);
        end
    end
end
