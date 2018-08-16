# COCO 数据集 / API 的使用

## COCO 数据集简介

微软发布的 COCO 数据集, 除了图片以外还提供物体检测, 分割(segmentation)和对图像的语义文本描述信息.   

COCO 数据集的网址是:   

MS COCO API - http://mscoco.org/   

Github网址 - https://github.com/pdollar/coco   

关于API更多的细节在网站: http://mscoco.org/dataset/#download   

### 1. 高效下载 coco 数据集  

这里介绍一种不会中断的下载方法. 使用 aria2 命令行下载。   

需要先安装：

~~~bash
sudo apt-get install aria2
~~~

进入存放 COCO 数据集的目录，依次输入下面 3 个命令下载：  

~~~bash
aria2c -c http://msvocds.blob.core.windows.net/annotations-1-0-3/instances_train-val2014.zip 
aria2c -c http://msvocds.blob.core.windows.net/coco2014/train2014.zip 
aria2c -c http://msvocds.blob.core.windows.net/coco2014/val2014.zip 
~~~

以上 3 个 url 链接分别为 2014 年的 annotations、train data、val data.  

### 2. coco 2017 数据集下载链接

各个链接的意思: 第一组是 train 数据，第二组是 val 验证数据集，第三组是 test 验证数据集。  

数据包括物体检测和 keypoints 身体关键点的检测。  

~~~html
http://images.cocodataset.org/zips/train2017.zip
http://images.cocodataset.org/annotations/annotations_trainval2017.zip

http://images.cocodataset.org/zips/val2017.zip
http://images.cocodataset.org/annotations/stuff_annotations_trainval2017.zip

http://images.cocodataset.org/zips/test2017.zip
http://images.cocodataset.org/annotations/image_info_test2017.zip
~~~

这些就是全部的 microsoft coco 数据集 2017 的链接了。  

数据集提供 Matlab, Python 和 Lua 的 API 接口. 其中 matlab 和 python 的 API 接口可以提供完整的图像标签数据的加载, parsing 和可视化.此外,网站还提供了数据相关的文章, 教程等.

## 2. COCO Python API

The COCO API assists in loading, parsing, and visualizing annotations in COCO. The API supports multiple annotation formats (please see the data format page). For additional details see: CocoApi.m, coco.py, and CocoApi.lua for Matlab, Python, and Lua code, respectively, and also the Python API demo.

Throughout the API "ann"=annotation, "cat"=category, and "img"=image.   

getAnnIds Get ann ids that satisfy given filter conditions.   
getCatIds Get cat ids that satisfy given filter conditions.   
getImgIds Get img ids that satisfy given filter conditions.   
loadAnns Load anns with the specified ids.   
loadCats Load cats with the specified ids.   
loadImgs Load imgs with the specified ids.  
loadRes Load algorithm results and create API for accessing them.   
showAnns Display the specified annotations.  

在使用 COCO 数据集提供的 API 和 demo 时, 需要首先下载 COCO 的图像和标签数据.   

安装:
首先解压数据文件:
图像数据下载到 coco/images/ 文件夹中
标签数据下载到 coco/ 文件夹中.
matlab, 在 matlab 的默认路径中添加 coco/MatlabApi
Python. 打开终端,将路径切换到 coco/PythonAPI下,输入 make


import sys
sys.path.append('E:/xinlib')
from data import cocox
import zipfile
查看 coco/images/ 文件夹下的数据：

image_names = cocox.get_image_names()
image_names
['E:/Data/coco/images/test2017.zip',
 'E:/Data/coco/images/train2017.zip',
 'E:/Data/coco/images/unlabeled2017.zip',
 'E:/Data/coco/images/val2017.zip']
查看 coco/ 文件夹的文件：

import os
dataDir = cocox.root
os.listdir(dataDir)
['annotations',
 'annotations_trainval2017.zip',
 'cocoapi',
 'images',
 'image_info_test2017.zip',
 'image_info_unlabeled2017.zip',
 'stuff_annotations_trainval2017.zip']
我们只需要获取 annotations 的信息（这里都是以 .zip 结尾）：

