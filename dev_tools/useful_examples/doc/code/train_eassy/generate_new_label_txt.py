#coding=utf-8

import os
import sys
	
def check_if_exists(path):
	if not os.path.exists(path):
		print "No such file or directory: %s" % path
		sys.exit(1)

def make_if_not_exists(path):
	if not os.path.exists(path):
		os.makedirs(path)


def read_items(ref_lable_txt_file):
	items = []
	with open(ref_lable_txt_file, "r") as f:
		lines = f.readlines()
		for l in lines:
			items.append(l.strip('\n'))

	return items

def generate_new_txt(ref_lable_txt_file, old_txt_file, new_txt_file):
	items = read_items(ref_lable_txt_file)
	print items

	f_new = open(new_txt_file, "a")
	if os.path.exists(new_txt_file):
		f_new = open(new_txt_file, "w")

	with open(old_txt_file, "r") as f_old:
		lines = f_old.readlines()
		for l in lines:
			class_name = l.strip('\n').split("|")[0]
			print class_name
			if class_name == "background":
				print class_name
			if class_name in items:
				f_new.write(l)
	f_new.close()



if __name__ == '__main__':

	new_txt_file = r"labels_new.txt"
	old_txt_file = r"labels.txt"
	ref_lable_txt_file = r"availiable_jst_data-labels.txt"

	check_if_exists(old_txt_file)
	check_if_exists(ref_lable_txt_file)



	generate_new_txt(ref_lable_txt_file,old_txt_file, new_txt_file)