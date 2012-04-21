function []=StartAndOrResumeSimulations(fs, speakers, noises, filename, N, L, stepsize, NumberOfEpochs, equalizeEnergyToGreaterOne, numberOfRecordings, speakersPath, noisesPath, savingPath)

[result nextN nextL nextStepsize recordingCounter NPos ssPos] = ResumeSimulationv3(filename, N, L, stepsize, equalizeEnergyToGreaterOne, savingPath);

if(result>=0)
    actualStepSize = [nextStepsize];

    speaker = struct('speakerrecording', {});
    speaker(1).speakerrecording=speakers(recordingCounter).speakerrecording;

    simulationPairNumber = result;
    speakerNumber = recordingCounter;

    actualN = [];
    actualL = [];
    for i=NPos:1:length(N)
        actualN = [actualN N(i)];
        actualL = [actualL L(i)];
    end

    if(actualN(1)~=nextN && actualL(1)~=nextL)
        error('Something went wrong!!!');
    end

    fprintf(1,'Next N: %d\n',nextN);
    fprintf(1,'Next L: %d\n',nextL);
    fprintf(1,'Step Size: %1.3f\n',nextStepsize);
    fprintf(1,'recordingCounter: %d\n',recordingCounter);
    fprintf(1,'Recording: %s\n',speaker(1).speakerrecording);

    ConfigurationScriptv4(fs, speaker, noises, actualStepSize, actualN, actualL, equalizeEnergyToGreaterOne, simulationPairNumber, speakerNumber, filename, NumberOfEpochs, speakersPath, noisesPath, savingPath);

    if((ssPos+1)<=length(stepsize))
        auxStepSize = [];
        for i=(ssPos+1):1:length(stepsize)
            auxStepSize = [auxStepSize stepsize(i)]
        end

        ConfigurationScriptv4(fs, speaker, noises, auxStepSize, N, L, equalizeEnergyToGreaterOne, (simulationPairNumber+length(actualN)), speakerNumber, filename, NumberOfEpochs, speakersPath, noisesPath, savingPath);
    end

    if((speakerNumber+1)<=numberOfRecordings)
        speaker = struct('speakerrecording', {});
        j = 1;
        for i=(speakerNumber+1):1:length(speakers)
            speaker(j).speakerrecording=speakers(i).speakerrecording;
            j=j+1;
        end

        ConfigurationScriptv4(fs, speaker, noises, stepsize, N, L, equalizeEnergyToGreaterOne, 0, (speakerNumber+1), filename, NumberOfEpochs, speakersPath, noisesPath, savingPath);
    end

else
    ConfigurationScriptv3(fs, speakers, noises, stepsize, N, L, equalizeEnergyToGreaterOne, filename, NumberOfEpochs, speakersPath, noisesPath, savingPath);
end