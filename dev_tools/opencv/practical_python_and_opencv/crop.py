#!/usr/bin/env python
#coding=utf-8

'''
功能: 图像裁剪处理

Keep in mind:
	1. 裁剪操作可以分别沿着 x(0), y(1). 或者沿着 x, y 同时翻转(-1).  
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

	# crop   
	# 裁剪框的左上角点是(240, 30),右上角点(335, 120) 
	# numpy slice 的 4 个 indexs:
	#	start_y, end_y
	#	start_x, end_x
	cropped = image[30:120, 240:335]
	cv2.imshow("Cropped", cropped)
	cv2.waitKey(0)
