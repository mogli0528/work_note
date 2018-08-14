import os
import shutil

data_dir = "wanson_extra_lu_er_wei"
# new_data_dir = "nbx012"
# if not os.path.exists(new_data_dir):
# 	os.makedirs(new_data_dir)

files = os.listdir(data_dir)
for f in files:

	# pos = f.find("l")
	f_new = f[2:]
	print f_new
	# shutil.copy(os.path.join(data_dir, f), os.path.join(new_data_dir, f_new))
	os.rename(os.path.join(data_dir, f), os.path.join(data_dir, f_new))