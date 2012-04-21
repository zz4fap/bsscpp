% 31-08-2008 Foi inserido ao c??digo a pondera????o necess??rio ao correto
% c??lculo das matrizes de correla????o, uma vez que depois de se calcular as
% multiplica????es necess??rias, deve-se ponderar o resultado com o valor N

% 17-02-2010 A fun????o foi modificada para que se aplique o m??todo PCA ??s
% matrizes Y antes que se calcule a matriz de correla????o.

% 01-10-2010 Adicionado parametro divByN. Se true significa que matrix deve
% ser normalizada por N.

%------------------------------------------------------------------
%                      Implementa????o eficiente
%------------------------------------------------------------------

% %Applying the Regularization factor - Test #4
%Using the Output Power as the Normalizing Matrix and a regularization
%factor to regulate the values when the power of the signal is too low.

function  [CMM] = efficientImplementationForCorrelationMethod(y1,y2,D,N,n,divByN)

y = y1(:,(n+1):(n+N));

ryy = y*y';

CMM = ryy*eye(D,D);

reg = 10*exp(-ryy);

CMM = CMM + reg*eye(D,D);

if(divByN)
    CMM = CMM/N;
end
%------------------------------------------------------------------
