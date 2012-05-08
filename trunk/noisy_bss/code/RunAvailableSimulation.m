% All parameters are strings or doubles.
function [averagetime SIR_final durationForStr SIR_inicial]=RunAvailableSimulation(numberOfEpochs, speaker, noise, fs, stepsize, N, L, filename, equalizeEnergyToGreaterOne, speakersPath, noisesPath, savingPath)

speaker_struct = struct('speakerrecording', {});
speaker_struct(1).speakerrecording = speaker;

noise_struct = struct('noisefile',{});
noise_struct(1).noisefile = noise;

[averagetime SIR_final durationForStr SIR_inicial]=ConfigurationScriptv3(fs, speaker_struct, noise_struct, stepsize, N, L, equalizeEnergyToGreaterOne, filename, numberOfEpochs, speakersPath, noisesPath, savingPath);
