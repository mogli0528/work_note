%% 串联连接: [sys] = series(sys1, sys2). 传递函数相乘.      
num_g = [1]; 
den_g = [500, 0, 0];
sys_g = tf(num_g, den_g);

num_h = [1, 1]; 
den_h = [1, 2];
sys_h = tf(num_h, den_h);

sys = series(sys_g, sys_h) 


%% 并联连接: [sys] = parallel(sys1, sys2). 