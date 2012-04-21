function []=CalculateApproximatedAndRealBinomialValues(n,iterations)


%-------------------------

for i=1:1:iterations

    x = round(rand(1,n));

    y(i) = sum(x);

end

[nn,xout] = hist(y,n);
p_n_k_approx_binomial = nn(k)/sum(nn);

%-------------------------

p_n_k_real_binomial = (factorial(n)/(factorial(k)*factorial(n-k)))*(p^k)*((1-p)^(n-k));