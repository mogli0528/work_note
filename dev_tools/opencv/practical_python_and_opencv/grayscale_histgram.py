#!/usr/bin/env python
#coding=utf-8

'''
功能: 图像通道拆分和合并  
	提取感兴趣的图像内容(还可以使用crop).

Keep in mind:
	实际调用的还是 cv2.split() 函数.
'''
from matplotlib import pyplot as plt
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

	image = cv2.cvtColor(image, cv2.COLOR_RGB2GRAY)
	cv2.imshow("GrayScale", image)
	cv2.waitKey(0)

	hist = cv2.calcHist([image], [0], None, [256], [0, 256])

	plt.figure()
	plt.title("HistGram of a GrayScale image")
	plt.xlabel("bins")
	plt.ylabel("# of pixels")
	plt.xlim([0, 256])
	plt.show(hist)

	cv2.waitKey(10)
