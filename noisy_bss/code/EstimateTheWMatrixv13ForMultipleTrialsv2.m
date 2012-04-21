%Additional Information:
% Wm(:,:,1)= W11
% Wm(:,:,2)= W12
% Wm(:,:,3)= W21
% Wm(:,:,4)= W22
%Cada matriz Wm(:,:,x) tem dimensao: 2LxD, onde D e encontrado como sendo
%igual a L, isto eh mostrado no artigo e tambem pode ser mostrado atraves
%da teoria de LPC (ver "Digital Processing of Speech", Rabiner)
%
% nums: n??mero de sensores
% numf: n??mero de fontes (Sinais Originais)
%OBS.: A variavel numChannels utilizada em outras funcoes e calculada como
%sendo: nums*numf
%
% y: matriz com 2 linhas e M colunas, onde M ?? o n??mero de amostras dos
% sinais y1 e y2 (y1: sa??da 1 do sistema de demixagem; y2: sa??da 2 do sistema de demixagem )
%
% L: n??mero de taps(coeficientes) dos filtros de de-mixagem
%OBS.: em outras funcoes se utiliza a notacao P, que tem o mesmo
%significado de L.
%
% N: n??mero de amotras de um bloco
%
% D: numero de atrasos(lags) que se deseja que a matriz de covariancias
% tenha. Ou tambem pode ser visto como a dimensao do vetor multidimensional
% da funcao densidade de probabilidade multidimensional que eh utilizada
% para derivarmos a funcao de custo que deve ser otimizada logo abaixo.
%
% numEpocas: numero de epocas para o qual se deseja calcular a estimativa
% para a matriz de de-mixagem W.
%
% Do not forget to use the following function before calling the
% GetOutputSIR() function
% Applying S1 to the Mixing Filter
%   [x11, x21]=SingleInputMultipleOutputMixingFilter(h11,h12,s1);
% Applying S2 to the Mixing Filter
%   [x12, x22]=SingleInputMultipleOutputMixingFilter(h21,h22,s2);
%
% Input Parameters:
% m: Iteration Number
% W: Matrix containing the un-mixing filter coeficients for the m-th iteration
%       W(:,:,1)=W11    W(:,:,2)=W12
%       W(:,:,3)=W21    W(:,:,4)=W22
% X11: Input to mic 1 due to only the original source signal 1
% X12: Input to mic 1 due to only the original source signal 2
% X21: Input to mic 2 due to only the original source signal 1
% X22: Input to mic 2 due to only the original source signal 2
%
% Output Parameters:
% SIR: Signal to interference ratio for both channels
% SIR(1) is the output SIR for the first output
% SIR(2) is the output SIR for the second output
%
% Numeros de interacoes executadas pela funcao: numEpocas
%
%08-07-2008 OBS.: Este arquivo m de inicializa????o, foi modificado para
%suportar a mudan??a do fator de sopreposi????o "a".
%
%19-07-2008 OBS.: A partir de agora o valor do step size ser?? "setado" nad
%fun????o Estimando_a_matriz_de_de_mixagemv5.
%
%19-07-2008 OBS.: O valor t, tempo aproximado para se calcular uma
%intera????o, ?? agora retornado para seu chamador, que em nosso caso, a
%fun????o Estimando_a_matrix_de_de_mixagemv6
%
% 31-08-2008 Foi inserido no c??digo da CrossCorrelationMatrixv2 e da
% AutoCorrelationMatrixv2 a pondera????o necess??ria ao correto
% c??lculo das matrizes de correla????o, uma vez que depois de se calcular as
% multiplica????es necess??rias, deve-se ponderar o resultado com o valor N
%
% 16-01-2010 Esta fun????o foi modificada para que os filtros que formam a
% matriz W sejam normalizados atrav??s da normaliza????o euclideana, i.e.
% sqrt(x1??+x2??+...+xn??)
%
% 09-02-2010 Adicionado normaliza????o dos valores de x1 e x2 ap??s a
% realiza????o da soma x11+x21 e x12+x22
%
% 17-02-2010 Esta nova vers??o, 12, foi criada com o intuito de chamar a
% fun????o BlockNaturalGradientSCv8, a qual testa a aplica????o do metodo PCA ??s
% matrizes Y.
%
% 06-10-2010 - Neste dia encontrei um bug na forma em que o algoritmo trata
% a sobreposi????o, anteriormente t??nhamos, N/a mas o correto ?? L/a, de
% acordo com os artigos estudados. Desta forma, a partir de agora usaremos
% a fun????o BlockNaturalGradientSCv9.

