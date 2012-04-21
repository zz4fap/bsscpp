%Conforme desenho da pagina 29, pode-se ver que o sinal s e filtrado por h1
%e h2. Vamos supor que s seja S1 e que h1 seja h11 e h2 seja h12, entao, x1
%sera a convolucao entre s1 e o filtro h11 e x2 a convolucao entre s1 e
%h12, com isto podera-se calcular a Relacao Sinal Interferencia, pois
%teremos separadamente a resposta do sistema de de-mixagem em relacao
%somente ao sinal S1 e depois, aplicando-se S2 teremos a resposta
%correspondente somente ao sinal S2.

function [x1, x2] = SingleInputMultipleOutputMixingFilter(h1, h2, s)

x1 = filter(h1,1,s);
x2 = filter(h2,1,s);
