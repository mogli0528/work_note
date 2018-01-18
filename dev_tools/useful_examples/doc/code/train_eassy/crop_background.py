#coding=utf-8

import os
import cv2
import numpy as np

# 挑选图片,随机裁剪背景图

def crop_area(image_name, num):
	image = cv2.imread(image_name)
	areas = []
	nums = 0 
	range_ = long(num**2)
	for i in xrange(range_):
		x_start = np.random.randint(100, 400)
		y_start = np.random.randint(120, 300)
		crop_width = np.random.randint(100, 200)
		crop_height = np.random.randint(100, 200)

		if x_start+crop_width < 650 and y_start+crop_height < 400:
			nums += 1
			areas.append((x_start, y_start, crop_width, crop_height))
			name = "{}_{:>04}.jpg".format(os.path.splitext(image_name)[0], i)
			cv2.imwrite(name, image[y_start:y_start+crop_height, x_start:x_start+crop_width])
		# 	print x_start, y_start, crop_width, crop_height
		if nums >= num:
			print "get..."
			return (x_start, y_start, crop_width, crop_height)


if __name__ == '__main__':
	# data_dir = "/home/klm/picture"
	data_dir = "/home/klm/Desktop/new_background"

	num = 1700
	# image_b = cv2.imread(data_dir + "/5200004.jpg")
	# image_w = cv2.imread(data_dir + "/5201725.jpg")
	crop_area(data_dir + "/bg1_0000.jpg", num)
	crop_area(data_dir + "/bg2_0000.jpg", num)
	
	# x_start = 100
	# y_start = 120
	# crop_width = 550
	# crop_height = 250
	# cv2.rectangle(image, (x_start, y_start), (x_start+crop_width, y_start+crop_height), (0, 255, 0), 2)
	# cv2.imshow("origin", image)
	# cv2.waitKey(0)