function [W SIR s2where stepsizevar costfunctionvalues averageTimeForEachEpoch y x1 x2 CostFuntionDelta bdiagRyy Ryy outputPowerOverEpochs bestyVectorEver] = EstimateTheWMatrixv13ForMultipleTrialsv2(L,D,N,numChannels,numEpocas,x11,x12,x21,x22,constraint,initializationType,a,stepsize,momentum,overlapping,learningMethod,fid,boldDriverFunction,isChangeMethodAllowed,epochToChangeMethod,newLearningMethod,changeStepSize,newStepSize,isNormalizingYAllowed,isAutomaticChangeOfAlgorithmAllowed,decayingFactor,fs,speaker1,speaker2,savingPath,isRecordingOfOutputWavesAllowed,uMin,uMax,divByN,methodForCrossCorrelation,methodForAutoCorrelation,blockLenght,saveSIR)

OldCostFunctionValue = Inf;
CurrentCostFunctionValue = 0;
u = stepsize;
beta = momentum;
wasLastUpdatePositive = false;
countSubtractions = 0;
averageDelta = 0;
count = 0;
movingAverageDelta = zeros(1,numEpocas);
maxSIR = -Inf;

savingPath=strcat(savingPath,sprintf('%i/FilesWithDifferentSIRs/',fs));

numOfSamples = size(x11,2);
if(overlapping == 1)
    numOfValues = floor((numOfSamples-N)/floor(blockLenght/a)) + 1;
else
    numOfValues = floor((numOfSamples-N)/blockLenght) + 1;
end

outputPowerOverEpochs = zeros(2,numOfValues,numEpocas);
CostFuntionDelta = zeros(1,numEpocas);
costfunctionvalues = zeros(1,numEpocas);
stepsizevar = zeros(1,numEpocas);

averageTimeForEachEpoch = 0;

SIR = zeros(1,2,numEpocas+1);
s2where = zeros(2,numEpocas+1);

deltaWOld = ones(2*L,D,numChannels);

%-------------------------------------------------------------------
%                 TESTING THE W MATRIX INITIALIZATION
%
%-------------------------------------------------------------------

    fprintf(1,'Size of W before init.\n');
    fprintf(1,'L: %d\n',L);
    fprintf(1,'N: %d\n',N);

%---------------------- 1 -----------------------
% Inicializa a matriz W com filtros da forma [1 0 0 0 .....] (Impulso Unit?rio)
W = InitialiseWMatrix(L,D,numChannels,initializationType);

fprintf(1,'Size of W after init iterations.\n');
fprintf(1,'size(W,1): %d\n',size(W,1));
fprintf(1,'size(W,2): %d\n',size(W,2));

%---------------------- 2 -----------------------
% Inicializa a matriz W com filtros que s??o o inverso da resposta ao
% impulso de um canal normal de voz, onde esta resposta se assemelha a uma
% exponencial decrescente.
alpha = 1;
%W = InitialiseWMatrixWithDecayingExponential(L,D,numChannels,alpha,betaForW);

%---------------------- 3 -----------------------
% Incializa a matriz W com filtros que s??o o inverso de uma resposta
% impulsiva exponecial, todos os filtros, W11, W12, W21 e W22 s??o
% inicializados com o mesmo filtro inverso encontrado nesta func??ao.
%W = InitialiseAllWMatricesWithDecayingExponential(L,D,numChannels,alpha,betaForW);

