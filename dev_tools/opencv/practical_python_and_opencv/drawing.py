#!/usr/bin/env python
#coding=utf-8

'''
功能: opencv 绘图操作

Keep in mind:
	1. opencv 中保存图片的 RGB 通道值是反向的.  
	2. 我们一般的写法就是 rows x columns, 而 numpy 中的写法则是 columns x rows

'''

import numpy as np
import cv2
import copy



'''
绘制直线
'''
def draw_line(canvas):
	green = (0, 255, 0)
	red = (0, 0, 255)

	# thinkness 的单位为像素
	cv2.line(canvas, (0,0), (300, 300), green, 3)
	cv2.imshow("canvas", canvas)
	cv2.waitKey(0)

	cv2.line(canvas, (300,0), (0, 300), red)
	cv2.imshow("canvas", canvas)
	cv2.waitKey(0)

	return canvas

'''
绘制矩形
'''
def draw_rectangle(canvas):
	blue = (255, 0, 0)
	green = (0, 255, 0)
	red = (0, 0, 255)

	cv2.rectangle(canvas, (10, 10), (60, 60), green)
	cv2.imshow("canvas", canvas)
	cv2.waitKey(0)

	cv2.rectangle(canvas, (50, 200), (200, 225), red, 3)
	cv2.imshow("canvas", canvas)
	cv2.waitKey(0)

	# 绘制一个实体的矩形 solid(-1) 
	cv2.rectangle(canvas, (200, 50), (225, 125), blue, -1)
	cv2.imshow("canvas", canvas)
	cv2.waitKey(0)

	return canvas

'''
绘制圆
'''
def draw_circle(canvas):
	canvas_copy = copy.deepcopy(canvas)
	(center_x, center_y) = (canvas.shape[1]/2, canvas.shape[1]/2)
	center = (center_x, center_y)
	white = (255, 255, 255)

	for r in xrange(0, 175, 25):
		cv2.circle(canvas, center, r, white, 3)

	cv2.imshow("canvas", canvas)
	cv2.waitKey(0)

	for i in xrange(25):
		radius = np.random.randint(5, high=200)
		color = np.random.randint(0, high=256, size=(3,)).tolist()
		pt = np.random.randint(0, high=300, size=(2,)).tolist()

		cv2.circle(canvas_copy, tuple(pt), radius, color, -1)

	cv2.imshow("abstract", canvas_copy)
	cv2.waitKey(0)

	return canvas


if __name__ == '__main__':
	canvas = np.zeros((300, 300, 3), dtype = np.uint8)
	# 硬拷贝
	canvas_copy = copy.deepcopy(canvas)

	canvas_line = draw_line(canvas)
	canvas_rectangle = draw_rectangle(canvas_line)
	canvas_cicle = draw_circle(canvas_copy)
