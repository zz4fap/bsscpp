%SYLVESTER CONSTRAINT

%channels: number of filters
%
%OBS.: A funcao SylvesterConstraintv2 foi modificada para possibilitar que os
%valores L e D possam ser variados independentemente.
%
%29-06-2008 - A fun????oo SylvesterConstraintv2 foi modificada para
%possibilitar que o usu??rio possa escolher entre as restri????es column e
%row.

function [deltaWsc]=SylvesterConstraintv4(Wsc, constraint, L, D, channels)

if(strcmpi(constraint,'avg'))
    W = zeros(2*L,1,channels);
    for i=1:1:channels
        for k=0:-1:-(L-1)
            W((abs(k)+1),1,i) = sum(diag(Wsc(:,:,i),k))/D;
        end
    end
    deltaWsc = SylvesterConstraintv3(W,'column',L,D,channels);
else
    deltaWsc = SylvesterConstraintv3(Wsc,constraint,L,D,channels);
end
