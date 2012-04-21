% Do not forget to use the following function before calling the
% GetSIRforIterationM() function
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
% x11: Input to mic 1 due to only the original source signal 1
% x12: Input to mic 1 due to only the original source signal 2
% x21: Input to mic 2 due to only the original source signal 1
% x22: Input to mic 2 due to only the original source signal 2
%
% Output Parameters:
% SIR: Signal to interference ratio for both channels
% SIR(1) is the output SIR for the first output channel
% SIR(2) is the output SIR for the second output channel
function [SIR s2where] = GetOutputSIRv2(W, x11, x12, x21, x22)

SIR = zeros(1,2);
s2where = zeros(2,1); % Esta vari??vel mostra em qual sa??da os sinais s1 e s2 saem.
                      % As posi????es marcam os sinais originais(s1 e s2), o
                      % conte??do de cada posi????o mostra em que sa??da(y1 ou y2) cada um
                      % dos sinais originais saiu.

% Outputs of the De-mixing system due to only the signal 1
% Y11 = Y1 devido ao sinal 1 (s1).
% Y21 = Y2 devido ao sinal 1 (s1).
[Y11, Y21] = MultipleInputMultipleOutputUnMixingFilter(W,x11,x21);
% Energy of the output signals due to excitation from signal 1
EnergyY11 = Energy(Y11);
EnergyY21 = Energy(Y21);

% Outputs of the De-mixing system due to only the signal 2
% Y12 = Y1 devido ao sinal 2 (s2).
% Y22 = Y2 devido ao sinal 2 (s2).
[Y12, Y22] = MultipleInputMultipleOutputUnMixingFilter(W,x12,x22);
% Energy of the output signals due to excitation from signal 2
EnergyY12 = Energy(Y12);
EnergyY22 = Energy(Y22);

if(EnergyY11 > EnergyY12)
    s2where(1) = 1;%s2where(1): s1 sai em y1
    SIR(1) = EnergyY11/EnergyY12;
else
    s2where(2) = 1;%s2where(2): s2 sai em y1
    SIR(1) = EnergyY12/EnergyY11;
end

if(EnergyY21 > EnergyY22)
    s2where(1) = 2;%s2where(1): s1 sai em y2
    SIR(2) = EnergyY21/EnergyY22;
else
    s2where(2) = 2;%s2where(1): s2 sai em y2
    SIR(2) = EnergyY22/EnergyY21;
end
