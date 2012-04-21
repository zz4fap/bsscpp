%NATURAL GRADIENT AND SYLVESTER CONSTRAINT
%
% Additional Information:
% Wm(:,:,1)= W11
% Wm(:,:,2)= W12
% Wm(:,:,3)= W21
% Wm(:,:,4)= W22
%Cada matriz Wm(:,:,x) tem dimensao: 2LxD, onde D e encontrado como sendo
%igual a L, isto eh mostrado no artigo e tambem pode ser mostrado atraves
%da teoria de LPC (ver "Digital Processing of Speech", Rabiner)
%
% nums: n??mero de sensores
% numf: n??mero de fontes (Sinais Originais)
%OBS.: A variavel numChannels utilizada em outras funcoes e calculada como
%sendo: nums*numf, sendo assim a partir de agora iremos substituir
%nums*numf por numChannels.
%
% y: matriz com 2 linhas e M colunas, onde M ?? o n??mero de amostras dos
% sinais y1 e y2 (y1: sa??da 1 do sistema de demixagem; y2: sa??da 2 do sistema de de-mixagem )
% y=[y1;y2]
%
% P: n??mero de taps(coeficientes) dos filtros de de-mixagem
%
% N: n??mero de amotras de um bloco
%
% u: Fator de aprendizagem (step size parameter). Geralmente o valor de u e
% constante, mas pode-se utilizar uma equacao que da passos (steps) maiores
% quando o gradiente esta indo na direcao do minimo, ou valores menores,
% quando o gradiente se direciona ao maximo da funcao, assim,
% desascelerando sua caminhada em direcao contraria a direcao correta, ou
% seja, a direcao do minimo, que no caso otimo eh o minimo global da
% superficie dada pela funcao de custo.
%
% M: numero de blocos no qual o sinal (y) foi dividido, note que aqui os
% blocos se deslocam de apenas 1 amostra a cada interacao da variavel i
% abaixo. Posteriormente pode-se modificar o algoritmo para que utiliza
% blocos nao sobrepostos ou com outras sobreposicoes.
%--------------------------------------------------------------------------
%
%Esta funcao calcula o coeficiente de atualizacao(gradiente natural) da
%equacao de atualizacao dos coeficientes dos filtros de de-mixagem para
%apenas uma interacao.
%
%--------------------------------------------------------------------------
%
% Numeros de interacoes executadas pela funcao: 2*2*M
%
%OBS.: A funcao NaturalGradientSCv2 foi modificada para possibilitar que os
%valores L e D possam ser variados independentemente.
%
%08-07-2008 OBS.: A fun??????o BlockNaturalGradientSC foi implementada para
%possibilitar que o algoritmo ande em blocos de N amostras, com uma
%sobreposi??????o dos blocos determinada pelo valor de "a", esta vari???vel pode
%assumir a seguinte faixa de valores 1 <= a <= N. Esta fun??????o ??? baseada na
%fun????o NaturalGradientSCv5.
%
%20-07-2008 Modificou-se esta fun????o para que a mesma utilize a fun????o
%CorrelationMatrixv4 ao inv??s da CorrelationMatrixv3, que calcula a matriz
%de correla????o de forma diferente ?? descrita no m??todo da covari??ncia.
%
%10-08-2008 Esta fun????o incorpora duas novas fun????es para o c??lculo das
%matrizes de correla????o, utiliza-se estas fun????es na esperan??a de que o
%tempo levado para o c??lculo das matrizes diminua.
%
% 31-08-2008 Foi inserido no c??digo da CrossCorrelationMatrixv2 e da 
% AutoCorrelationMatrixv2 a pondera????o necess??ria ao correto
% c??lculo das matrizes de correla????o, uma vez que depois de se calcular as
% multiplica????es necess??rias, deve-se ponderar o resultado com o valor N
%
% 06-08-2008 Inseriu-se a variavel overlapping para controlar a
% sobreposi????o de blocos, se igual a 1 o overlapping esta habilitado se 0,
% n??o est??.
%
% 07-08-2008 Modificou-se o algoritmo para que n (o inst??nte de tempo)
% fosse incrementado continuamente com valores N/a.
%
% 27-02-2010 A fun????o agora retorna o valor da potencia de saida dos canais 
% para todas as itera????es. 
%
% 06-10-2010 - Neste dia encontrei um bug na forma em que o algoritmo trata
% a sobreposi????o, anteriormente t??nhamos, N/a mas o correto ?? L/a, de
% acordo com os artigos estudados.
%
% 10-10-2010 - Adicionado a op????o 'avg' no case de restri????es, ela calcula
% toda a matriz W n??o fazendo uma multiplica????o matriz/vetor e sim
% matriz/matriz.

