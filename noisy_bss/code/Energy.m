% A entrada desta fun????o ?? um vetor linha

function [E]=Energy(signal)

M=size(signal,2);
E=0;
for i=1:1:M
    E=E+(signal(i)^(2));
end