#coding=utf-8
from __future__ import print_function
import numpy as np

# 1. 使用 list 构建一维向量.   
l = [1, 2, 3] 
x = np.array(l)
print(type(x))

x = np.array([[1, 2, 3],
              [4, 5, 6]])

print(x[1,0])
print(x[:,0])
print(x[1:])
print(x[1,1:])

x = np.array([[1., 2.],
              [3., 4.]])
print('abddition:\n', x + x)
print('\nelement-wise multiplication\n', x * x)
print('\nmultiplication\n', np.dot(x, x))
print('\ndot is also a member of np.array\n', x.dot(x))

print('zeros\n', np.zeros(7))
print('\nzeros(3x2)\n', np.zeros((3, 2)))
print('\neye\n', np.eye(3))

print('\nlinspace\n', np.arange(0, 2, 0.1))

import matplotlib.pyplot as plt
a = np.array([6, 3, 5, 2, 4, 1])
plt.plot([1, 4, 2, 5, 3, 6])
plt.plot(a)
plt.show()