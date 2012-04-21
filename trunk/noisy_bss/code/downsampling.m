function [] = downsampling()

clc

curentDir = pwd;

path = '/Users/zz4fap/Desktop/BSS+ASR/data_base/novabase/todos/treino/wav/11025/';
writingPath = '/Users/zz4fap/Desktop/BSS+ASR/data_base/novabase/todos/treino/wav/8000/';

cd(path);

[status result]=system('ls *.WAV');

if(status~=0)
    error(result);
    exit
end

cd(curentDir);

p = 7256;
q = 10000;
downsampled_Fs = 8000;

for i=1:1:1200

    [filename, count, errmsg, nextindex]=sscanf(result, '%s',1);
    result = result((nextindex):length(result));

    [y, Fs, nbits] = wavread(strcat(path,filename));
    if(Fs~=11025)
        error('Sampling frequency different from 11025!!!');
        exit;
    end

    y = y';


    downsampled_y = resample(y,p,q);

    downsampled_L = length(downsampled_y);
    downsampled_y = downsampled_y';

    wavwrite(downsampled_y,downsampled_Fs,strcat(writingPath,filename));
end
