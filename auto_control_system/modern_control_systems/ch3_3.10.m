%% 磁盘驱动系统  
%  简化为双质量块系统   
clear;
clc;

% 模型参数
k = 10;     % 簧片弹性系数, 单位:N/m
M1 = 0.02;  % 磁头支架质量, 单位:kg
M2 = 0.0005;
b1 = 410e-3;% 质量块的摩擦系数, 单位:N/(m/s)
b2 = 4.1e-3;
t = [0 : 0.001 : 1.5];

% 状态空间模型  
A = [0 0 1 0;
	 0 0 0 1;
	 -k/M1 k/M1 -b1/M1 0;
	 k/M2 -k/M2 0 -b2/M2
	];
B = [0; 0; 1/M1; 0];
C = [0 0 0 1];
D = [0];
sys = ss(A, B, C, D);

%% Simulated Step Response
y = step(sys, t);
plot(t, y); grid
xlabel('Time(s)');
ylabel('v_t(m/s)');