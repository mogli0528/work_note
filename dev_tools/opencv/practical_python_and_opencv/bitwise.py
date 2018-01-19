#!/usr/bin/env python
#coding=utf-8

'''
功能: 图像按位计算  
	当一个给定的像素值大于 0 ,说明这个像素是 on (white)
	当一个给定的像素值等于 0 ,说明这个像素是 off (black)

Keep in mind:
	1. and 	: 两个像素值均大于 0 , 为 true  
	2. or 	: 其中一个像素值大于 0 , 为 true  
	3. xor 	: 两个像素值不同, 为 true  
	4. not 	: 像素值反转  
'''

import numpy as np 
import argparse
import cv2
import os

# ap = argparse.ArgumentParser()
# ap.add_argument("-i", "--image", required=True, 
# 	help = "Path to the image")

# args = vars(ap.parse_args())

# if os.path.exists(args["image"]):
# 	image = cv2.imread(args["image"])
# 	cv2.imshow("original", image)
# 	cv2.waitKey(0)

rectangle = np.zeros((300,300), dtype = "uint8")

cv2.rectangle(rectangle, (25,25), (275, 275), 255, -1)
cv2.imshow("Rectangle", rectangle)
cv2.waitKey(0)

circle = np.zeros((300,300), dtype = "uint8")
cv2.circle(circle, (150, 150), 150, 255, -1)
cv2.imshow("Circle", circle)
cv2.waitKey(0)	

bitwise_and = cv2.bitwise_and(rectangle, circle)
cv2.imshow("And", bitwise_and)
cv2.waitKey(0)

bitwise_or = cv2.bitwise_or(rectangle, circle)
cv2.imshow("Or", bitwise_or)
cv2.waitKey(0)

bitwise_xor = cv2.bitwise_xor(rectangle, circle)
cv2.imshow("Xor", bitwise_xor)
cv2.waitKey(0)

bitwise_not = cv2.bitwise_not(rectangle, circle)
cv2.imshow("Not", bitwise_not)
cv2.waitKey(0)

