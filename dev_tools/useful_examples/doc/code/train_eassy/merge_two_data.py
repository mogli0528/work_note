#coding=utf-8
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



filename = r"jst_before_check_list.txt" # txt文件和当前脚本在同一目录下，所以不用写具体路径
data_dir_a = r"/home/klm/jst_data"
data_dir_b = r"/media/klm/work/thinkStation/data/jst_data"
output_dir = r"/home/klm/dst_jst_data"
# classes_a = []
# classes_b = []


def read_classes_of_each_directory(txt_file):
	classes_a = []
	classes_b = []

	with open(txt_file, 'r') as f:
		lines = f.readlines() # 整行读取数据
		if not lines:
			print "read lines error.."
			sys.exit(1)
		# 将整行数据分割处理，如果分割符是空格，括号里就不用传入参数，如果是逗号， 则传入‘，'字符。
		if len(lines) >= 2:
			classes_a = lines[0].strip('\n').split(",")
			classes_b = lines[1].strip('\n').split(",")
	
	return classes_a, classes_b

def merge_data(classes_a, classes_b, data_dir_a, data_dir_b, output_dir):
	check_if_exists(data_dir_a)
	check_if_exists(data_dir_b)
	make_if_not_exists(output_dir)

	c_dst = ""
	# 拷贝 a 数据
	# for c_a in classes_a:
	# 	c_a_full = os.path.join(data_dir_a, c_a) 
	# 	c_dst = os.path.join(output_dir, c_a) 

	# 	if not os.path.exists(c_dst):
	# 		print "copy [a] %s ..." % c_a
	# 		shutil.copytree(c_a_full, c_dst)
	# 	else:
	# 		print "%s directory already exists" % c_dst

	for c_b in classes_b:
		c_b_full = os.path.join(data_dir_b, c_b) 
		c_dst = os.path.join(output_dir, c_b) 

		if not os.path.exists(c_dst):
			print "copy [b] %s ..." % c_b
			shutil.copytree(c_b_full, c_dst)
		else:
			print "%s directory already exists" % c_dst


if __name__ == '__main__':
	classes_a, classes_b = read_classes_of_each_directory(filename)
	merge_data(classes_a, classes_b, data_dir_a, data_dir_b, output_dir)
