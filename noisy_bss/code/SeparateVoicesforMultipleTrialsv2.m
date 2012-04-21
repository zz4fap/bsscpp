function [] = SeparateVoicesforMultipleTrialsv2(numEpocas, duration, constraint, fs, learningMethod, boldDriverFunction, momentum, stepsize, L, N, isL2Norm, isChangeMethodAllowed, epochToChangeMethod, newLearningMethod, changeStepSize, newStepSize, isNormalizingYAllowed, isAutomaticChangeOfAlgorithmAllowed, decayingFactor, initializationType, speakers, noises, startFrom,isRecordingOfOutputWavesAllowed, equalizeVectorsToGreaterLentg, equalizeEnergyToGreaterOne, h11, h12, h21, h22,filterSet,uMin,uMax,divByN,overlapping,a,methodForCrossCorrelation,methodForAutoCorrelation,blockLenght, speakersPath, noisesPath, savingPath, filenameForSimulationsInformation)

fprintf(1,'Recording path of speakers: %s\n', speakersPath);
fprintf(1,'Recording path of noises: %s\n', noisesPath);

%Fixed Variables----------------------------
saveVariables = true; % Set this variable to true if you want to save all the variables to a .mat file.
numChannels = 4;
saveSIR = false;
averageTime = 0;
%-------------------------------------------

%Changing Variables-----------------------------
D = L; %D = L; %P ?? igual a L

filename = strcat(savingPath,learningMethod);
filename = strcat(filename,'.dat');

% Open the file for writing.
fid = fopen(filename, 'wb');

simulationPairNumber = 0;

% Open the file for writing.
fid2 = fopen(strcat(savingPath,filenameForSimulationsInformation),'ab');
%-------------------------------------------

for speakersCounter=1:1:length(speakers)

    for noisesCounter=1:1:length(noises)

        for l=1:1:length(constraint)

            for i=1:1:length(fs)

                speakerRecording = speakers(speakersCounter).speakerrecording;

                noiseRecording = noises(noisesCounter).noisefile;

                [x11 x12 x21 x22 bits x11Energy x12Energy x21Energy x22Energy MaxEigValue EigenValues] = GetXSignalsv5(speakersPath, noisesPath, fs, duration, speakerRecording, noiseRecording, equalizeVectorsToGreaterLentg, equalizeEnergyToGreaterOne, h11, h12, h21, h22);

                for d=1:1:length(momentum)

                    for k=1:1:length(stepsize)

                        for j=1:1:length(L)

                            for olf=1:1:length(a)

                                % Calculo da duracao
                                durationForStrInitial = length(x11)/fs(i);

                                fprintf(1,'learning method: %s, fs: %i, L: %i, N: %i, epochs: %i, step size: %e, momentum: %1.2f, constraint: %s, duration: %1.4f\n\n\n',learningMethod,fs(i),L(j),N(j),numEpocas,stepsize(k),momentum(d),constraint(l).constraintName,durationForStrInitial);
                                if(isChangeMethodAllowed)
                                    fprintf(1,'Changing of the learning method is allowed!!!!\n');
                                end

                                % writing actual parameters to a file so that I can resume executing it from the point it stopped running.
                                fprintf(fid,'%s_%s_%s_%i_%1.2f_%e_%i_%i_%i\n',speakerRecording,noiseRecording,constraint(l).constraintName,fs(i),momentum(d),stepsize(k),N(j),L(j),a(olf));

                                % Estimativa da matriz W e calculo da SIR para cada epoca.
                                [W OutputSIR s2where stepsizevar costfunctionvalues averagetime y x1 x2 CostFuntionDelta bdiagRyy Ryy outputPowerOverEpochs bestyVectorEver] = EstimateTheWMatrixv13ForMultipleTrialsv2(L(j),D(j),N(j),numChannels,numEpocas,x11,x12,x21,x22,constraint(l).constraintName,initializationType,a(olf),stepsize(k),momentum(d),overlapping,learningMethod,fid,boldDriverFunction,isChangeMethodAllowed,epochToChangeMethod,newLearningMethod,changeStepSize,newStepSize,isNormalizingYAllowed,isAutomaticChangeOfAlgorithmAllowed,decayingFactor,fs(i),speakerRecording,noiseRecording,savingPath,isRecordingOfOutputWavesAllowed,uMin,uMax,divByN,methodForCrossCorrelation,methodForAutoCorrelation,blockLenght(j),saveSIR);

                                % Numero total de interacoes executadas pela simulacao:
                                M = size(x11,2);
                                CargaComputacional = numEpocas*2*2*M*L(j)*L(j)*N(j);

                                % It counts the iteration number so that it's possible to resume simulation from where it stopped
                                simulationPairNumber=simulationPairNumber+1;
                                fprintf(fid2,'%i %i\n',speakersCounter,simulationPairNumber);

                                % Calcula da duracao
                                durationForStr = length(x1)/fs(i);

                                if(saveVariables)
                                    % Save all the output variables
                                    str1 = sprintf(savingPath);
                                    str1 = strcat(str1,speakerRecording);
                                    str1 = strcat(str1,'_');
                                    str1 = strcat(str1,noiseRecording);
                                    str1 = strcat(str1,'_');

                                    if(divByN)
                                        str1 = strcat(str1,'NormRyyByN_');
                                    else
                                        str1 = strcat(str1,'NotNormRyyByN_');
                                    end
                                    str2 = sprintf('N_%d_L_%d_StepSize_%1.5f_',N(j),L(j),stepsize(k));
                                    str1 = strcat(str1,str2);
                                    str2 = sprintf('fs_%d_',fs(i));
                                    str1 = strcat(str1,str2);
                                    str1 = strcat(str1,learningMethod);
                                    str2 = sprintf('_epochs_%d_momentum_%1.2f',numEpocas,momentum(d));
                                    if(strcmp(learningMethod,'bolddriver') || strcmp(learningMethod,'bolddriver+momentum'))
                                        str2 =  strcat(str2,'_');
                                        str2 =  strcat(str2,boldDriverFunction);
                                        if(isL2Norm)
                                            str2 =  strcat(str2,'AndL2Norm');
                                        end
                                    end
                                    if(isChangeMethodAllowed)
                                        str3 = sprintf('_After%dChangeTo_%s',epochToChangeMethod,newLearningMethod);
                                        str2 =  strcat(str2,str3);
                                    end
                                    if(strcmp(constraint(l).constraintName,'column'))
                                        str2 =  strcat(str2,'_');
                                        str2 =  strcat(str2,constraint(l).constraintName);
                                    end
                                    if(strcmp(constraint(l).constraintName,'avg'))
                                        str2 =  strcat(str2,'_');
                                        str2 =  strcat(str2,constraint(l).constraintName);
                                    end
                                    if(isNormalizingYAllowed)
                                        str2 =  strcat(str2,'_');
                                        str2 =  strcat(str2,'NormY');
                                    end
                                    if(isAutomaticChangeOfAlgorithmAllowed)
                                        str2 =  strcat(str2,'_');
                                        str2 =  strcat(str2,'AutoChange');
                                        if(changeStepSize)
                                            str2 =  strcat(str2,'_');
                                            str2 =  strcat(str2,'ChangeStepSize');
                                        else
                                            str2 =  strcat(str2,'_');
                                            str2 =  strcat(str2,'DoNotChangeStepSize');
                                        end
                                    end
                                    if(strcmp(initializationType,'shifted'))
                                        str2 =  strcat(str2,'_');
                                        str2 =  strcat(str2,'ShiftedInit');
                                    end
                                    if(strcmp(learningMethod,'exponentialdecay'))
                                        str3 = sprintf('_DecayingFactor%d',decayingFactor);
                                        str2 = strcat(str2,str3);
                                    end
                                    if(overlapping == 1)
                                        str3 = sprintf('_olf%d',a(olf));
                                        str2 = strcat(str2,str3);
                                    end
                                    str1 = strcat(str1,str2);
                                    str3 = sprintf('_Duration%1.2f',durationForStr);
                                    str1 = strcat(str1,str3);

                                    switch lower(methodForCrossCorrelation)
                                        case 'correlation'
                                            str1 = strcat(str1,'_Corr');
                                        case 'covariance'
                                            str1 = strcat(str1,'_Cov');
                                    end
                                    switch lower(methodForAutoCorrelation)
                                        case 'correlation'
                                            str1 = strcat(str1,'Corr');
                                        case 'covariance'
                                            str1 = strcat(str1,'Cov');
                                        case 'efficientcorrelation' % When this method is used it means that the Output power is used as the Normalizing Auto Correlation Matrix Ryy.
                                            str1 = strcat(str1,'EffCorr');
                                        case 'efficientcovariance'
                                            str1 = strcat(str1,'EffCov');
                                    end

                                    SIR_final = 10*log10((OutputSIR(1,1,(numEpocas+1)) + OutputSIR(1,2,(numEpocas+1)))/2);

                                    str1 = strcat(str1,sprintf('_%1.2fdB',SIR_final));

                                    str1 = strcat(str1,'.mat');

                                    fprintf(1,'Saving file as: %s\n',str1);

                                    save(str1);
                                end

                            end
                        end
                    end
                end
            end
        end
    end
    fprintf(fid,'------------------------------------------------------------------------------\n');
