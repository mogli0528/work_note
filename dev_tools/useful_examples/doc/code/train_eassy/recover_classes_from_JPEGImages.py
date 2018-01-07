#coding=utf-8
'''
功能： 将 Annotations 和 JPEGImages 中的文件还原为独立目录封装的数据
      其中包含有.txt文件
注意： 
    这个脚本只处理单个 bounding box 的情形,直接忽略多个 bounding box
      
changelog:

'''

import os
import cv2
import shutil
from xml.etree import ElementTree as ET  

JPEGImages = "jpgs"
Annotations = "xmls"
Txts = "txts"
cluster_dir = "new_cluster"

def check_if_exist(path):
    return os.path.exists(path)

def make_if_not_exist(path):
    if not os.path.exists(path):
        os.makedirs(path)

#code
def iter_all_files_in_database(jpgPath, xmlPath):
    # list all .*jpg files
    imgs = os.listdir(jpgPath)
    imgs = [(imgs[i], jpgPath + "/" + imgs[i]) for i in range(len(imgs))]
    for i in range(len(imgs)-1, -1, -1):
        if not imgs[i][1].lower().endswith("jpg") and not imgs[i][1].lower().endswith("png") and not imgs[i][1].lower().endswith("jpeg"):
            del imgs[i]

    # list all .*xml files
    xmls = os.listdir(xmlPath)
    xmls = [(xmls[i], xmlPath + "/" + xmls[i]) for i in range(len(xmls))]
    for i in range(len(xmls)-1, -1, -1):
        if not xmls[i][1].lower().endswith("xml"):
            del xmls[i]

    if(len(imgs) != len(xmls)):
        print "error: imgs is not equals to xmls counts."
        exit(1)
    else:
        return imgs, xmls

def extract_classname_from_xml(path):
    '''
    从标注好的 xml 文件中提取 class_name 信息
    即使一个 xml 中可能包含有同一个类别的多个 bounding box, 也可以处理.

    Args:
        path: 每个xml文件的路径

    Returns:
        xml中包含的类别信息
    '''
    
    # print path
    per=ET.parse(path) 

    class_name = ''     # 存放本 xml 文件中所有的 name

    # 找出 xml 中包含的所有的 name, 和下面的 bndbox 意义对应
    name_node = per.getiterator("name") 
    if len(name_node) > 0:
        for name in name_node:
            if class_name == '':
                class_name = name.text
            # for name in name_node:
            if name.text != class_name:
                print "warning: class number larger than 1, ignore: %s" % class_name

    return class_name

def cluster_files(jpgPath, xmlPath, txtPath, output_dir):
    make_if_not_exist(output_dir)

    each_class_number = {}

    imgs, xmls = iter_all_files_in_database(JPEGImages, Annotations)
    for i in range(len(imgs)):
        pos = imgs[i][0].rfind(".")   # 文件扩展名的前缀

        # every xml file
        jpg_file = os.path.join(jpgPath, imgs[i][0][:pos]+".jpg")
        # txt_file = os.path.join(txtPath, imgs[i][0][:pos]+".xml.txt")
        xml_file = os.path.join(xmlPath, imgs[i][0][:pos]+".xml")
        class_name = extract_classname_from_xml(xml_file)

        if class_name != '':
            # dict 必须显示初始化
            if class_name not in each_class_number:
                each_class_number[class_name] = 1
            else:
                each_class_number[class_name] += 1

            # copy data
            class_name = os.path.join(output_dir, class_name)
            make_if_not_exist(class_name)
            shutil.copy(xml_file, class_name)
            shutil.copy(jpg_file, class_name)
            # shutil.copy(txt_file, class_name)
    
    return each_class_number

if __name__ == '__main__':
    each_class_number = cluster_files(JPEGImages, Annotations, Txts, cluster_dir)
    each_class_number = sorted(each_class_number.items(), lambda x, y: cmp(x[1], y[1]))
    for item in each_class_number:
        print  item[0],item[1] 
       