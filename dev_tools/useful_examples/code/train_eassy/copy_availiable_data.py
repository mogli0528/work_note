#coding=utf-8
'''
功能:
	提取低于jst047的类别的数据   
'''
import os
import sys
import shutil

def check_if_exists(path):
	if not os.path.exists(path):
		print "No such file or directory: %s" % path
		sys.exit(1)

def make_if_not_exists(path):
	if not os.path.exists(path):
		os.makedirs(path)

filename = r"jst_after_check_availible.txt" # txt文件和当前脚本在同一目录下，所以不用写具体路径
origin_dir = r"/home/klm/dst_jst_data"
output_dir = r"/home/klm/aviliable_jst_data"

def read_classes_to_copy(txt_file):
	classes = []

	with open(txt_file, 'r') as f:
		lines = f.readlines() # 整行读取数据
		if not lines:
			print "read lines error.."
			sys.exit(1)
		# 将整行数据分割处理，如果分割符是空格，括号里就不用传入参数，如果是逗号， 则传入‘，'字符。
		if len(lines) >= 1:
			classes = lines[0].strip('\n').split(",")
	
	return classes

def copy_data(classes, origin_dir, output_dir):
	check_if_exists(origin_dir)
	make_if_not_exists(output_dir)

	c_dst = ""
	
	for c in classes:
		c_full = os.path.join(origin_dir, c) 
		c_dst = os.path.join(output_dir, c) 
		c_index = int(c[3:])
		if c_index >= 47:
			if not os.path.exists(c_dst):
				print "copy %s ..." % c
				shutil.copytree(c_full, c_dst)
			else:
				print "%s directory already exists" % c_dst


if __name__ == '__main__':
	classes = read_classes_to_copy(filename)
	copy_data(classes, origin_dir, output_dir)
