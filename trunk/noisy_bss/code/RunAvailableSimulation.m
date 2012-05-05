% All parameters are strings or doubles.
function []=RunAvailableSimulation(numberOfEpochs, speaker, noise, fs, stepsize, N, L, filename, equalizeEnergyToGreaterOne, speakersPath, noisesPath, savingPath)

speaker_struct = struct('speakerrecording', {});
speaker_struct(1).speakerrecording = speaker;

noise_struct = struct('noisefile',{});
noise_struct(1).noisefile = noise;

ConfigurationScriptv3(fs, speaker_struct, noise_struct, stepsize, N, L, equalizeEnergyToGreaterOne, filename, numberOfEpochs, speakersPath, noisesPath, savingPath);
