% 31-08-2008 Foi inserido ao c??digo a pondera????o necess??rio ao correto
% c??lculo das matrizes de correla????o, uma vez que depois de se calcular as
% multiplica????es necess??rias, deve-se ponderar o resultado com o valor N.

% 17-02-2010 A fun????o foi modificada para que se aplique o m??todo PCA ??s
% matrizes Y antes que se calcule a matriz de correla????o.

% 01-10-2010 Adicionado parametro divByN. Se true significa que matrix deve
% ser normalizada por N.

function  [CMM] = CrossCorrelationMatrixv5(y1, y2, D, N, n, divByN, method)

CMM = zeros(D,D);

switch lower(method)
    case 'correlation'
        CMM = correlationMethod(y1,y2,D,N,n,divByN);
    case 'covariance'
        CMM = covarianceMethod(y1,y2,D,N,n,divByN);
    otherwise
        error('Invalid Method');
end
