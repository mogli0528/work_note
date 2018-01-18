#!/usr/bin/env python
#coding=utf-8

'''
功能: 访问和操作像素值

Keep in mind:
	1. opencv 中保存图片的 RGB 通道值是反向的.  
	2. 我们一般的写法就是 rows x columns, 而 numpy 中的写法则是 columns x rows

颜色表:

• Black: (0,0,0)
• White: (255,255,255)
• Red: (255,0,0)
• Green: (0,255,0)
• Blue: (0,0,255)
• Aqua: (0,255,255)
• Fuchsia: (255,0,255)
• Maroon: (128,0,0)
• Navy: (0,0,128)
• Olive: (128,128,0)
• Purple: (128,0,128)
• Teal: (0,128,128)
• Yellow: (255,255,0)
'''

import argparse
import cv2

ap = argparse.ArgumentParser()
ap.add_argument("-i", "--image", required=True, 
	help="Path to the image")

args = vars(ap.parse_args())

image = cv2.imread(args["image"])
cv2.imshow("Origin", image)

(b, g, r) = image[0, 0]
print "Pixel at (0, 0) - Red: %d, Green: %d, Blue: %d" % (r, g, b)
image[0, 0] = (0, 0, 255)
(b, g, r) = image[0, 0]
print "Pixel at (0, 0) - Red: %d, Green: %d, Blue: %d" % (r, g, b)

# 更改一大块像素值
corner = image[0:100, 0:120]
cv2.imshow("corner", corner)

# numpy 切片格式
# 100 行, 120 列  
# image[y_start:y_end, x_start:x_end]

image[0:100, 0:120] = (0, 0, 255)
cv2.imshow("updata", image)

cv2.waitKey(0)



