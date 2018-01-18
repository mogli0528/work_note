#!/usr/bin/env python
#coding=utf-8

'''
功能: 图像 resize 处理

Keep in mind:
	1. resize 操作要保持 aspect ratio.  
	2. 我们一般的写法就是 rows*columns(width*height), 
		而 numpy 中的写法则是 column*rows(height*width)
	3. resize 的插值方式 
	 	cv2.INTER_LINEAR (效果比较好)
		cv2.INTER_CUBIC
		cv2.INTER_NEAREST
		

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

	# resize   
	## [[1, 0, 25], [0, 1, 50]]
	## 其中 [1, 0, 25] 的 25 表示要右移的像素值,负数表示左移   
	## 其中 [0, 1, 50] 的 50 表示要上移的像素值,负数表示下移   
	(h, w) = image.shape[:2]
	r = 150.0 / w
	dim = (150, int(r*h))

	resized = cv2.resize(image, dim, interpolation = cv2.INTER_LINEAR)
	cv2.imshow("Resized", resized)
	cv2.waitKey(0)

	r = 200.0 / h
	dim = (int(r*w), 200)
	resized = cv2.resize(image, dim, interpolation = cv2.INTER_LINEAR)
	cv2.imshow("Reseized_height", resized)
	cv2.waitKey(0)

	resized = mi.resize(image, width = 50)
	cv2.imshow("Resized", resized)
	cv2.waitKey(0)