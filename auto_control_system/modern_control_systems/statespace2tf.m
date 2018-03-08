%% 状态空间到传递函数的转换   
clear;
clc;

num = [2, 8, 6];
den = [1, 8, 16, 6];
sys_tf = tf(num, den);
sys_ss = ss(sys_tf);
sys = tf(sys_ss)

%% expm() 和 exp() 函数    
%  expm() 是计算状态转移矩阵的. exp() 是对矩阵中每个元素计算指数函数的.  
A = [0, -2; 1 -3];
dt = 0.2; 
phi = expm(A*dt);
x0 = [1, 1];
x = phi*x0'

%%' [y, T, x] = lsim(sys, u, t, x0)  求解状态方程表示的系统的输出时间响应   
%   其中, y(t) 是 t 时刻的输出响应;
%   其中, T 是 时间常量;
%   其中, x(t) 是 t 时刻的状态响应;
%   其中, u 是系统输入;
%   其中, t 是所求的系统响应为 t 时刻对应的响应;
%   其中, x0 是系统初始条件(可选);
A = [0, -2; 1, -3];
B = [2; 0];
C = [1, 0];
D = [0];
sys = ss(A, B, C, D);
x0 = [1, 1];
t = [0 : 0.01 : 1];
u = 0 * t;
[y, t, x] = lsim(sys, u, t, x0);
subplot(121), plot(t, x(:, 1));grid
xlabel('Time(s)'), ylabel('x_1')
subplot(122), plot(t, x(:, 2));grid
xlabel('Time(s)'), ylabel('x_2')