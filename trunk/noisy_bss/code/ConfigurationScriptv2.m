function []=ConfigurationScriptv2(fs, speakers, noises, stepsize, N, L)

% Location of the speech and noise files
speakersPath = '/Users/zz4fap/Desktop/BSS+ASR/data_base/speech/todos/treino/wav/';
noisesPath = '/Users/zz4fap/Desktop/BSS+ASR/data_base/noises/wav/short/';

equalizeEnergyToGreaterOne = true;
if(equalizeEnergyToGreaterOne)
    savingPath = '/Users/zz4fap/Desktop/BSS+ASR/simulations/EqualizedEnergy/';
else
    savingPath = '/Users/zz4fap/Desktop/BSS+ASR/simulations/EnergyNotEqualized/';
end

numEpocas = 500;
duration = 0; % Use 0 for the whole utterance. This value is given is seconds [s].
startFrom = 0;
constraint = struct('constraintName',{'row'}); % (row is the std. value)
initializationType = 'non-shifted'; % 'non-shifted' is used for causal systems. (non-shifted is the std. value)

% Filters
filterset='filterset5';
[h11 h12 h21 h22] = GetFiltersForSimulation(filterset);

learningMethod = 'fixedstepsize';

uMin = 0.0002;
uMax = 0.15;

boldDriverFunction = '';
isL2Norm = true;

momentum = [0.8]; %If you do not want to use the momentum term, set it to 0.

isChangeMethodAllowed = false;
epochToChangeMethod = numEpocas;
newLearningMethod = '';
changeStepSize = false;
newStepSize = 0;

isNormalizingYAllowed = false;

isAutomaticChangeOfAlgorithmAllowed = false;

decayingFactor = 20;

isRecordingOfOutputWavesAllowed = false;

equalizeVectorsToGreaterLentg = false;

divByN = false;

overlapping = 1; %If 1, it means that overlapping is allowed, if 0, overlapping is not allowed
a = [2]; %Overlapping Factor ( 2 is the std. value)

methodForCrossCorrelation = 'covariance';
methodForAutoCorrelation = 'efficientcorrelation';

blockLenght = L;

SeparateVoicesforMultipleTrialsv2(numEpocas, duration, constraint, fs, learningMethod, boldDriverFunction, momentum, stepsize, L, N, isL2Norm, isChangeMethodAllowed, epochToChangeMethod, newLearningMethod, changeStepSize, newStepSize, isNormalizingYAllowed, isAutomaticChangeOfAlgorithmAllowed, decayingFactor, initializationType, speakers, noises, startFrom, isRecordingOfOutputWavesAllowed, equalizeVectorsToGreaterLentg, equalizeEnergyToGreaterOne, h11, h12, h21, h22,filterset,uMin,uMax,divByN,overlapping,a,methodForCrossCorrelation,methodForAutoCorrelation,blockLenght, speakersPath, noisesPath, savingPath);
%--------------------------------------------------
