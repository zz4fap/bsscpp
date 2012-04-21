%Get the coeficients of a FIR filter from a matrix that exibits a Sylvester
%Structure
%Where P is the order of the FIR Filter and 
%W is the un-mixing system matrix
%W(:,:,1)=W11 with dimension 2L*L
%W(:,:,2)=W12 with dimension 2L*L
%W(:,:,3)=W21 with dimension 2L*L
%W(:,:,4)=W22 with dimension 2L*L

function [filter]=GetFilter(W,P)

filter=zeros(1,P);
for j=1:1:P
    filter(j)=W(P,(P-(j-1)));
end