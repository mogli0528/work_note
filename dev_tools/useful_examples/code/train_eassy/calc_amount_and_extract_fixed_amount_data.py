#coding=gbk

import os
import random
import shutil

def mkdir_if_not_exists(path):
	if not os.path.exists(path):
		os.makedirs(path)

data_dir = "roi"
origin_dir = "error"
dst_dir = "dst_"
copy_amount = 1321

def calc_amount(data_dir):
	c_amount = {}

	classes = os.listdir(data_dir)

	for c in classes:
		c_full = os.path.join(data_dir, c)
		amount_of_c = len(os.listdir(c_full))
		# if c not in c_amount:
		c_amount[c] = amount_of_c

	return c_amount

def extract_fix_amount(origin_dir, dst_dir):
	
	classes = os.listdir(origin_dir)
	files_of_class = {}


	for c in classes:
		copy_files = []
		c_full = os.path.join(origin_dir, c)
		c_dst = os.path.join(dst_dir, c)
		mkdir_if_not_exists(c_dst)

		files = os.listdir(c_full)
		for f in files:
			copy_files.append(os.path.join(c_full, f))

		if len(copy_files) >= copy_amount:
			random.shuffle(copy_files)
			copy_files = copy_files[:copy_amount]
		
		for f in copy_files:
			shutil.copy(f, c_dst)

		# if c not in c_amount:
		# 	files_of_class[c] = copy_files


	

if __name__ == '__main__':
	# c_amount = calc_amount(data_dir)

	# c_amount = sorted(c_amount.items(), lambda x, y: cmp(x[1], y[1]))
	# print  c_amount 
	# for a in c_amount:
	# 	print a
	extract_fix_amount(origin_dir, dst_dir)