function [deltaW CostFunctionValue bdiagRyy Ryy outputPowerOverIterations] = BlockNaturalGradientSCv11(y,Wm,L,D,N,numChannels,constraint,a,overlapping,divByN,methodForCrossCorrelation,methodForAutoCorrelation,blockLenght)

M = size(y(1,:),2);
channels = numChannels;
CostFunctionValue = 0;
detr22 = 0;
detr11 = 0;
sumCount = 0;

if(overlapping == 1)
    numOfValues = floor((M-N)/floor(blockLenght/a)) + 1;
else
    numOfValues = floor((M-N)/blockLenght) + 1;
end
outputPowerOverIterations = zeros(2,numOfValues);

switch lower(constraint)
    case 'row'
        Wsc = zeros(1,D,channels);
    case 'column'
        Wsc = zeros(2*L,1,channels);
    case 'avg'
        Wsc = zeros(2*L,D,channels);
    otherwise
        error('Invalid Constraint');
end

n = 0;
while((n+N-1) <= (M-1))   
    
    %--------------------------------------------------------------%
    R12 = CrossCorrelationMatrixv5(y(1,:), y(2,:), D, N, n, divByN, methodForCrossCorrelation);
    R22 = AutoCorrelationMatrixv5(y(2,:), y(2,:), D, N, n, divByN, methodForAutoCorrelation);
    if(det(R22) == 0)
        R22 = R22 + (sqrt(var(diag(R22)))*eye(D,D));
        fprintf(1,'det(R22)==0: Trying to fix it by adding variance to the matrix\n');
    end
    if (det(R22) ~= 0) %If the determinant of R continues being equal to zero this test fails.
        R = R12*(R22^(-1));
        switch lower(constraint)
            case 'row'
                Wsc(:,:,2) = Wsc(:,:,2) + Wm(L,:,1)*R;
                Wsc(:,:,4) = Wsc(:,:,4) + Wm(L,:,3)*R;
            case 'column'
                Wsc(:,:,2) = Wsc(:,:,2) + Wm(:,:,1)*R(:,1);
                Wsc(:,:,4) = Wsc(:,:,4) + Wm(:,:,3)*R(:,1);
            case 'avg'
                Wsc(:,:,2) = Wsc(:,:,2) + Wm(:,:,1)*R;
                Wsc(:,:,4) = Wsc(:,:,4) + Wm(:,:,3)*R;
        end
    else
        detr22 = detr22+1;
        fprintf(1,'det(R22)==0: %d vezes\n',detr22);
    end
    %--------------------------------------------------------------%

    %--------------------------------------------------------------%
    R11 = AutoCorrelationMatrixv5(y(1,:), y(1,:), D, N, n, divByN, methodForAutoCorrelation);
    R21 = R12';
    if(det(R11) == 0) %If the determinant of R continues being equal to zero this test fails.
        R11 = R11 + (sqrt(var(diag(R11)))*eye(D,D));
        fprintf(1,'det(R11)==0: Trying to fix it by adding variance to the matrix\n');
    end
    if (det(R11) ~= 0)
        R = R21*(R11^(-1));
        switch lower(constraint)
            case 'row'
                Wsc(:,:,1) = Wsc(:,:,1) + Wm(L,:,2)*R;
                Wsc(:,:,3) = Wsc(:,:,3) + Wm(L,:,4)*R;
            case 'column'
                Wsc(:,:,1) = Wsc(:,:,1) + Wm(:,:,2)*R(:,1);
                Wsc(:,:,3) = Wsc(:,:,3) + Wm(:,:,4)*R(:,1);
            case 'avg'
                Wsc(:,:,1) = Wsc(:,:,1) + Wm(:,:,2)*R;
                Wsc(:,:,3) = Wsc(:,:,3) + Wm(:,:,4)*R;                
        end        
    else
        detr11 = detr11+1;
        fprintf(1,'det(R11)==0: %d vezes\n',detr11);
    end
    %--------------------------------------------------------------%

    getL2Norm = true;
    Ryy = [R11 R12;R21 R22];
    bdiagRyy = [R11 zeros(D,D);zeros(D,D) R22];
    
    if(~getL2Norm)
        regularizationfactor = 1e-323;
        
        if(det(Ryy) == Inf || det(Ryy) == -Inf || det(bdiagRyy) == Inf || det(bdiagRyy) == -Inf)
            
            maxRyy = abs(max(max(Ryy)));
            maxbdiagRyy = abs(max(max(bdiagRyy)));

            if(maxRyy >= maxbdiagRyy)
                maxValue = maxRyy;
            else
                maxValue = maxbdiagRyy;
            end
            
        else
            maxValue = 1;
        end
        
        detRyy = det(Ryy/maxValue);
        detbdiagRyy = det(bdiagRyy/maxValue);

        if(detRyy == 0 ||detbdiagRyy == 0)
            CostFunctionValue = CostFunctionValue + (log10((detbdiagRyy + regularizationfactor)) - log10((detRyy + regularizationfactor)));
        elseif(detRyy < 0 || detbdiagRyy < 0)
            CostFunctionValue = CostFunctionValue + (log10(abs(detbdiagRyy)) - log10(abs(detRyy)));
        else
            CostFunctionValue = CostFunctionValue + (log10((detbdiagRyy)) - log10((detRyy)));
        end
        
        %logSum = log10(det(R11/maxValue)) + log10(det(R22/maxValue));
        %fprintf(1,'log10(abs(detbdiagRyy)): %e - log10((detRyy)): %e\n',log10(abs(detbdiagRyy)),log10((detRyy)));

        % Verifica a exist??ncia de valores imagin??rios nos valores da fun????o de custo
        if(detRyy < 0 || detbdiagRyy < 0)
            fprintf(1,'An imaginary value seems to have been found in the CostFuntionValue!!! det(Ryy) or det(bdiag(Ryy)) < 0.\n');
        end
    else
        CostFunctionValue = CostFunctionValue + sqrt(sum(sum(R12.^2))); % L2-Norm of R12
    end
    
    % Incrementa a vari??vel sum para que possa ser utilizada para ponderar os pesos.
    sumCount = sumCount + 1;
    
    % It stores the Output Sinal Power of the signals over all epochs.
    outputPowerOverIterations(:,sumCount) = [R11(1,1);R22(1,1)]; 
    
    % Incrementa o inst??nte de tempo em passos de L/a
    n = n + (floor(blockLenght/a))*overlapping + blockLenght*(1 - overlapping);
end

CostFunctionValue = CostFunctionValue/sumCount;

for i=1:1:channels
    %
    %Wsc(:,:,i)=((2*u)/M)*Wsc(:,:,i); %Antes multiplicava-se o gradiente
    %por 2 pois os sinais considerados eram ditos complexos e como visto da
    %teoria de variaveis complexas, deve-se multiplicar o gradiente por 2
    %para eliminarmos o valor 0.5 vindo da derivada de uma varivel complexa
    %
    Wsc(:,:,i)=(1/sumCount)*Wsc(:,:,i);
end

% Wsc = VectorNormv2(Wsc, L, D, numChannels, constraint);

deltaW = SylvesterConstraintv4(Wsc,constraint,L,D,channels);

fprintf(1,'Number of iterations: %d\n',sumCount);
fprintf(1,'Number of zero det(R11): %d\n',detr11);
fprintf(1,'Number of zero det(R22): %d\n',detr22);

