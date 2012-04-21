filename = '/Users/zz4fap/Desktop/SimulationsInformationForRecordings_8.txt';
MaxCount=447;

% Open the file for writing.
fid = fopen(filename, 'w+');

count=1;
for speakerNumber=1:1:120
    for simulationPairNumber=1:1:24
        fprintf(fid,'%i %i\n',speakerNumber,simulationPairNumber);
        count=count+1;
        if(count==(MaxCount+1))
            break;
        end
    end
    if(count==(MaxCount+1))
        break;
    end
end

