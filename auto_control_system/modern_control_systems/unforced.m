%% compute unforced response to an initail condition
% theta = arccos(zeta)
c = (y0 / sqrt(1 - zeta^2));
y = c* exp(-zeta * wn * t) .* sin(wn * sqrt(1 - zeta^2) * t + acos(zeta));
%
bu = c * exp(-zeta * wn * t);
bl = -bu;
% 
plot(t,y, t,bu,'--', t,bl,'-'), grid
xlabel('Time(s)'), ylabel('y(t)(m)')
legend(['\omega_n=',num2str(wn), '   \zeta=',num2str(zeta)])
