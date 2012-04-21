function [s1 s2] = EqualizeSourceSignalsEnergy(s1,s2)

minerror = 0.01;

s1OriginalEnergy = 10*log10(Energy(s1));
s2OriginalEnergy = 10*log10(Energy(s2));

fprintf(1,'s1 energy before equalizing: %1.2f\n',s1OriginalEnergy);
fprintf(1,'s2 energy before equalizing: %1.2f\n',s2OriginalEnergy);

if(s1OriginalEnergy > s2OriginalEnergy)
    news2 = s2*sqrt(Energy(s1)/Energy(s2));
    news2Energy = 10*log10(Energy(news2));
    fprintf(1,'New s2 energy: %1.2f\n',news2Energy);
    if(news2Energy > (s1OriginalEnergy+minerror) || news2Energy < (s1OriginalEnergy-minerror))
        fprintf(1,'Error: s2 energy after equalizing: %1.2f\n',news2Energy);
        error('new s2 energy is different from s1 original energy !!!!');
    end
    s2 = news2;
else
    news1 = s1*sqrt(Energy(s2)/Energy(s1));
    news1Energy = 10*log10(Energy(news1));
    fprintf(1,'New s1 energy: %1.2f\n',news1Energy);
    if(news1Energy > (s2OriginalEnergy+minerror) || news1Energy < (s2OriginalEnergy-minerror))
        fprintf(1,'Error: s1 energy after equalizing: %1.2f\n',news1Energy);
        error('new s1 energy is different from s2 original energy !!!!');
    end
    s1 = news1;
end

s1NewEnergy = 10*log10(Energy(s1));
s2NewEnergy = 10*log10(Energy(s2));

fprintf(1,'\n\ns1 energy after equalizing: %1.2f\n',s1NewEnergy);
fprintf(1,'s2 energy after equalizing: %1.2f\n',s2NewEnergy);
