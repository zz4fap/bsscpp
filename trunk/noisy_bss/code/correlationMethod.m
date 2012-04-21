%--------------------------------------------------------------------
%              Implementação do método da Correlação
%--------------------------------------------------------------------

function [CMM] = correlationMethod(y1,y2,D,N,n,divByN)

CMM = zeros(D,D);

Y1 = zeros(D,N+D-1); %Este é o Y transposto, utilizado para calcular Y'Y
Y2 = zeros(D,N+D-1);

for i = 1:1:D % Forma todas as linhas da Matriz Y_p(m)
    Y1(i,:) = [zeros(1,i-1) y1(:,(n+1):(n+N)) zeros(1,D-i)];
    Y2(i,:) = [zeros(1,i-1) y2(:,(n+1):(n+N)) zeros(1,D-i)];
end

CMM = Y1*Y2';

if(divByN)
    CMM = CMM/N;
end
