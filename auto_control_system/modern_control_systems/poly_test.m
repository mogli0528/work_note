%% 多项式
clear;
clc;

p = [1, 3, 0, 4];
% roots() 可以求多项式为 0 时的根, 用列向量表示.     
r = roots(p)  
% poly() 可以根据根来重构多项式
p = poly(r)

p = [3, 2, 1];
q = [1, 4]
% conv() 可以计算多项式的乘积
n = conv(p, q)
% 当变量的值给定时,可以使用 polyval() 
polyval(n, -5)

