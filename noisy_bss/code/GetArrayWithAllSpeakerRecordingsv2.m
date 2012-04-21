function [speakers]=GetArrayWithAllSpeakerRecordingsv2(path,numberOfRecordings,fs,tester)

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

    if(strcmp(tester,'felipe')==1 && i<=(numberOfRecordings/2))
        speakers(i).speakerrecording = filename;
    elseif(strcmp(tester,'ynoguti')==1 && i>(numberOfRecordings/2))        
        speakers(i-(numberOfRecordings/2)).speakerrecording = filename;
    end

end

% for i=1:1:numberOfRecordings
% fprintf(1,'%s\n',speakers(i).speakerrecording);
%
% end
