#coding=utf-8

import cv2
import matplotlib.pyplot as plt
import numpy as np

src = cv2.imread("building.jpg", cv2.IMREAD_COLOR)

kernels = [
    (u'低通滤波器', np.array([[1, 1, 1], [1, 2, 1], [1, 1, 1]])*0.1), 
    (u'高通滤波器', np.array([[0.0, -1, 0], [-1, 5, -1], [0, -1, 0]])), 
    (u'边缘检测器', np.array([[-1.0, -1, -1], [-1, 8, -1], [-1, -1, -1]]))
]

index = 0 
fig, axes = plt.subplots(1, 3, figsize = (12, 4.3))
for ax, (name, kernel) in zip(axes, kernels):
    dst = cv2.filter2D(src, -1, kernel)
    
    ax.imshow(dst[:,:,::-1])
    ax.set_title(name)
    ax.axis("off")
fig.subplots_adjust(0.01, 0, 0.98, 1, 0.02, 0)
plt.show(5000)