%---------------------- 4 -----------------------
% Inicializa a matriz W com filtros que s??o o inverso dos respectivos
% filtros reais de mixagem, ou seja, os filtros que foram utilizados para
% se misturar os sinais.
%W = InitialiseWMatrixWithRealInverseMixingFilters(L,D,numChannels);

%---------------------- 5 -----------------------
% Inicializa a matriz W com a matrix W encontrada durante a separa????o que
% utiliza a exponencial decrescente como fator de aprendizagem. O ??ltimo
% valor de W, ou seja, o valor que resultou em uma separa??ao com SIR maior
% do que 35dB ?? usado como valor inicial da matrix W.
%load W.mat

Waux = zeros(2*L,D,numChannels);
Want = zeros(2*L,D,numChannels);
Want = W; % Equivale a fazer W(-1) = W

%-------------------------------------------------------------------
%                       TESTING WHITENNING
% Faz o branqueamento (whitening) das componentes do sinal de sa??da do sistema de mixagem, whitening quer dizer que:  E[y1y2]=E[y2y1]=0 e E[y1y1]=E[y2y2]=1
% Realiza o "whitening" do sinal de entrada do sistema de de-mixagem
%[x11 x12 x21 x22 Zcorr Scross EigenVectors EigenValues] = Whiteningv4(x11, x12, x21, x22);
%-------------------------------------------------------------------

% Calculo da SIR inicial, ou seja, a SIR obtida com a matriz W inicial.
[SIR(:,:,1) s2where(:,1)] = GetOutputSIRv2(W, x11, x21, x12, x22);

%Print initial SIR
sir = 10*log10((SIR(1,1,1) + SIR(1,2,1))/2);
fprintf(1,'Initial SIR: %f\n\n\n',sir);

%Save initital SIR
if(saveSIR)
    fwrite(fid, sir, 'float');
end

% Valores das sa??das dos filtros de mixagem (h11, h12, h21, h22)
x1 = x11 + x21;
x2 = x12 + x22;

% %Added on Tuesday, February 9th of 2010
% x1 = x1./(max(sqrt((x1.^2))));
% x2 = x2./(max(sqrt((x2.^2))));

% OBS.: Quando se utilizou a normaliza????o de x1 e x2 se obteve alguns bons
% resultados, mas em alguns dos testes onde se teve bons resultados, como foi o caso do Momentum term, n??o
% houve converg??ncia, o algotimo alcan??ou um bom valor de SIR(38.6592) mas
% n??o convergiu, come??ando a decair depois de algums ??pocas
% Normalizing x1 and x2
%[x1,x2] = NormalizingX(x1, x2);

% % Whitening the X vector
% x = [x1;x2];
% [x Zcorr Scross EigenVectors EigenValues] = Whiteningv5(x);
% x1 = x(1,:);
% x2 = x(2,:);

% % Added on the 17th day on the February of 2010
% % It applies the PCA method to the input data, that one which was received by
% % the microphones.
% x = [x1;x2];
% [z] = ApplyPCA2InputData(x, L);
% clear x1 x2;
% x1 = z(1,:);
% x2 = z(2,:);

%Calculo do valor de y
[y1 y2] = MultipleInputMultipleOutputUnMixingFilter(W, x1, x2);
y = [y1;y2];

