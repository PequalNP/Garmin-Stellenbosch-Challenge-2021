% load processes .wav
files = dir('*.txt');
for i=1:length(files)
    eval(['load ' files(i).name ' -ascii']);
end
% Transmitted Audio time
x_tr = Transmit_1;       % Data sample points amplitudes
fs_tr = 100000;          % sample frequency/ sample rate
ts_tr = 1/fs_tr;         % time step 
N_tr  = 50001;           % number of smaples
tmax_tr = (N_tr-1)*ts_tr;% max time (duration of audio)
t_tr = 0:ts_tr:tmax_tr;  % time points for each data point

% Transmitted Audio frequency
f_tr= -fs_tr/2:fs_tr/(N_tr-1):fs_tr/2; % frequncy steps 
z_tr= fftshift(fft(x_tr));             % amplitude 


[max_Z_tr, i_tr]= max(z_tr);
F_act_tr = f_tr(i_tr);      %actual frequency of sound doppler effect
if F_act_tr == 0
    sortedZ = sort(z_tr);
    secondLargest = sortZ(N_tr-1);
    index = sortedZ == secondLargest;
    F_act_tr = f_tr(index); 
end 

figure(1)
tiledlayout(2,1) 
% Transmitted Audio x_time plot
nexttile
plot(t_tr(1:180),x_tr(1:180))
title('Transmitted Audio in the Time-Domain')
xlabel('t (x10^{-4} second)')
ylabel('x(t)')
% Transmitted Audio X_frequency plot
nexttile
plot(f_tr,z_tr)
title('Transmitted Audio in the Frequency-Domain')
xlabel('hz')
ylabel('z(f)')
figure()
  

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% Shotput process Audio x(t)
x_sh = Shotput_receive_1;
fs_sh = 100000;
ts_sh = 1/fs_sh;
N_sh  = 50001;
tmax_sh = (N_sh-1)*ts_sh;
t_sh = 0:ts_sh:tmax_sh;

% filter_X_sh = bandpass(x_sh,[9210 9214],fs_sh);

% Shotput setup Audio X[f]
f_sh=-fs_sh/2:fs_sh/(N_sh-1):fs_sh/2;
z_sh=fftshift(fft(x_sh));

% get non-zero frequency with largest amplitude 
[max_Z_sh, i_sh]= max(z_sh);
F_obs_sh = abs(f_sh(i_sh));      %observed frequency of sound doppler effect for shotputs
if F_obs_sh == 0
    sortedZ = sort(z_sh);
    secondLargest = sortedZ(N_sh-1 );
    index = (z_sh == secondLargest);
    F_obs_sh = abs(f_sh(index)); 
end

% shotput_received_filter = bandpass(x_sh,[9100 10000]);

figure(2)
tiledlayout(2,1) 
% Shotput Audio x_time plot
nexttile
plot(t_sh(1:100),x_sh(1:100))
title('Shotput Received Audio in the Time-Domain')
xlabel('t (x10^{-4} second)')
ylabel('X(t)')
% Shotput Audio X_frequency plot
nexttile
plot(f_sh,z_sh)
title('Shotput Audio Frequency in the Frequency-Domain')
xlabel('hz')
ylabel('z(f)')
% Shotput Filetered Audio x_time plot
% nexttile
% plot(t_sh(1:100),filter_X_sh(1:100))
% title('Shotput Filetered Audio x_time plot')
% xlabel('t (x10^{-4} second)')
% ylabel('X(t)')
figure()

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% Javelin Audio x_time 
x_ja = Javelin_receive_1;
fs_ja = 100000;
ts_ja = 1/fs_ja;
N_ja  = 50001;
tmax_ja = (N_ja-1)*ts_ja;
t_ja= 0:ts_ja:tmax_ja;

% Javelin Audio X_frequency
f_ja=-fs_ja/2:fs_ja/(N_ja-1):fs_ja/2;
z_ja=fftshift(fft(x_ja));

% filter_X_ja = bandpass(x_ja,[8000 10000],fs_ja);


% get non-zero frequency with largest amplitude 
[max_Z_ja, i_ja]= max(z_ja);
F_obs_ja = abs(f_sh(i_ja));      %observed frequency of sound doppler effect for shotputs
if F_obs_ja == 0
    sortedZ = sort(z_ja);
    secondLargest = sortedZ(N_ja-1);
    index = (z_ja == secondLargest);
    F_obs_ja = abs(f_ja(index)); 
end

figure(3)
tiledlayout(2,1) 
% Javelin Audio x_time plot
nexttile
plot(t_ja(1:700),x_ja(1:700))
title('Javelin Received Audio in the Time-Domain')
xlabel('t (x10^{-4} second)')
ylabel('x(t)')

% Javelin Audio X_frequency plot
nexttile
plot(f_ja,z_ja)
title('Javelin Received Audio in the Frequency-Domain')
xlabel('hz')
ylabel('z(f)')

% % Javelin Filetered Audio x_time plot
% nexttile
% plot(t_ja(1:700),filter_X_ja(1:700))
% title('Javelin Filetered Audio x_time plot')
% xlabel('t (x10^{-4} second)')
% ylabel('X(t)')
 figure()


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
v = 343;
ratio_frequencies_sh = F_obs_sh/F_act_tr;
ratio_frequencies_ja = F_obs_ja/F_act_tr;

V_sh = v *((1-ratio_frequencies_sh)/(1+ratio_frequencies_sh));

V_ja = v *((1-ratio_frequencies_ja)/(1+ratio_frequencies_ja));
