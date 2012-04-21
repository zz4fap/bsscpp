function [speakers]=GetSpeakersListFromFile(filename)

%clear all;clc;

%filename='recordings_10';

speakers = struct('speakerrecording', {});

% Open the file for reading.
fid = fopen(filename, 'rb');

i=1;

while(feof(fid)~=1)

    file=fscanf(fid,'%s',1);
    
    fprintf(1,'file #%i: %s\n',i,file);

    if(strcmp(file,'')~=1)
        speakers(i).speakerrecording=file;
    end
    i=i+1;
    
end