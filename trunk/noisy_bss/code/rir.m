function [h]=rir(fs, mic, n, r, rm, src);
%RIR   Room Impulse Response.
%   [h] = RIR(FS, MIC, N, R, RM, SRC) performs a simple room impulse
%         response calculation.
%
%      FS =  sample rate.
%      MIC = row vector giving the x,y,z coordinates of
%            the microphone.  
%      N =   The program will account for (2*N+1)^3 virtual sources 
%      R =   reflection coefficient for the walls, in general -1&lt;R&lt;1.
%      RM =  row vector giving the dimensions of the room.  
%      SRC = row vector giving the x,y,z coordinates of 
%            the sound source.
%
%   EXAMPLE:
%
%      fs=44100;
%      rm=[20 19 21];
%      mic=[19 18 1.6];
%      src=[5 2 1];
%      r=0.3;
%      n=12;
%      h=rir(fs, mic, n, r, rm, src);
%
%   NOTES:
%
%   1) To implement this filter, you will need to do a fast 
%      convolution.  The program FCONV.m will do this. It is available
%      at http://www.2pi.us/code/fconv.m
%   2) All distances are in meters.
%   3) A paper has been written on this model.  It is available at:
%      http://www.2pi.us/rir.html
%      
%
%Version 3.2
%Copyright ??? 2003 Stephen G. McGovern

%Some of the following comments are references to equations in my paper.

nn=[-n:1:n];                          % Index for the sequence
rms=nn+0.5-0.5*(-1).^nn;              % Part of equations 2,3,&amp; 4
srcs=(-1).^(nn);                      % part of equations 2,3,&amp; 4
xi=[srcs*src(1)+rms*rm(1)-mic(1)];    % Equation 2 
yj=[srcs*src(2)+rms*rm(2)-mic(2)];    % Equation 3 
zk=[srcs*src(3)+rms*rm(3)-mic(3)];    % Equation 4 

[i,j,k]=meshgrid(xi,yj,zk);           % convert vectors to 3D matrices
d=sqrt(i.^2+j.^2+k.^2);               % Equation 5
time=round(fs*d/343)+1;               % Similar to Equation 6
              
[e,f,g]=meshgrid(nn, nn, nn);         % convert vectors to 3D matrices
c=r.^(abs(e)+abs(f)+abs(g));          % Equation 9
e=c./d;                               % Equivalent to Equation 10

h=full(sparse(time(:),1,e(:)));       % Equivalent to equation 11
