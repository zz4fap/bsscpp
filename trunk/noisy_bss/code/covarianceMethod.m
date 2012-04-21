%--------------------------------------------------------------------
%              Implementa????o do m??todo da Covari??ncia
%--------------------------------------------------------------------

function  [CMM] = covarianceMethod(y1,y2,D,N,n,divByN)

CMM = zeros(D,D);

Y1 = zeros(D,N); %Este ?? o Y transposto, utilizado para calcular Y'Y
Y2 = zeros(D,N);

y1 = [zeros(1,(D-1)) y1];
y2 = [zeros(1,(D-1)) y2];

for i = 0:1:(D-1) % Forma todas as linhas da Matriz Y_p(m)
    Y1((i+1),:) = y1(:,(n+D-i):(n+D+N-1-i));
    Y2((i+1),:) = y2(:,(n+D-i):(n+D+N-1-i));
end

CMM = Y1*Y2';

if(divByN)
    CMM = CMM/N;
end
