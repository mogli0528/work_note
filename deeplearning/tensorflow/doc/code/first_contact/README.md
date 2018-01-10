# `First Contact With Tensorflow` 笔记+源码  

## 术语  
描述维度：Shape，Rank，Dimension Number。  
tensorflow获取数据的3种方式：数据文件，预加载的常量或变量，python代码生成的数据。  
1. 执行数学计算的同时降维(reduce)    

|操作|描述|
|----|----|
|tf.reduce_sum |沿着某一维计算元素的和|
|tf.reduce_prod| 沿着某一维计算元素的积|
|tf.reduce_min |沿着某一维计算元素的最小值|
|tf.reduce_max |沿着某一维计算元素的最大值|
|tf.reduce_mean| 沿着某一维计算元素的平均值|

2.

## 1. 线性回归
[源码](./linear_regression.py)   
## 2. 聚类分析   
[源码](./clustering.py)   
## 3. `mnist`单层神经网络   
[源码](./mnist.py)   
