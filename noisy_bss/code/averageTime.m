clear all;clc

% total average time given in minutes.
total_average_time = zeros(1,4);
number_of_items = zeros(1,4);

folder_with_saved_data = '/Users/zz4fap/Desktop/BSS+ASR/simulations/EqualizedEnergy/';
command=sprintf('ls %s',folder_with_saved_data)
[status, result] = system(command);

[filename, count, errmsg, nextindex]=sscanf(result, '%s',1);
while(strcmp(filename,'fixedstepsize.dat')==0)

    %fprintf(1,'%s\n',filename);

    filename=strcat(folder_with_saved_data,filename);
    filename2savesir = filename;
    load(filename);

    avg_in_min = (averagetime*numEpocas)/60;
    fprintf(1,'N: %d, L: %d - %1.4f\n',N(j),L(j),avg_in_min);

    NL = N(j)+L(j);
    if(NL == 320)
        %fprintf(1,'NL == 320\n');
        total_average_time(1) = total_average_time(1) + avg_in_min;
        number_of_items(1) = number_of_items(1) + 1;
    elseif(NL == 384)
        total_average_time(2) = total_average_time(2) + avg_in_min;
        number_of_items(2) = number_of_items(2) + 1;
    elseif(NL == 640)
        total_average_time(3) = total_average_time(3) + avg_in_min;
        number_of_items(3) = number_of_items(3) + 1;
    elseif(NL == 768)
        total_average_time(4) = total_average_time(4) + avg_in_min;
        number_of_items(4) = number_of_items(4) + 1;
    else

    end

    %preparing for the next itaration.
    result = result((nextindex):length(result));
    [filename, count, errmsg, nextindex]=sscanf(result, '%s',1);
end

for i=1:1:4
    total_average_time(i) = total_average_time(i)/number_of_items(i);
end