# numpy 的使用   
numpy.array implements a one or more dimensional array. Its type is numpy.ndarray, and we will refer to this as an ndarray for short.

```python 
import numpy as np
```

1. 使用 list 或 tuple 构建一维向量.   
```python
l = [1, 2, 3]     # you can also use tuples: (4,5,6)
x = np.array(l)
print(type(x))
print(x)
```
2. 使用多层方括号创建多维向量.   
```python
x = np.array([[1, 2, 3],
              [4, 5, 6]])
```
3. 重写 dtype parameter, 指定元素的数据类型.   
```python
x = np.array([1, 2, 3], dtype=float)
```
4. 使用下标访问向量元素.   
```python
x = np.array([[1, 2, 3],
              [4, 5, 6]])
print(x[1,2])
```
5. 和 list 类似, 通过使用 slices 功能来访问整行或整列. 冒号 (:) 表示某行或者某列的所有元素.因此 x[:,0] 返回第一列的所有数据.    
```python
print(x[:,0])    # 获取第一列
print(x[1:])     # 获取第二行
```
6. 对于 Python 的 lists, 可以使用负数的 indexes 倒着访问 array 中的元素. -1 访问的是最后一个元素, -2 访问的是倒数第二个元素, 以此类推.   
```python
print(x[1,-2:])   # 获取第二行的最后两个元素
```
7. 矩阵运算.  
|operator|运算|
|-----|------|
|+ 					|矩阵加法  |
|* 					|矩阵对应元素相乘|
|np.dot 			|矩阵乘法(线性代数) |
|.T 				|矩阵转置   |
|numpy.linalg.inv 	|矩阵的逆|
```python
x = np.array([[1., 2.],
              [3., 4.]])
print('abddition:\n', x + x)
print('\nelement-wise multiplication\n', x * x)
print('\nmultiplication\n', np.dot(x, x))
print('\ndot is also a member of np.array\n', x.dot(x))
```
**Note**:  
Python 3.5 引入了 @ 操作符来表示矩阵乘法.   
8. Helper functions, 矩阵生成辅助函数.    
zeros() to create a matrix of all zeros.    
ones() to get all ones.    
eye() to get the identity matrix.    
这三个函数均可以传入一个 tuple 参数来指定生成矩阵的维度.   
```python
print('zeros\n', np.zeros(7))
print('\nzeros(3x2)\n', np.zeros((3, 2)))
print('\neye\n', np.eye(3))
```
9. 等间距矩阵.   
(1) np.arange(start, stop, step), 类似于 Python 的 range() 函数, 不同的是它返回的是 NumPy array 类型.   
(2) np.linspace(start, stop, num), 线性等分向量和 np.arange() 稍微不同, num 是 array 的长度.   
```python
print('\narange\n', np.arange(0, 2, 0.1))
print('\nlinspace\n', np.linspace(0, 2, 20))
```
10. 数据图形化.  
Matplotlib 包含了图形显示库 pyplot. 标准作坊式 import it as plt. 一旦导入之后,就可以使用 plt.plot 来调用成员函数绘图.可以通过多次调用 plt.plot 函数绘制一系列的曲线,不同曲线以不同颜色区分.   
```python
import matplotlib.pyplot as plt
a = np.array([6, 3, 5, 2, 4, 1])
plt.plot([1, 4, 2, 5, 3, 6])
plt.plot(a)
plt.show()
```
**Note:**   
plt.plot 假设 x 轴以 1 的长度递增, 因此用户可以向 plt.plot() 函数传递 x, y 来指定自己的 x 轴数据.    
```python
plt.plot(np.arange(0,1, 0.1), [1,4,3,2,6,4,7,3,4,5]);```
