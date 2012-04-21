function [speakers]=GetArrayWithAllSpeakerRecordings(path,numberOfRecordings,fs)

curentDir = pwd;

path=strcat(path,sprintf('%i/',fs));

cd(path);

fprintf(1,'Path were to read wav files from: %s\n',path)

[status result]=system('ls *.WAV');

if(status~=0)
    error(result);
    exit
end

cd(curentDir);

speakers = struct('speakerrecording', {});

for i=1:1:numberOfRecordings

    [filename, count, errmsg, nextindex]=sscanf(result, '%s',1);
    result = result((nextindex):length(result));
    
    speakers(i).speakerrecording = filename;
end

% for i=1:1:numberOfRecordings
% fprintf(1,'%s\n',speakers(i).speakerrecording);
% 
% end