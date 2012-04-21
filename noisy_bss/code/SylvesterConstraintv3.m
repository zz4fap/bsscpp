%SYLVESTER CONSTRAINT

%channels: number of filters
%
%OBS.: A funcao SylvesterConstraintv2 foi modificada para possibilitar que os
%valores L e D possam ser variados independentemente.
%
%29-06-2008 - A fun????oo SylvesterConstraintv2 foi modificada para
%possibilitar que o usu??rio possa escolher entre as restri????es column e
%row.

function [deltaWsc]=SylvesterConstraintv3(Wsc, constraint, L, D, channels)

deltaWsc=zeros(2*L,D,channels);

switch lower(constraint)
    case 'row'
        for i=1:1:channels
            for j=1:1:D
                for k=1:1:L
                    deltaWsc((k+(j-1)),j,i)=Wsc(:,(D-(k-1)),i);
                end
            end
        end
    case 'column'
        for i=1:1:channels
            for j=1:1:D %move as colunas
                for k=1:1:L %move as linhas
                    deltaWsc(k+(j-1),j,i)=Wsc(k,:,i);
                end
            end
        end         
end