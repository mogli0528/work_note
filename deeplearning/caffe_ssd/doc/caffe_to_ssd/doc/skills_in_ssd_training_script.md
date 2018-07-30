# ssd 训练脚本中使用到的 skills  
1. 将字符串转换为 list    
```python
gpus = "0,1,2,3,4,5,6,7"
gpulist = gpus.split(",")
num_gpus = len(gpulist)
```
2. 两个比较好用的脚本函数     
判断目录是否存在,如果不存在则创建.   
```python
import os

def check_if_exist(path):
    return os.path.exists(path)

def make_if_not_exist(path):
    if not os.path.exists(path):
        os.makedirs(path)
```
3. int-float 强制类型转换
```python
per_device = int(math.ceil(float(batch_size) / num_gpus))
```
`math.ceil()`对浮点数向上取整.如果浮点数为 2.2, 则返回值为 3. 
`math.floor()`对浮点数向下取整.如果浮点数为 2.8, 则返回值为 2. 
4. 将字符串输出到文件中   
```python
from __future__ import print_function

model_name = r"googlenet.caffemodel"
file_ = r"solver.prototxt"

with open(file_, 'w') as f:
    print('name: "{}_test"'.format(model_name), file=f)  # print 形式
    f.write("hello\n")  # 正常形式
    f.write("world\n")
```
**Note**   
如果文件不存在,那么 print 会自动新建文件.   
5. 双引号 vs 单引号   
由于字符串中有可能既有双引号又有单引号，例如：    
`字符串：demo'1'`   
这时候就可以：str　= " demo'1' "；这样看更直观，当然也可以使用转义字符\'代替单引号：`str　= 'demo\'1\''`。    
`字符串：demo"1"`   
这时候就可以：str　= ' demo"1" '。   
6. 拷贝文件到文件夹中.   
```python
shutil.copy(train_net_file, job_dir)
```
将 train_net_file 文件拷贝到 job_dir/ 中.  
7. 判断文件后缀    
```python
if file.endswith(".caffemodel"):
	print (file)
```
8. 删除文件   
```python
os.remove("{}/{}".format(snapshot_dir, file))
```
9. 更改文件权限   
```python
os.chmod(job_file, stat.S_IRWXU)
```
10. 获取本文件的绝对路径   
```python
py_file = os.path.abspath(__file__)
```
11. 运行 shell 脚本(更改脚本的执行权限)    
```python
sh_file = "{}.sh".format(os.path.splitext(py_file)[0])

with open(sh_file, 'w') as f:
  f.write('echo $HOME\n')
 
os.chmod(sh_file, stat.S_IRWXU)
subprocess.call(sh_file, shell=True)
```
12. 以上所有技巧的示例脚本   
```python
#coding=utf-8
from __future__ import print_function  # python 3 的 print 形式

import os
import stat
import subprocess

model_name = r"googlenet.caffemodel"
file_ = r"solver.prototxt"
with open(file_, 'w') as f:
    print ('name: "{}_test"'.format(model_name), file=f)
    f.write("hello\n")
    f.write("world\n")

files = os.listdir(".")
for f in files:
	if f.endswith(".prototxt"):
		print (f)

py_file = os.path.abspath(__file__)
sh_file = "{}.sh".format(os.path.splitext(py_file)[0])

with open(sh_file, 'w') as f:
  f.write('echo $HOME\n')
 
os.chmod(sh_file, stat.S_IRWXU)
subprocess.call(sh_file, shell=True)
```