annDir = [z_name for z_name in os.listdir(dataDir) if z_name.endswith('.zip')]
annDir
['annotations_trainval2017.zip',
 'image_info_test2017.zip',
 'image_info_unlabeled2017.zip',
 'stuff_annotations_trainval2017.zip']
解压 annotations 的文件：

for ann_name in annDir:
    z = zipfile.ZipFile(dataDir + '/' + ann_name)
    # 全部解压
    z.extractall(dataDir)
# 封装为函数
cocox.unzip_annotations()
# 删除标签的压缩文件
cocox.del_annotations()
由于图片数据比较大，我就不解压了，不过可以通过 MXNet + zipfile 来直接获取图片信息。

获取图片数据
我以 test2017.zip 为例：

image_names
['E:/Data/coco/images/test2017.zip',
 'E:/Data/coco/images/train2017.zip',
 'E:/Data/coco/images/unlabeled2017.zip',
 'E:/Data/coco/images/val2017.zip']
z = zipfile.ZipFile(image_names[0])
# 测试集的图片名称列表
z.namelist()
['test2017/',
 'test2017/000000259564.jpg',
 'test2017/000000344475.jpg',
 ...]
我们可以看出，第一个是目录名，之后的才是图片。下面我们来看看第一张图片：

from mxnet import image
r = z.read(z.namelist()[1])    # bytes
data = image.imdecode(r)       # 转换为 NDArray 数组，可以做数值运算
data
[[[ 87  94  78]
  [ 85  94  77]
  [ 87  96  79]
  ..., 
  [108  63  44]
  [252 244 233]
  [253 253 253]]

 [[ 86  95  76]
  [ 88  97  78]
  [ 85  94  75]
  ..., 
  [ 55  14   0]
  [150  94  81]
  [252 245 216]]

 [[ 90  99  78]
  [ 89  98  77]
  [ 89  98  77]
  ..., 
  [ 63  37  12]
  [ 90  30   6]
  [149  83  61]]

 ..., 
 [[ 86 104  82]
  [ 89 102  82]
  [ 84 102  80]
  ..., 
  [ 50  62  40]
  [ 50  61  45]
  [ 51  58  50]]

 [[ 89 101  77]
  [ 87  96  75]
  [ 89 104  83]
  ..., 
  [ 54  63  42]
  [ 49  53  39]
  [ 53  54  48]]

 [[ 96 100  77]
  [ 94  97  76]
  [ 88 103  82]
  ..., 
  [ 44  58  32]
  [ 45  57  37]
  [ 49  57  42]]]
<NDArray 480x640x3 @cpu(0)>
x = data.asnumpy()   # 转换为 array
# 显示图片
%pylab inline 
plt.imshow(x)
output_21_3.png-125.1kB

为此，我们可以将其封装为一个迭代器：cocox.data_iter(dataType)

获取标签信息（利用官方给定教程）
安装 python API：
pip install -U pycocotools
Windows （一般需要安装 visual studio）下有许多的坑：Windows 10 编译 Pycocotools 踩坑记

%pylab inline
from pycocotools.coco import COCO
import numpy as np
import skimage.io as io
import matplotlib.pyplot as plt
import pylab
pylab.rcParams['figure.figsize'] = (8.0, 10.0)
这里有一个坑 (由 PIL 引发) import skimage.io as io 在 Windows 下可能会报错，我的解决办法是：

先卸载 Pillow，然后重新安装即可。

插曲：PIL(Python Imaging Library)是Python一个强大方便的图像处理库，名气也比较大。Pillow 是 PIL 的一个派生分支，但如今已经发展成为比 PIL 本身更具活力的图像处理库。

dataDir = cocox.root
dataType = 'val2017'
annFile = '{}/annotations/instances_{}.json'.format(dataDir, dataType)
# initialize COCO api for instance annotations
coco=COCO(annFile)
loading annotations into memory...
Done (t=0.93s)
creating index...
index created!
COCO??
COCO 是一个类：

Constructor of Microsoft COCO helper class for reading and visualizing annotations.
:param annotation_file (str): location of annotation file
:param image_folder (str): location to the folder that hosts images.
display COCO categories and supercategories
cats = coco.loadCats(coco.getCatIds())
nms = [cat['name'] for cat in cats]
print('COCO categories: \n{}\n'.format(' '.join(nms)))

