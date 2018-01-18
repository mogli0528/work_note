#!/usr/bin/env python
#coding=utf-8

'''
功能: 图像平移处理

Keep in mind:
	1. opencv 中保存图片的 RGB 通道值是反向的.  
	2. 我们一般的写法就是 rows x columns, 而 numpy 中的写法则是 columns x rows

'''

import numpy as np
import cv2
import argparse
import my_imutils as mi
import os

ap = argparse.ArgumentParser()
ap.add_argument("-i", "--image", required=True, 
	help="Path to the image")

args = vars(ap.parse_args())

if os.path.exists(args["image"]):
	image = cv2.imread(args["image"])
	cv2.imshow("original", image)
	cv2.waitKey(0)	

	# 平移矩阵  
	## [[1, 0, 25], [0, 1, 50]]
	## 其中 [1, 0, 25] 的 25 表示要右移的像素值,负数表示左移   
	## 其中 [0, 1, 50] 的 50 表示要上移的像素值,负数表示下移   
	M = np.float32([[1, 0, 25], [0, 1, 50]])
	shifted = cv2.warpAffine(image, M, (image.shape[1], image.shape[0]))
	cv2.imshow("Shifted down and right", shifted)
	cv2.waitKey(0)	

	M = np.float32([[1, 0, -50], [0, 1, -90]])
	shifted = cv2.warpAffine(image, M, (image.shape[1], image.shape[0]))
	cv2.imshow("Shifted up and left", shifted)

	cv2.waitKey(0)	

	shifted = mi.shift(image, 0, 100)
	cv2.imshow("Shifted down", shifted)
	cv2.waitKey(0)