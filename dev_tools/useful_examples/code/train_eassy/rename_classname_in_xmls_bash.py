#!/usr/bin/env python
#coding=utf-8

'''
功能： 修改 Annotations/*.xml 文件中有错误的 class_name  
注意： 
    这个脚本只处理单个 bounding box 的情形,直接忽略多个 bounding box
      
changelog:

2018-01-08:
    1. 修改了只对 jstxxx 感兴趣的 class name.   
    2. 更正了 xml 中 class name 为 '' 的情况.   

'''

sh name = "modify_classname_in_xmls" 
sh_file = "{}.sh".format()

with open(sh_file, 'w') as f:
  f.write('echo $HOME\n')
 
os.chmod(sh_file, stat.S_IRWXU)
subprocess.call(sh_file, shell=True)