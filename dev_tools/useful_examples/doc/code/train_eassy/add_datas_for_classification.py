#!/usr/bin/env python
#encoding=utf-8

'''
功能： 向现有的样本库中添加补录的数据
'''

add_data_dir = "data_add"
origin_data_dir = "data_origin"

import os
import shutil

def check_if_exist(path):
    return os.path.exists(path)

def make_if_not_exist(path):
    if not os.path.exists(path):
        os.makedirs(path)


def main(src_dir, dst_dir):
	classes = os.listdir(src_dir)
	for c in classes:
		c_dst = os.path.join(dst_dir, c)  # destination class directory
		c_full = os.path.join(src_dir, c)

		# check the origin directory
		if not check_if_exist(c_dst):
			print "new class added. class: %s." % c
			shutil.copytree(c_full, c_dst)
		
		for f in os.listdir(c_full):
			f_full = os.path.join(c_full, f)  # files in class c 
			# do copy 
			shutil.copy(f_full, c_dst)

if __name__ == '__main__':
	main(add_data_dir, origin_data_dir)
