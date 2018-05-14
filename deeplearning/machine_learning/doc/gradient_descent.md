# 梯度下降法    

自动驾驶中预测方向是一个连续变量问题, 因此属于回归问题.    

**Notations**:    
- $ m $: 样本的个数;    
- $ x $: 输入变量 / 特征;   
- $ y $: 输出变量 / 目标变量;   
- ($ x, y $): 训练样本;   
- ($ x^{(i)}, y^{(i)} $): 第 i 个训练样本, 即表示训练样本的第 i 行;   





## 机器学习的流程    

```flow
st=>start: Training Set
learning_algorithm=>subroutine: Learning Algorithm
hyperthis=>operation: Hyperthis
e=>end: predict 
st->learning_algorithm
learning_algorithm->hyperthis(right)->e
```

## 线性回归问题   

1. 函数假设    
$$ h_\theta(x) = \sum_{i=0}^{n} \theta_i x_i = \theta ^ Tx $$   
其中,  $ n $ 是特征的个数.    
2. cost 函数      
找到 $ \theta $ 使得下面的函数值最小:   
$$ J(\theta) = \frac{1}{2} \sum_{i = 1}^{m} (h_\theta(x^{i}) - y^{i})^2 $$    
3. 优化方法     
梯度下降法: 站在上坡上, 环顾四周之后, 决定我怎么样才能快速到达山脚下. 梯度下降法是沿着下降最快的方向走一步, 而这个方向就是梯度的方向.      
梯度下降法的特点是它最后一定会停下.    
缺点是: 梯度下降法最后达到的最小值依赖于初始点的位置, 也就是说它可能陷入局部最优 而不是全局最优.    