end

%Close it.
fclose(fid);
fclose(fid2);

if(0)

    % % Plota a SIR de saida do sistema de de-mixagem
    for i=1:1:(numEpocas+1)
        SIR(i) = 10*log10(OutputSIR(1,1,i));
    end
    figure(1)
    plot(SIR)
    ylabel('Signal-to-Interference Ratio (SIR) in dB for Channel 1')
    xlabel('Number of iterations')
    set(gca,'XTick',0:10:(numEpocas))
    clear SIR

    for i=1:1:(numEpocas+1)
        SIR(i) = 10*log10(OutputSIR(1,2,i));
    end
    figure(2)
    plot(SIR)
    ylabel('Signal-to-Interference Ratio (SIR) in dB for Channel 2')
    xlabel('Number of iterations')
    set(gca,'XTick',0:10:(numEpocas))
    clear SIR

    % Plota a m???dia da SIR dos dois canais de sa??da
    for i=1:1:(numEpocas+1)
        SIR(i) = 10*log10((OutputSIR(1,1,i) + OutputSIR(1,2,i))/2);
    end
    figure(3)
    plot(SIR)

    ylabel('M??dia da SIR para ambos canais de saida em dB')
    xlabel('N??mero de iteracoes')
    %ylabel('SIR in dB for the average SIR of both Channels ')
    %xlabel('Number of iterations')
    set(gca,'XTick',0:10:(numEpocas))

    fprintf(1,'Average time for each iteration: %i\n',averagetime);
    fprintf(1,'Total time for algorithm execution: %i\n',(averagetime*numEpocas));

    imagValue = 0;
    for i=1:numEpocas
        if(abs(costfunctionvalues(i)) ~= costfunctionvalues(i))
            imagValue = imagValue + 1;
        end
    end
    fprintf(1,'Number of imaginary cost function values: %i\n',imagValue);
    fprintf(1,'Number of non-imaginary cost function values: %i\n',(numEpocas-imagValue));

    sum(s2where(1,:))/size(s2where,2)
    sum(s2where(2,:))/size(s2where,2)

    %save('SIR1',SIR)

end
