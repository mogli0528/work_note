#coding=utf-8
'''
功能： 提取固定数量的 voc 中的数据
    amount 变量控制提取的数量  

changelog:
     
'''

import os
import cv2
import time
from xml.etree import ElementTree as ET  
import random
import shutil

def check_if_exists(path):
    if not os.path.exists(path):
        print "No such file or dirextory: %s." % path
        sys.exit(1)

def make_if_not_exists(path):
    if not os.path.exists(path):
        os.makedirs(path)


def seprate_data(data_dir, dst_dir, num):
    check_if_exists(data_dir)

    c_amount = {}
    c_full = ""

    classes = os.listdir(data_dir)  
    for c in classes:
        c_full = os.path.join(data_dir, c)
        amount = os.listdir(c_full)
        c_amount[c]=len(amount)

    # return c_amount
    c_amount = sorted(c_amount.items(), lambda x, y: cmp(x[1], y[1]))
    for c, amount in c_amount:
        if int(amount) > num:
            shutil.copytree(os.path.join(data_dir, c), os.path.join(dst_dir, c))
            print "copy: %s ..." %(c)
            shutil.rmtree(os.path.join(data_dir, c))


def extract_fixed_amount(jpg_path, output_dir, num = 500):
    check_if_exists(jpg_path)
    make_if_not_exists(output_dir)
    
    images = []

    # 获取所有的图片和 xml 信息   
    imgs = os.listdir(jpg_path)
    for image in imgs:
        image = os.path.join(jpg_path, image)
        images.append(image)

    # 打乱数据
    random.shuffle(images)

    if len(images) < num:
        print "error: num(%d) is larger than total files amount" % num
        sys.exit(1)
    else:
        # 提取固定数量的数据
        images_to_extract = images[:num]

    # 拷贝数据到新目录
    for image in images_to_extract:
        shutil.copy(image, output_dir)
        print "copy %s..." % image


if __name__ == '__main__':
    data_path = "jst_v2"
    dst_path = "need_to_extract"
    output_dir = "dst"
    amount = 2500 
    # 将需要处理的数据单独存放 
    seprate_data(data_path, dst_path, amount)
    #
    classes = os.listdir(dst_path)
    for c in classes:
        extract_fixed_amount(os.path.join(dst_path, c), os.path.join(output_dir, c), amount)