function [numberOfFinishedSimulations] = GetNumberOfFinishedSimulations(filename)

numberOfFinishedSimulations = 0;

fid = fopen(filename, 'rb');
if(fid>0)
    while 1
        tline = fgetl(fid);
        if ~ischar(tline),   break,   end
        numberOfFinishedSimulations = numberOfFinishedSimulations + 1;
    end
    fclose(fid);
end