nms = set([cat['supercategory'] for cat in cats])
print('COCO supercategories: \n{}'.format(' '.join(nms)))
COCO categories: 

~~~
person bicycle car motorcycle airplane bus train truck boat traffic light fire hydrant stop sign parking meter bench bird cat dog horse sheep cow elephant bear zebra giraffe backpack umbrella handbag tie suitcase frisbee skis snowboard sports ball kite baseball bat baseball glove skateboard surfboard tennis racket bottle wine glass cup fork knife spoon bowl banana apple sandwich orange broccoli carrot hot dog pizza donut cake chair couch potted plant bed dining table toilet tv laptop mouse remote keyboard cell phone microwave oven toaster sink refrigerator book clock vase scissors teddy bear hair drier toothbrush
~~~

COCO supercategories: 
appliance sports person indoor vehicle food electronic furniture animal outdoor accessory kitchen
# get all images containing given categories, select one at random
catIds = coco.getCatIds(catNms=['person', 'dog', 'skateboard'])
imgIds = coco.getImgIds(catIds=catIds)
imgIds = coco.getImgIds(imgIds=[335328])
img = coco.loadImgs(imgIds[np.random.randint(0, len(imgIds))])[0]
img
{'license': 4,
 'file_name': '000000335328.jpg',
 'coco_url': 'http://images.cocodataset.org/val2017/000000335328.jpg',
 'height': 640,
 'width': 512,
 'date_captured': '2013-11-20 19:29:37',
 'flickr_url': 'http://farm3.staticflickr.com/2079/2128089396_ddd988a59a_z.jpg',
 'id': 335328}
官方给的这个代码需要将图片数据集解压：

# load and display image
# use url to load image
# I = io.imread(img['coco_url'])
I = io.imread('%s/images/%s/%s' % (dataDir, dataType, img['file_name']))
plt.axis('off')
plt.imshow(I)
plt.show()
我们可以使用 zipfile 模块直接读取图片，而无须解压：

image_names[-1]
'E:/Data/coco/images/val2017.zip'
val_z = zipfile.ZipFile(image_names[-1])
I = image.imdecode(val_z.read('%s/%s' % (dataType, img['file_name']))).asnumpy()
plt.axis('off')
plt.imshow(I)
plt.show()
output_36_0.png-493.1kB

load and display instance annotations
plt.imshow(I)
plt.axis('off')
annIds = coco.getAnnIds(imgIds=img['id'], catIds=catIds, iscrowd=None)
anns = coco.loadAnns(annIds)
coco.showAnns(anns)
output_38_0.png-491.6kB

initialize COCO api for person keypoints annotations
annFile = '{}/annotations/person_keypoints_{}.json'.format(dataDir, dataType)
coco_kps = COCO(annFile)
loading annotations into memory...
Done (t=0.43s)
creating index...
index created!
load and display keypoints annotations
plt.imshow(I)
plt.axis('off')
ax = plt.gca()
annIds = coco_kps.getAnnIds(imgIds=img['id'], catIds=catIds, iscrowd=None)
anns = coco_kps.loadAnns(annIds)
coco_kps.showAnns(anns)
output_42_0.png-491kB

initialize COCO api for caption annotations
annFile = '{}/annotations/captions_{}.json'.format(dataDir, dataType)
coco_caps = COCO(annFile)
loading annotations into memory...
Done (t=0.06s)
creating index...
index created!
load and display caption annotations
annIds = coco_caps.getAnnIds(imgIds=img['id'])
anns = coco_caps.loadAnns(annIds)
coco_caps.showAnns(anns)
plt.imshow(I)
plt.axis('off')
plt.show()
A couple of people riding waves on top of boards.
a couple of people that are surfing in water
A man and a young child in wet suits surfing in the ocean.
a man and small child standing on a surf board  and riding some waves
A young boy on a surfboard being taught to surf.

## 编译 COCO Python API 遇到的问题  

### 1. error: pycocotools/_mask.c: No such file or directory

错误来自编译 cocoapi PythonAPI. 解决办法是安装 cython 后再编译.   
　
~~~bash
sudo pip3 install cython
make -j8
~~~
