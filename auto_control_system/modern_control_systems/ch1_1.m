%% 绘制 质量块-弹簧-阻尼系统 的系统的响应曲线   
y0 = 0.15;
wn = sqrt(2);
zeta = 1 / (2 * sqrt(2));
t = [0 : 0.1 : 10];
%unforced;
%% compute unforced response to an initail condition
% theta = arccos(zeta)
c = (y0 / sqrt(1 - zeta^2));
y = c* exp(-zeta * wn * t) .* sin(wn * sqrt(1 - zeta^2) * t + acos(zeta));
%
bu = c * exp(-zeta * wn * t);
bl = -bu;
% 
plot(t,y, t,bu,'--', t,bl,'--'), grid, hold on

%% wn 使用比较大的值时, 观察系统的响应曲线
wn1 = 2*sqrt(2);
zeta1 = 1 / (2 * sqrt(2));
y = c* exp(-zeta1 * wn1 * t) .* sin(wn1 * sqrt(1 - zeta1^2) * t + acos(zeta1));
%
bu = c * exp(-zeta1 * wn1 * t);
bl = -bu;
plot(t,y, t,bu,':', t,bl,':'), grid
xlabel('Time(s)'), ylabel('y(t)(m)')
legend(['\omega_n=',num2str(wn), '   \zeta=',num2str(zeta)], ['\omega_n1=',num2str(wn1), '   \zeta1=',num2str(zeta1)])
