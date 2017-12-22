#coding=utf-8
'''
功能： 统计样本中所有类别的总数
      后期可以添加单个样本占总样本的百分比
'''

import os
import cv2
import time
from xml.etree import ElementTree as ET  

# 保存.jpg和.xml文件的目录
# test
# jpgPath = "jpg"
# xmlPath = "xml"

# Standard directory
jpgPath = "JPEGImages"
xmlPath = "Annotations"



def check_files():
    #code
    imgs = os.listdir(jpgPath)
    imgs = [(imgs[i], jpgPath + "/" + imgs[i]) for i in range(len(imgs))]
    for i in range(len(imgs)-1, -1, -1):
        if not imgs[i][1].lower().endswith("jpg") and not imgs[i][1].lower().endswith("png") and not imgs[i][1].lower().endswith("jpeg"):
            del imgs[i]

    xmls = os.listdir(xmlPath)
    xmls = [(xmls[i], xmlPath + "/" + xmls[i]) for i in range(len(xmls))]
    for i in range(len(xmls)-1, -1, -1):
        if not xmls[i][1].lower().endswith("xml"):
            del xmls[i]

    # The two type files must equal   
    if len(imgs) == 0:  
        print "empty imgs dir."
        exit(1)
    else:
        if(len(imgs) != len(xmls)):
            print "error: imgs is not equals to xmls counts."
            exit(1)
    return imgs, xmls

def calc_amount(path, amount_dict):
    
    # print path
    per=ET.parse(path) 

    name_node = per.getiterator("name") 
    for name in name_node:
        # print "node.text:%s" % name.text   
        cls_name = name.text
        if cls_name in amount_dict:
            amount_dict[cls_name] += 1
        else:
            amount_dict[cls_name] = 1

if __name__ == '__main__':
    imgs, xmls = check_files()

    amount_dict = {}
    i = 0
    while i < len(imgs):
        pos = imgs[i][0].rfind(".")   # 文件扩展名的前缀
        calc_amount("%s/%s.xml" %(xmlPath, imgs[i][0][:pos]), amount_dict)
        
        i = i+1
    
    # 排序
    amount_dict = sorted(amount_dict.items(), lambda x, y: cmp(x[1], y[1]))
    items_per_line = 3

    for i in range(items_per_line):
        print '|类别|数量',
    print '|' 
    for i in range(items_per_line):
        print '|----|----',
    print '|' 
    
    i = 0
    sum_ = 0
    
    for item in amount_dict:
        print '|', item[0], '|', item[1], '    ',
        sum_+= item[1]
        i += 1
        if i % items_per_line == 0:
            print '|'

    print '|'
    print "sum:", sum_


