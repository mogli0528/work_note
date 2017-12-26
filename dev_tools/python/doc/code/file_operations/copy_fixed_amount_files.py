#coding=utf-8
'''
功能： 从标注出来的数据中选取固定数量的样本图片
注意： 
      每次使用时修改 amount 的值,确保总样本数量大于 amount 
'''
import os
import shutil
import sys
import random
import time

xml_t_dir = r'xml'
jpg_t_dir = r'jpg'
src_dir = r'test'
amount = 400

def find_xmls_jpgs(src_directory):
	if not os.path.exists(src_directory):
		print "error: %s is not exist" % src_directory
		#os.exists(1)
		sys.exit(1)
		
	tmp_jpgs_xmls = {}
	jpgs_xmls = []
	
	dirs = os.listdir(src_directory)
	# 遍历各个类别
	for d in dirs:
		# 类别目录的全路径
		class_dir = os.path.join(src_directory, d)
		files = os.listdir(class_dir)

		for i in range(len(files)-1, -1, -1):
			# 每个文件的全路径
			file_dir = os.path.join(src_directory,d,files[i])
			if files[i].lower().endswith("jpg") or files[i].lower().endswith("xml") or files[i].lower().endswith("txt"):
				base,ext = os.path.splitext(file_dir)
				jpg = base + r'.jpg'
				xml = base + r'.xml'
				# print jpg, xml

				if os.path.exists(xml) and os.path.exists(jpg):
					if not xml in tmp_jpgs_xmls:
						tmp_jpgs_xmls[jpg] = xml
					
		# 打乱顺序
		if amount > len(tmp_jpgs_xmls):
			print "error: copy amount too large, %d .vs. %d" % (amount, len(tmp_jpgs_xmls))
			sys.exit(1)
		print len(tmp_jpgs_xmls)
		rand_list = tmp_jpgs_xmls.keys()
		random.shuffle(rand_list)

		jpgs_xmls += (rand_list[:amount])
		tmp_jpgs_xmls.clear()

	return jpgs_xmls

def copyfiles_to_target_dir(src_dir, jpg_dir, xml_dir):
	jpgs_xmls = find_xmls_jpgs(src_dir)
	print len(jpgs_xmls)

	if not os.path.exists(jpg_dir):
		os.mkdir(jpg_dir)	
	else:
		print "warning: directory exists: %s" % jpg_dir
	if not os.path.exists(xml_dir):
		os.mkdir(xml_dir)
	else:
		print "warning: directory exists: %s" % xml_dir
	count = 0
	# 抽取 600*num_class 张图片
	for i in range(len(jpgs_xmls)):		
		# basename = os.path.basename(jpgs_xmls[i])
		file_name, ext = os.path.splitext(jpgs_xmls[i])
		print file_name, ext
		jpg_file = file_name + '.jpg'
		xml_file = file_name + '.xml'
		# print jpgs_xmls[i][0], jpgs_xmls[i][1]
		try:
			count += 1
			shutil.copy(jpg_file, jpg_t_dir)
			shutil.copy(xml_file, xml_t_dir)
		except:
			print "copy error" + jpg_file
	print 'copy %d files, done...' % count

if __name__ == '__main__':
	copyfiles_to_target_dir(src_dir, jpg_t_dir, xml_t_dir)
