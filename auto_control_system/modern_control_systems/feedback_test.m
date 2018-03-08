%% [sys] = feedback(sys1, sys2, sign)
%% 很多时候, 闭环控制系统包含的是单位反馈回路.因此反馈函数的调用方式如下: 
%% [sys] = feedback(sys, [1], sign)
%%  其中, [1] 表示闭环控制系统包含的是单位反馈回路, 即 H(s) = 1.   

clear;
clc;

%% 单位负反馈
num_g = [1, 1];
den_g = [1, 2];
num_h = [1];
den_h = [500, 0, 0];
sys_g = series(tf(num_g, den_g), tf(num_h, den_h));
sys = feedback(sys_g, [1], -1)

%% 负反馈   
num_h = [1, 1];
den_h = [1, 2];
num_g = [1];
den_g = [500, 0, 0];
sys_g = feedback(tf(num_g, den_g), tf(num_h, den_h), -1)

%% 图 2.26 的多回路化简.    
ng1 = [1]; dg1 = [1, 10];
sysg1 = tf(ng1, dg1);

ng2 = [1]; dg2 = [1, 1];
sysg2 = tf(ng2, dg2);

ng3 = [1, 0, 1]; dg3 = [1, 4, 4];
sysg3 = tf(ng3, dg3);

ng4 = [1, 1]; dg4 = [1, 6];
sysg4 = tf(ng4, dg4);

nh1 = [1, 1], dh1 = [1, 2];
sysh1 = tf(nh1, dh1);

nh2 = [2];  dh2 = [1];
sysh2 = tf(nh2, dh2);

nh3 = [1]; dh3 = [1];
sysh3 = tf(nh3, dh3);

sys1 = sysh2/ sysg4;
sys2 = feedback(series(sysg3, sysg4), sysh1, 1);
sys3 = feedback(series(sysg2, sys2), sys1, -1);
sys4 = feedback(series(sysg1, sys3), sysh3, -1);

%% 完成零-极点对消  
%% [sys] = minreal(sys1)
sys = minreal(sys4)






