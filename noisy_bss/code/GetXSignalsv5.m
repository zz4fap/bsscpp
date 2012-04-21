function [x11 x12 x21 x22 bits x11Energy x12Energy x21Energy x22Energy MaxEigValue EigenValues] = GetXSignalsv5(SpeakersRecordingsPath, NoisesRecordingsPath, fs, duration, speakerRecording, noiseRecording, equalizeVectorsToGreaterLentg, equalizeEnergyToGreaterOne, h11, h12, h21, h22)

debug = true;

% 1 - Leitura e equaliza??o quanto ao n?mero de amostras dos arquivos de audio.
[s1 s2 bits] = GeneratingSpeechSignalsv3(duration, fs, speakerRecording, noiseRecording, SpeakersRecordingsPath, NoisesRecordingsPath, equalizeVectorsToGreaterLentg);

% Equalizando energia dos sinais originais
if(equalizeEnergyToGreaterOne)
    [s1 s2] = EqualizeSourceSignalsEnergy(s1,s2);
end

% 2 - Cria sinais x11, x12, x21 e x22
% Obtendo a entrada do sistema de de-mixagem
[a11 a12] = SingleInputMultipleOutputMixingFilter(h11,h12,s1);
[a21 a22] = SingleInputMultipleOutputMixingFilter(h21,h22,s2);

% 3 - Verifica quais s?oo realmente os sinais com maior e menor energia em cada canal de entrada do sistema de demixagem
[x11 x12 x21 x22 x11Energy x12Energy x21Energy x22Energy MaxEigValue EigenValues] = getRealXSignalsv2(a11, a12, a21, a22);

% 4 - Verifica se os sinais respeitam os padr??es naturais.
% x11Energy > x21Energy e x22Energy > x12Energy

if(debug)
    x11Energy
    x21Energy
    x22Energy
    x12Energy
end

% if(x11Energy < x21Energy || x22Energy < x12Energy)
%     error('There was an error: Either x11Energy < x21Energy or x22Energy < x12Energy');
% end
