# 液压支架顶梁检测    

Solutions:    
对于顶梁边缘检测:    
(1) 使用用户预先定好的一条顶梁线开始, 结合采煤机的运动速度和摄像机成像平面与顶梁之间的角度关系, 实现回环检测;     
(2) 直接对 ``图像增强`` 后的图像使用轮廓检测 + 霍夫直线检测, 提取直线边缘.     
(3) 目前具体的实现策略: 绘制一条先验的直线, 然后进行检测, 在 10 帧内选择 ``斜率最相近(使用向量夹角来度量)`且`和原点的距离相差最小`` 的一条直线作为新的顶梁位置.    
提前设置好一秒内可能变化的阈值范围, 如果没有检测到合适的直线, 就原地不动, 直线不进行更新. 使用时间戳进行累积, 如果超出给定的时间范围后, 直线还没有变化, 那么就需要重新权衡阈值.   
(4) 动态改变阈值, 如果可以频繁检测到直线, 那么不断减小阈值. 如果很多帧没有检测到直线了, 那么就增大阈值. 防止累积误差导致的直线死去.   
(5) 而增加阈值的过程不是一次线性增长, 而是指数级增长.   

## 方案3   
TODO list:    
- [x] 用户指定一条线, 然后以这条线为先验基准;   
- [x] 一秒内刷新一次, 而不是每一帧都刷新;   
