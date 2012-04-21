clear all;clc

fs = 8000;

path = '/Users/zz4fap/Desktop/BSS+ASR/data_base/noises/raw/';

cd(path);

[status result]=system('ls *.raw');

if(status~=0)
    error(result);
    exit
end

for i=1:1:5

    [filename, count, errmsg, nextindex]=sscanf(result, '%s',1);
    result = result((nextindex):length(result));

    fid = fopen(filename, 'r');

    A = fread(fid, 'int16', 'ieee-be');
    
    filename = strrep(filename, '.raw', '.wav')

    a = A(:,1)./(max(sqrt((A(:,1).^2))));

    wavwrite(a,fs,filename);

    fclose(fid);

end

cd(curentDir);