for i=1:1:numEpocas

    fprintf(1,'Epoch number: %d\n',i);
    fprintf(1,'Calculating the Natural Gradient.... \n');
    
    % Start the measurement of the time the algorithm takes to finish one
    % epoch.
    tic
    
    % Added to allow the use of other learning method and stepsize during
    % the training.
    if(isChangeMethodAllowed && (i == (epochToChangeMethod+1)))
        learningMethod = newLearningMethod;
        fprintf(1,'Changing learning method: %s\n', learningMethod);
        if(changeStepSize)
            u = newStepSize;
            stepsize = newStepSize;
            fprintf(1,'Changing  Step Size: %e\n', stepsize);
        end
    end
        
    %-----------------------------------------------
    %******* Normalizando os vetores de sa??da ******
    %-----------------------------------------------
    if(isNormalizingYAllowed)
        y = NormalizingY(y);
    end
    %-----------------------------------------------

    %-----------------------------------------------
    %********** Branqueamento do sinal y ***********
    %-----------------------------------------------
    %      [y Zcorr Scross EigenVectors EigenValues] = Whiteningv5(y);
    %-----------------------------------------------

    %-----------------------------------------------
    %******** C??lculo do Gradiente Natural *********
    %-----------------------------------------------
    [deltaW CurrentCostFunctionValue bdiagRyy Ryy outputPowerOverIterations] = BlockNaturalGradientSCv11(y,W,L,D,N,numChannels,constraint,a,overlapping,divByN,methodForCrossCorrelation,methodForAutoCorrelation,blockLenght);
    %-----------------------------------------------
    
    outputPowerOverEpochs(:,:,i) = outputPowerOverIterations(:,:);
    
    CostFuntionDelta(i) = CurrentCostFunctionValue/(CurrentCostFunctionValue - OldCostFunctionValue);
    
    if(i>1 && isAutomaticChangeOfAlgorithmAllowed)
        [averageDelta movingAverageDelta count] = DecideWhenToChangeTheAlgorithm(CurrentCostFunctionValue, OldCostFunctionValue, i, averageDelta, movingAverageDelta, count);
        if(count == 5)
            fprintf(1,'Changing to %s algorithm at epoch %d\n',newLearningMethod,i);
            learningMethod = newLearningMethod;
            if(changeStepSize)
                u = newStepSize;
                stepsize = newStepSize;
                fprintf(1,'Changing  Step Size: %e\n', stepsize);
            end
        end
    end
    
    if((isChangeMethodAllowed || isAutomaticChangeOfAlgorithmAllowed) && (~strcmp(lower(learningMethod),'momentumterm')))
        Want = W;
    end

    %------------------------------------------------
    %******** Par??metros de Aprendizagem ************
    %------------------------------------------------
    useMTerm = false;
    switch lower(learningMethod)
        
        %---------1. Exponencial Learning Rate------------
        case 'exponentialdecay'
            u = stepsize*exp(-((i-1)^2)/(numEpocas*decayingFactor));
            fprintf(1,'Learning Method: Exponential Decay\n');
            
        %-----------------2. Annealing--------------------
        case 'annealing'
            u = stepsize/(1 + (i/10));
            fprintf(1,'Learning Method: Annealing\n');
            
        %------------------3. BoldDriver------------------
        case {'bolddriver','bolddriver+momentum'}
            %[u] = BoldDriver(CurrentCostFunctionValue, OldCostFunctionValue, u);
            %[u wasLastUpdatePositive] = BoldDriverv2(CurrentCostFunctionValue, OldCostFunctionValue, u, wasLastUpdatePositive);
            %[u wasLastUpdatePositive countSubtractions] = BoldDriverv3(CurrentCostFunctionValue, OldCostFunctionValue, u, wasLastUpdatePositive, stepsize, countSubtractions);
            %[u wasLastUpdatePositive countSubtractions] = BoldDriverv4(CurrentCostFunctionValue, OldCostFunctionValue, u, wasLastUpdatePositive, 0.002, countSubtractions);
            %[u wasLastUpdatePositive countSubtractions] = BoldDriverv5(CurrentCostFunctionValue, OldCostFunctionValue, u, wasLastUpdatePositive, stepsize, countSubtractions);
            %[u wasLastUpdatePositive countSubtractions] = BoldDriverv6(CurrentCostFunctionValue, OldCostFunctionValue, u, wasLastUpdatePositive, stepsize, countSubtractions);
            %[u wasLastUpdatePositive countSubtractions] = BoldDriverv7(CurrentCostFunctionValue, OldCostFunctionValue, u, wasLastUpdatePositive, stepsize, countSubtractions);
            %[u wasLastUpdatePositive countSubtractions] = BoldDriverv8(CurrentCostFunctionValue, OldCostFunctionValue, u, wasLastUpdatePositive, stepsize, countSubtractions);
            
            if(strcmp(boldDriverFunction,'bolddriverv10'))
                [u wasLastUpdatePositive countSubtractions] = BoldDriverv10(CurrentCostFunctionValue, OldCostFunctionValue, u, wasLastUpdatePositive, stepsize, countSubtractions, uMin, uMax);
            elseif(strcmp(boldDriverFunction,'bolddriverv11'))
                [u wasLastUpdatePositive countSubtractions] = BoldDriverv11(CurrentCostFunctionValue, OldCostFunctionValue, u, wasLastUpdatePositive, stepsize, countSubtractions, uMin, uMax);
            elseif(strcmp(boldDriverFunction,'bolddriverv12'))
                [u wasLastUpdatePositive countSubtractions] = BoldDriverv12(CurrentCostFunctionValue, OldCostFunctionValue, u, wasLastUpdatePositive, stepsize, countSubtractions);
            elseif(strcmp(boldDriverFunction,'bolddriverv13'))
                [u wasLastUpdatePositive countSubtractions] = BoldDriverv13(CurrentCostFunctionValue, OldCostFunctionValue, u, wasLastUpdatePositive, stepsize, countSubtractions);
            elseif(strcmp(boldDriverFunction,'bolddriverv14'))
                [u wasLastUpdatePositive countSubtractions] = BoldDriverv14(CurrentCostFunctionValue, OldCostFunctionValue, u, wasLastUpdatePositive, stepsize, countSubtractions);
            elseif(strcmp(boldDriverFunction,'bolddriverv15'))
                [u wasLastUpdatePositive countSubtractions] = BoldDriverv15(CurrentCostFunctionValue, OldCostFunctionValue, u, wasLastUpdatePositive, stepsize, countSubtractions, uMin, uMax);
            elseif(strcmp(boldDriverFunction,'bolddriverv16'))
                [u wasLastUpdatePositive countSubtractions] = BoldDriverv16(CurrentCostFunctionValue, OldCostFunctionValue, u, wasLastUpdatePositive, stepsize, countSubtractions);
            elseif(strcmp(boldDriverFunction,'bolddriverv17'))
                [u wasLastUpdatePositive countSubtractions] = BoldDriverv17(CurrentCostFunctionValue, OldCostFunctionValue, u, wasLastUpdatePositive, stepsize, countSubtractions);
            else
                error('Wrong Bold Driver Function!!!!!!!!!!');
            end
            
            % When usigng the combined methods we need to enter this if in
            % order to enable the momentum term.
            if(strcmpi(learningMethod,'bolddriver+momentum'))
                useMTerm = true;
                Waux = W;
                momentumTerm = beta*(W - Want);
                fprintf(1,'Learning Method: Momentum Term + Bold Driver: %e - BoldDriver Function: %s\n', u, boldDriverFunction);
            else
                fprintf(1,'Learning Method: Bold Driver: %e - Function: %s\n', u, boldDriverFunction);
            end
            
        %-------4. Utilizando o termo "Momentum"----------
        case 'momentumterm'
            useMTerm = true;
            Waux = W;
            momentumTerm = beta*(W - Want);
            fprintf(1,'Learning Method: Momentum Term\n');
            
        %---------------5. Step Size fixo-----------------
        case 'fixedstepsize'
            fprintf(1,'Learning Method: Fixed Step Size\n');
            
        %---------------6. Angle between gradient directions in consecutive iterations-----------------
        case 'anglebetweendirections'
            angleBetweenGradientDirectionsInConsecutiveIterationsv2(deltaW,deltaWOld,L);
            
        otherwise
            error('Wrong Learning Method!!!!!!!!!!');
    end
 
    %----------Atualizando a eq. Normal------------
    W = W - u*deltaW;
    %------------------------------------------------
    
    %If the momentum term is used then the term is added to W.
    if(useMTerm)
        W = W + momentumTerm;
        Want = Waux;
    end

    OldCostFunctionValue = CurrentCostFunctionValue;
    costfunctionvalues(i) = CurrentCostFunctionValue;
    stepsizevar(i) = u;
    
    deltaWOld = deltaW;

    %----------------Calculo da SIR--------------------
    %Calculo da SIR para cada uma das epocas(interacoes)
    fprintf(1,'Calculating the Output SIR.... \n');
    [SIR(:,:,i+1) s2where(:,i+1)] = GetOutputSIRv2(W, x11, x21, x12, x22);
    
    sir = 10*log10((SIR(1,1,i+1) + SIR(1,2,i+1))/2);
    fprintf(1,'Output SIR for epoch[%d]: %f\n\n\n',i,sir);
    
    %Save Output SIR
    if(saveSIR)
        fwrite(fid, sir, 'float');
    end
    %-------------Calculo da SIR - END-----------------

    %---------------Calculo do novo y------------------
    %Calculo da saida y para cada uma das intera????es
    fprintf(1,'Calculating new output values.... \n');
    
    [y1, y2] = MultipleInputMultipleOutputUnMixingFilter(W, x1, x2);
    y = [y1;y2];
    %------------Calculo do novo y - END---------------
    
    %------------ Saving the best y vector ever ---------------
    if(sir > maxSIR)
        maxSIR = sir;
        bestyVectorEver=y;
    end
    %------------ Saving the best y vector ever ---------------
    
    %------Saving audio files with different SIRs------
    if(isRecordingOfOutputWavesAllowed)
        sirAtChannel1 = 10*log10(SIR(1,1,i+1));
        sirAtChannel2 = 10*log10(SIR(1,2,i+1));

        outputAtChannel1 = y1./(max(sqrt((y1.^2))));
        outputAtChannel2 = y2./(max(sqrt((y2.^2))));

        if(s2where(1,i+1) == 1)
            filename1 = sprintf('%s_SIR_%1.3fdB_LM_%s_Fs_%iHz_step_%1.3f_N_%i_L_%i_mom_%1.2f_epoch_%i.wav',speaker1,sirAtChannel1,learningMethod,fs,stepsize,N,L,momentum,i);
            filename1 = strcat(savingPath,filename1);
            wavwrite(outputAtChannel1,fs,filename1);
        elseif(s2where(1,i+1) == 2)
            filename1 = sprintf('%s_SIR_%1.3fdB_LM_%s_Fs_%iHz_step_%1.3f_N_%i_L_%i_mom_%1.2f_epoch_%i.wav',speaker1,sirAtChannel2,learningMethod,fs,stepsize,N,L,momentum,i);
            filename1 = strcat(savingPath,filename1);
            wavwrite(outputAtChannel2,fs,filename1);
        else
            error('Wrong Output Channel Number!!!!!!!!!!');
        end

        if(s2where(2,i+1) == 2)
            filename2 = sprintf('%s_SIR_%1.3fdB_LM_%s_Fs_%iHz_step_%1.3f_N_%i_L_%i_mom_%1.2f_epoch_%i.wav',speaker2,sirAtChannel2,learningMethod,fs,stepsize,N,L,momentum,i);
            filename2 = strcat(savingPath,filename2);
            wavwrite(outputAtChannel2,fs,filename2);
        elseif(s2where(2,i+1) == 1)
            filename2 = sprintf('%s_SIR_%1.3fdB_LM_%s_Fs_%iHz_step_%1.3f_N_%i_L_%i_mom_%1.2f_epoch_%i.wav',speaker2,sirAtChannel1,learningMethod,fs,stepsize,N,L,momentum,i);
            filename2 = strcat(savingPath,filename2);
            wavwrite(outputAtChannel1,fs,filename2);
        else
            error('Wrong Output Channel Number!!!!!!!!!!');
        end
    end
    %--------------------------------------------------
    
    t = toc;
    fprintf(1,'Elapsed time for NaturalGradientSC is %d seconds. \n',t);
    fprintf(1,'\n\n');
    averageTimeForEachEpoch = averageTimeForEachEpoch + t;

end

averageTimeForEachEpoch = averageTimeForEachEpoch/numEpocas;
