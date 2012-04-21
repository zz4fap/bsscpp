% A matriz de entrada desta funcao ??: W, que e definida da seguinte forma:
% W(:,:,1)=W11 com dimensao 2L*L
% W(:,:,2)=W12 com dimensao 2L*L
% W(:,:,3)=W21 com dimensao 2L*L
% W(:,:,4)=W22 com dimensao 2L*L
%
% x1 e x2 sao os sinais de saida do sistema de mistura, ou seja, sao as
% saidas da funcao: SingleInputMultipleOutputMixingFilter


function [y1, y2] = MultipleInputMultipleOutputUnMixingFilter(W, x1, x2)

% Obt??m o n??mero de taps dos filtros de de-mixagem
P = size(W(:,:,1),2);

W11 = GetFilter(W(:,:,1),P);
W21 = GetFilter(W(:,:,3),P);
y1 = filter(W11,1,x1) + filter(W21,1,x2);

W12 = GetFilter(W(:,:,2),P);
W22 = GetFilter(W(:,:,4),P);
y2 = filter(W12,1,x1) + filter(W22,1,x2);