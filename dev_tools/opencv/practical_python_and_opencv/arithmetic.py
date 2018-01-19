#!/usr/bin/env python
#coding=utf-8

'''
功能: 图像算术计算

Keep in mind:
	1. 算术运算需要注意 color space 和 data type.   
	2. opencv 和 numpy 的处理方式不同,根据需要进行选择.  
		opencv 内部的处理方式: 截取
	   	numpy 内部的处理方式: 回绕

'''

import numpy as np 
import argparse
import cv2
import os

ap = argparse.ArgumentParser()
ap.add_argument("-i", "--image", required=True, 
	help = "Path to the image")

args = vars(ap.parse_args())

if os.path.exists(args["image"]):
	image = cv2.imread(args["image"])
	cv2.imshow("original", image)
	cv2.waitKey(0)

	# opencv 内部的处理方式: 截取
	print "cv2: max of 255: " + str(cv2.add(np.uint8([200]), np.uint8([100])))
	print "cv2: min of 0: " + str(cv2.subtract(np.uint8([50]), np.uint8([100])))

	# numpy 内部的处理方式: 回绕
	print "wrap around + : " + str(np.uint8([200]) + np.uint8([100]))
	print "wrap around - : " + str(np.uint8([20]) - np.uint8([100]))

	M = np.ones(image.shape, dtype = "uint8")*100	
	added = cv2.add(image, M)
	cv2.imshow("Add", added)
	cv2.waitKey(0)

	M = np.ones(image.shape, dtype = "uint8")*50
	subtracted = cv2.subtract(image, M)
	cv2.imshow("Subtract", subtracted)
	cv2.waitKey(0)
