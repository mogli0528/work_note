#!/usr/bin/env python
#coding=utf-8

'''
功能: 图像旋转处理

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

	# 旋转矩阵  
	## [[1, 0, 25], [0, 1, 50]]
	## 其中 [1, 0, 25] 的 25 表示要右移的像素值,负数表示左移   
	## 其中 [0, 1, 50] 的 50 表示要上移的像素值,负数表示下移   
	(h, w) = image.shape[:2]
	center = (w / 2, h / 2)

	M = cv2.getRotationMatrix2D(center, 45, 0.5)
	rotated = cv2.warpAffine(image, M, (w, h))
	cv2.imshow("Rotated by 45 degrees", rotated)
	cv2.waitKey(0)	

	M = cv2.getRotationMatrix2D(center, -90, 0.5)
	rotated = cv2.warpAffine(image, M, (w, h))
	cv2.imshow("Rotated by -90 degrees", rotated)
	cv2.waitKey(0)	

	rotated = mi.rotate(image, 180)
	cv2.imshow("Rotated by 180 Degrees", rotated)
	cv2.waitKey(0)