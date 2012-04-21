%numChannels: Representa o numero de filtros com o quais se pode modelar o ambiente
%em que os locutores se encontram (Por exemplo, utilizaremos em nossas simulacoes numChannels =4)
%
%A matriz de saida desta funcao eh: W, que e definida da seguinte forma:
%W(:,:,1)=W11 com dimensao 2L*L
%W(:,:,2)=W12 com dimensao 2L*L
%W(:,:,3)=W21 com dimensao 2L*L
%W(:,:,4)=W22 com dimensao 2L*L

function [W]=InitialiseWMatrix(L,D,numChannels,initializationType)

switch lower(initializationType)
    case 'shifted'
        % Inicializa somente as matrizes W11 e W22 com um filtro passa tudo deslocado de ceil(L/2)amostras(impulso unitario)
        W=zeros(2*L,D,numChannels);
        inc=1;
        for m=1:1:sqrt(numChannels)
            for i=1:1:D
                W((ceil(L/2)+(i-1)),i,inc)=1;
            end
            inc=inc+sqrt(numChannels)+1;
        end
    case 'non-shifted'
        % Inicializa somente as matrizes W11 e W22 com um filtro passa tudo(impulso unitario)
        W=zeros(2*L,D,numChannels);
        inc=1;
        for m=1:1:sqrt(numChannels)
            for i=1:1:D
                W(i,i,inc)=1;
            end
            inc=inc+sqrt(numChannels)+1;
        end
end

% %Inicializa todas as matizes com um filtro passa tudo(impulso unitario)
% W=zeros(2*L,D,numChannels);
% for m=1:1:numChannels
%     for i=1:1:D
%         W(i,i,m)=1;
%     end 
% end

% %Inicializa todos os coeficientes das matrizes com 1
% for m=1:1:numChannels
%     for i=1:1:2*L
%         for j=1:1:D
%             W(i,j,m)=1;
%         end 
%     end
% end