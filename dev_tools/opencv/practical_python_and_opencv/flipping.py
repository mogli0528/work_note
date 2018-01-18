#!/usr/bin/env python
#coding=utf-8

'''
功能: 图像镜像处理

Keep in mind:
	1. flip 操作可以分别沿着 x(0), y(1). 或者沿着 x, y 同时翻转(-1).  
	2. 我们一般的写法就是 rows*columns(width*height), 
		而 numpy 中的写法则是 column*rows(height*width)

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

	# flip   
	## [[1, 0, 25], [0, 1, 50]]
	## 其中 [1, 0, 25] 的 25 表示要右移的像素值,负数表示左移   
	## 其中 [0, 1, 50] 的 50 表示要上移的像素值,负数表示下移   
	flipped = cv2.flip(image, 1)
	cv2.imshow("Flipped Horizontally", flipped)
	cv2.waitKey(0)

	flipped = cv2.flip(image, 0)
	cv2.imshow("Flipped Vertically", flipped)
	cv2.waitKey(0)

	flipped = cv2.flip(image, -1)
	cv2.imshow("Flipped Horizonlly & Vertically", flipped)
	cv2.waitKey(0)