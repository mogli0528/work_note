%% 求传递函数的零点和极点
clear;
clc;

sys = tf([1, 10], [1, 2, 1]); 
% 求传递函数的极点
p = pole(sys)
% 求传递函数的零点
z = zero(sys)

% [P, Z] = pzmap(sys), 返回零点列向量和极点列向量
% 如果将等号左侧置空就会自动生成零极点分布图: pzmap(sys)   
pzmap(sys)


%% 实例
% G(s)
num_g = [6, 0, 1];
den_g = [1, 3, 3, 1];
sys_g = tf(num_g, den_g);
z = zero(sys_g)
p = pole(sys_g)

% H(s)
num_h1 = [1, 1];
num_h2 = [1, 2];
den_h1 = [1, 2*i]; 
den_h2 = [1, -2*i];
den_h3 = [1, 3];
num_h = conv(num_h1, num_h2);
den_h = conv(den_h1, conv(den_h2, den_h3));
sys_h = tf(num_h, den_h);

% G(s) / H(s)
sys = sys_g / sys_h;
pzmap(sys)
