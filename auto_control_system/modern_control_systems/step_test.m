%% 计算牵引电机的 step 响应   
%% 调用 step() 时, 如果等号左边置空, 则绘制输出响应.
clear;
clc;

num = [5400];
den = [2, 2.5, 5402];
sys = tf(num, den);
t = [0 : 0.005 : 3];
% step(sys, t);   % 绘制输出响应
[y, t] = step(sys, t);
plot(t, y), grid;
xlabel('Time(s)');
ylabel('Wheel velocity(m/s)');

%% 磁盘驱动器示例    
num = [200];
den = [1, 20, 200];
sys = tf(num, den);
t = [0 : 0.005 : 3];
step(sys, t);   % 绘制输出响应

%% test    
num = [20]; 
den2 = [1];
den1 = [1, 10, 25];
den = conv(den1, den2);
sys = tf(num, den);
pzmap(sys)

