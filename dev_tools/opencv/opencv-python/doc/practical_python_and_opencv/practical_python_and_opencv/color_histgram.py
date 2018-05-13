#!/usr/bin/env python
#coding=utf-8

'''
功能: 彩色图直方图  
	统计图像中的像素值个数.


Keep in mind:
	实际调用的还是 cv2.split() 函数.
	1. 蓝色代表低像素值
	2. 红色代表高像素值
'''
from matplotlib import pyplot as plt
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
	cv2.waitKey(1)

	chans = cv2.split(image)
	colors = ('b', 'g', 'r')

	# 一维表示
	# plt.ion() 
	plt.figure()
	plt.title("Flattened color HistGram")
	plt.xlabel("Bins")
	plt.ylabel("# of pixels")

	for (chan, color) in zip(chans, colors):
		hist = cv2.calcHist([chan], [0], None, [256], [0, 256])
		plt.plot(hist, color=color)
		plt.xlim([0, 256])

	# plt.show()
	plt.pause(1)  #显示秒数
	plt.close()
	# cv2.waitKey(10)

	# 2维表示
	fig = plt.figure()
	ax = fig.add_subplot(131)
	hist = cv2.calcHist([chans[1], chans[0]], [0,1], None, [32, 32], [0, 256, 0, 256])
	p = ax.imshow(hist, interpolation="nearest")
	ax.set_title("HistGram for G and B")
	plt.colorbar(p)

	ax = fig.add_subplot(132)
	hist = cv2.calcHist([chans[1], chans[2]], [0,1], None, [32, 32], [0, 256, 0, 256])
	p = ax.imshow(hist, interpolation="nearest")
	ax.set_title("HistGram for G and R")
	plt.colorbar(p)

	ax = fig.add_subplot(133)
	hist = cv2.calcHist([chans[0], chans[2]], [0,1], None, [32, 32], [0, 256, 0, 256])
	p = ax.imshow(hist, interpolation="nearest")
	ax.set_title("HistGram for B and R")
	plt.colorbar(p)

	plt.show()
	cv2.waitKey(0)

	# 3维表示
	hist = cv2.calcHist([image], [0,1,2], None, [8, 8, 8], [0,256, 0,256, 0,256])
	print "3D HistGram shape: %s, with %d values" % (hist.shape, hist.flatten().shape[0])