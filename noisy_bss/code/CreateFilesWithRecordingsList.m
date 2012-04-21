function []=CreateFilesWithRecordingsList(numberOfRecordings,numberOfFilesToCreate,fs,speakersPath)

speakers = GetArrayWithAllSpeakerRecordings(speakersPath,numberOfRecordings,fs);

for i=1:1:numberOfFilesToCreate
    
    filename = sprintf('recordings_%i',i);
    % Open the file for writing.
    fid = fopen(filename, 'wb');
    
    fprintf(1,'--------- %s ---------\n',filename);
    
    for j=1+(numberOfRecordings/numberOfFilesToCreate)*(i-1):1:i*(numberOfRecordings/numberOfFilesToCreate)
        
        fprintf(fid,'%s\n',speakers(j).speakerrecording);
        
        fprintf(1,'%i - %s\n',j,speakers(j).speakerrecording);

    end
    
    fprintf(1,'----------------------\n');
    fclose(fid);
    
end