#!/usr/bin/env python
#coding=utf-8

'''
功能: 读取图片,显示图片,再保存图片

'''

import argparse
import cv2

ap = argparse.ArgumentParser()
ap.add_argument("-i", "--image", required=True, help = "Path to the image")
# 解析命令行参数并将其保存在 dictionary 中
args = vars(ap.parse_args())
print args["image"]
image = cv2.imread(args["image"])
# 注意宽高的顺序
# Keep in mind:
# 我们一般的写法就是 rows x columns, 而 numpy 中的写法则是 columns x rows
print "width: %d pixels" % image.shape[1]
print "height: %d pixels" % image.shape[0]
print "channels: %d pixels" % image.shape[2]

cv2.imshow("Image", image)
# 以 95% 的质量保存图片
# 以默认质量保存图片
cv2.imwrite("newimage.jpg", image, (cv2.IMWRITE_JPEG_QUALITY, 95))
cv2.waitKey(0)


