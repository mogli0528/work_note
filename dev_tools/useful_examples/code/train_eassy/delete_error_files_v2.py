#!/usr/bin/env python
#coding=utf-8
'''
功能： 对截出的 ROI 类别进行挑选, 根据这些正确的样本对 ssd 标注样本进行清洗.

'''

import os
import fileinput

def del_error_files(right_file_dir, ssd_data_dir):
    for x in os.listdir(right_file_dir):
        if x.find("jst") != -1:
            filename, extname = os.path.splitext(x)
            all_files = list(set([item[:item.find(".")] for item in os.listdir("{}/{}".format(ssd_data_dir, filename))]))
            right_file = "{}/{}".format(right_file_dir, x)
            if not os.path.isfile(right_file):
                continue
            right_files = [item[:item.find(".")] for item in os.listdir(right_file)]
            err_files = [item for item in all_files if item not in right_files]
            
            for item in err_files:
                jpg_file = "{}/{}/{}.jpg".format(ssd_data_dir, filename, item)
                xml_file = "{}/{}/{}.xml".format(ssd_data_dir, filename, item)
                txt_file = "{}/{}/{}.xml.txt".format(ssd_data_dir, filename, item)
                os.remove(jpg_file)
                os.remove(xml_file)
                os.remove(txt_file)
                print("Delete => " + jpg_file)
                print("Delete => " + xml_file)
                print("Delete => " + txt_file) 


if __name__ == '__main__':

    right_file_dir = "/home/lsn/data/jst_new/new/right_files"
    ssd_data_dir = "/home/lsn/data/jst_new/new/1227-jst-end"

    del_error_files(right_file_dir, ssd_data_dir)