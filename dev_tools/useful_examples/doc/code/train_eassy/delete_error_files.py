#!/usr/bin/env python
#coding=utf-8
'''
功能： 删除经过 ROI 挑选后存在错误的样本
      需要挑选着提供正确的样本列表 .txt 文件
'''

import os
import fileinput

right_file_dir = "/home/lsn/data/jst_new/new/right_files"
database_dir = "/home/lsn/data/jst_new/new/1227-jst-end"

for x in os.listdir(right_file_dir):
    # x = jst184.txt ... 
    if x.find("jst") != -1:
        filename, extname = os.path.splitext(x)
        all_files = list(set([item[:item.find(".")] for item in os.listdir("{}/{}".format(database_dir, filename))]))
        right_file = "{}/{}".format(right_file_dir, x)
        if not os.path.isfile(right_file):
            continue
        right_files = [item[:item.find(".")] for item in fileinput.input(right_file)]
        err_files = [item for item in all_files if item not in right_files]
        
        for item in err_files:
            jpg_file = "{}/{}/{}.jpg".format(database_dir, filename, item)
            xml_file = "{}/{}/{}.xml".format(database_dir, filename, item)
            txt_file = "{}/{}/{}.xml.txt".format(database_dir, filename, item)
            os.remove(jpg_file)
            os.remove(xml_file)
            os.remove(txt_file)
            print("Delete => " + jpg_file)
            print("Delete => " + xml_file)
            print("Delete => " + txt_file) 