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


def extract_fixed_amount(jpg_path, xml_path, output_dir, num = 500):
    check_if_exists(jpg_path)
    check_if_exists(xml_path)
    make_if_not_exists(output_dir)
    
    images_xmls = []

    # 获取所有的图片和 xml 信息   
    imgs = os.listdir(jpg_path)
    xmls = os.listdir(xml_path)
    if(len(imgs) != len(xmls)):
        print "error: image amount must equal to xmls amount."
        exit(1)
    for image in imgs:
        image_pre, ext = os.path.splitext(image)
        image = os.path.join(jpg_path, image)
        xml = os.path.join(xml_path, image_pre+".xml")

        images_xmls.append((image, xml))

    # 打乱数据
    random.shuffle(images_xmls)

    if len(images_xmls) < num:
        print "error: num(%d) is larger than total files amount" % num
        sys.exit(1)
    else:
        # 提取固定数量的数据
        images_xmls_to_extract = images_xmls[:num]

    # 拷贝数据到新目录
    for image, xml in images_xmls_to_extract:
        shutil.copy(image, output_dir)
        shutil.copy(xml, output_dir)
        print "copy %s..." % image
        print "copy %s..." % xml


if __name__ == '__main__':
    jpg_path = "JPEGImages"
    xml_path = "Annotations"
    output_dir = "dst"
    amount = 400 

    extract_fixed_amount(jpg_path, xml_path, output_dir, amount)