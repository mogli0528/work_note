#!/usr/bin/env python
#coding=utf-8
'''
功能： 1. 使用新标注的样本完全替换原来的样本
	  2. rename
'''
import os
import shutil

# 存放分类数据的目录
add_items_dir = "/home/klm/data/jst_59_add"
origin_dir = "/home/klm/data/jst_271"
target_dir = "/home/klm/data/jst019"

def remove_and_copy(add_items_dir, origin_dir):
	files = os.listdir(add_items_dir)
	for f in files:
		# 删除原来的错误样本数据
		f_remove = os.path.join(origin_dir, f) 
		if os.path.exists(f_remove):
			shutil.rmtree(f_remove)
		# f = os.path.join(add_items_dir, f)
		# 将新准备好的数据拷贝过去
		shutil.copytree(os.path.join(add_items_dir, f), "{}/{}".format(origin_dir,f))
		print "copying %s ..." % f 


def rename(target_dir):
	files = os.listdir(target_dir)
	for f in files:
		if f.find("jstt") != -1:
			# jstt1970_02860 -> jstt1970_02860
			old_name = os.path.join(target_dir, f)
			new_name = "jst" + f[4:]
			new_name = os.path.join(target_dir, new_name)
			os.rename(old_name, new_name)
			print old_name + " => " + new_name

if __name__ == '__main__':
	pass
