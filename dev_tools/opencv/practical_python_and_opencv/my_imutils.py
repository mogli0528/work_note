#!/usr/bin/env python
#coding=utf-8


import numpy as np
import cv2

# 平移
def shift(image, x, y):
	M = np.float32([[1, 0, x], [0, 1, y]])
	shifted = cv2.warpAffine(image, M, (image.shape[1], image.shape[0]))

	return shifted

# 旋转变换
def rotate(image, angle, center = None, scale = 1.0):
	(h, w) = image.shape[:2]

	if center is None:
		center = (w / 2, h / 2)

	M = cv2.getRotationMatrix2D(center, angle, scale)

	return cv2.warpAffine(image, M, (w, h))


def resize(image, width=None, height=None, inter=cv2.INTER_LINEAR):

	dim = None

	(h,w) = image.shape[:2]

	if width is None and height is None:
		return image

	if height is not None:
		r = height / float(h)
		dim = (int(r*w), height)
	else:
		r = width / float(w)
		dim = (width, int(r*h))

	return cv2.resize(image, dim, interpolation = inter)
