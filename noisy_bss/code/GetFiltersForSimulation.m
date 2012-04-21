function [h11 h12 h21 h22]=GetFiltersForSimulation(filterset)

switch lower(filterset)
    case 'filterset2'
        h11 = [0.1226 0.2094 0.6222];
        h12 = [0.5924 0.9666 0.5148];
        h21 = [0.9411 0.2310 0.5591];
        h22 = [0.0840 0.8245 0.1754];
    case 'filterset5'
        h11 = [0.9134    0.6324    0.3975    0.1034    0.0547];
        h12 = [0.4854    0.2800    0.1419    0.0542    0.0117];
        h21 = [0.6555    0.5171    0.2706    0.1031    0.0277];
        h22 = [0.9502    0.7655    0.4387    0.3816    0.0344];
    case 'filterset6'
        h11 = [0.5924 0.9666 0.6222];
        h12 = [0.1226 0.2094 0.5148];
        h21 = [0.0840 0.2310 0.1754];
        h22 = [0.9411 0.8245 0.5591];
    case 'imgmethod'
        src1 = [1 1.2 1];
        src2 = [1 0.8 1];
        rm = [3 2 2.5];
        mic1 = [1.4 1.2 1];
        mic2 = [1.4 0.8 1];
        r = 0.3;
        n = 3;
        [h11 h12 h21 h22] = GenerateFilterWithImageMethod(fs(1), src1, src2, rm, mic1, mic2, r, n);
    otherwise
        error('Wrong filter set used!!!!!!!!!!');
end
