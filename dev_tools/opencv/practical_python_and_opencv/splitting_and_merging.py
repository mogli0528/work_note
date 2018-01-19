#!/usr/bin/env python
#coding=utf-8

'''
功能: 图像通道拆分和合并  
	提取感兴趣的图像内容(还可以使用crop).

Keep in mind:
	实际调用的还是 cv2.split() 函数.
'''

import numpy as np 
import argparse
import cv2
import os
import copy

ap = argparse.ArgumentParser()
ap.add_argument("-i", "--image", required=True, 
	help = "Path to the image")

args = vars(ap.parse_args())

if os.path.exists(args["image"]):
	image = cv2.imread(args["image"])
	image_copy = copy.deepcopy(image)
	cv2.imshow("original", image)
	cv2.waitKey(0)

	(b,g,r) = cv2.split(image)
	cv2.imshow("Blue", b)
	cv2.imshow("Green", g)
	cv2.imshow("Red", r)

	# merged = cv2.merge([b,g,r])
	# cv2.imshow("Merged", merged)
	# cv2.waitKey(0)

	zero = np.zeros(image.shape[:2], dtype="uint8")
	cv2.imshow("Blue+z", cv2.merge([b, zero, zero]))
	cv2.imshow("Green-z", cv2.merge([zero, g, zero]))
	cv2.imshow("Red-z", cv2.merge([zero, zero, r]))
	cv2.waitKey(0)


