path = '/Users/zz4fap/Desktop/BSS+ASR/data_base/novabase/todos/treino/wav/';
filename = 'M050909.WAV';

[y, Fs, nbits] = wavread(strcat(path,filename));

y = y';

L = length(y);

NFFT = 2^nextpow2(L); % Next power of 2 from length of y
Y = fft(y,NFFT)/L;
f = Fs/2*linspace(0,1,NFFT/2);

% Plot single-sided amplitude spectrum.
plot(f,2*abs(Y(1:NFFT/2))) 
title('Single-Sided Amplitude Spectrum of y(n)')
xlabel('Frequency (Hz)')
ylabel('|Y(f)|')

p = 7256;
q = 10000;

downsampled_Fs = 8000;
downsampled_y = resample(y,p,q);
downsampled_L = length(downsampled_y);



figure(2)
downsampled_NFFT = 2^nextpow2(downsampled_L); % Next power of 2 from length of y
downsampled_Y = fft(downsampled_y,downsampled_NFFT)/downsampled_L;
downsampled_f = downsampled_Fs/2*linspace(0,1,downsampled_NFFT/2);

% Plot single-sided amplitude spectrum.
plot(downsampled_f,2*abs(downsampled_Y(1:downsampled_NFFT/2))) 
title('Single-Sided Amplitude Spectrum of downsampled_y(n)')
xlabel('Frequency (Hz)')
ylabel('|downsampled_Y(f)|')

sound(y,Fs,nbits)
sound(downsampled_y,downsampled_Fs,nbits)