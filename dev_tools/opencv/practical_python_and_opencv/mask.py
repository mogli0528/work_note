#!/usr/bin/env python
#coding=utf-8

'''
功能: 图像掩模 mask 计算  
	提取感兴趣的图像内容(还可以使用crop).

Keep in mind:
	实际调用的还是 cv2.bitwise_and() 函数.
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


	(h, w) = image.shape[:2]
	mask = np.zeros((h, w), dtype = "uint8")

	# draw a white rectangle   
	(c_x, c_y) = (w / 2, h / 2)
	cv2.rectangle(mask, (c_x - 75, c_y - 75), (c_x + 75, c_y + 75), 255, -1)
	cv2.imshow("Mask", mask)
	cv2.waitKey(0)

	masked = cv2.bitwise_and(image, image, mask=mask)
	cv2.imshow("Rectangle Masked", masked)
	cv2.waitKey(0)

	mask = np.zeros((h, w), dtype = "uint8")
	cv2.circle(mask, (c_x, c_y), 100, 255, -1)
	masked = cv2.bitwise_and(image_copy, image_copy, mask=mask)
	cv2.imshow("Circle Masked", masked)
	cv2.waitKey(0)


