%% tf() 传递函数的使用  
clear;
clc;

%sys = tf([1, 0], [1, 2, 1])
num_1 = [10];
den_1 = [1, 2, 5];
sys_1 = tf(num_1,den_1)

num_2 = [1];
den_2 = [1, 1];
sys_2 = tf(num_2, den_2)

sys = sys_1 + sys_2
