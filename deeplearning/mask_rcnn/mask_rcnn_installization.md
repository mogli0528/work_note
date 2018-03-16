# ubuntu 14.04 配置   

## 提前下载的文件   
tensorflow 各种版本的下载地址: http://mirrors.nju.edu.cn/tensorflow/linux/gpu/   
- cudnn6: [https://developer.nvidia.com/compute/machine-learning/cudnn/secure/v6/prod/8.0_20170307/cudnn-8.0-linux-x64-v6.0-tgz](https://developer.nvidia.com/compute/machine-learning/cudnn/secure/v6/prod/8.0_20170307/cudnn-8.0-linux-x64-v6.0-tgz)    
- tensorflow1.3: [http://mirrors.nju.edu.cn/tensorflow/linux/gpu/](http://mirrors.nju.edu.cn/tensorflow/linux/gpu/), 根据不同的 python 版本选择下载.       
- 预训练的 coco 权重: [https://github.com/matterport/Mask_RCNN/releases](https://github.com/matterport/Mask_RCNN/releases).   
> 注意区分 python2 python3 对应的 pip 和 pip3 安装 package 的不同.   

## Requirements  
- Python 3.4+  
- TensorFlow 1.3+  
- Keras 2.0.8+  
- Jupyter Notebook  
- Numpy, skimage, scipy, Pillow, cython, h5py   

安装依赖库.  
```
sudo apt-get install gfortran 
sudo pip3 install Cython scikit-image keras h5py numpy 

# 安装 python3 jupyter 
sudo pip3 install --upgrade pip  ## 切记: 这行必不可少 
sudo pip3 install jupyter
```

## tensorflow 需要用到 cudnn.so.6 .   
解压之前下载好的 cudnn_v6, 然后执行以下命令拷贝库文件到系统目录:  
```
cp -HL /path/to/cudnn/lib64/libcudnn.so.6 /usr/lib/x86_64-linux-gnu/
cp -HL /path/to/cudnn/lib64/libcudnn.so.6.0.21 /usr/lib/x86_64-linux-gnu/
chmod a+r /usr/lib/x86_64-linux-gnu/libcudn*
```

## 下载 Mask_RCNN 源码   
``` 
git clone https://github.com/matterport/Mask_RCNN.git
```
该目录下包含以下功能:   

- Source code of Mask R-CNN built on FPN and ResNet101.  
- Training code for MS COCO  
- Pre-trained weights for MS COCO  
- Jupyter notebooks to visualize the detection pipeline at every step  
- ParallelModel class for multi-GPU training  
- Evaluation on MS COCO metrics (AP)  
- Example of training on your own dataset  

## cocoapi   
```
git clone  https://github.com/cocodataset/cocoapi.git
# 编译生成 coco 包   
cd cocoapi/PythonAPI/
python3 setup.py build_ext --inplace
cp pycocotools/ /path/to/Mash_RCNN/ -R 
# 然后切换到 Mask_RCNN 工程目录下  
cd /path/to/Mash_RCNN/pycocotools/
```
如果导包时提示符号错误(或者未定义), 则是由于 python2 / python3 版本的原因. 务必确认上一步中编译生成 coco 包时使用的是 `python3 setup.py build_ext --inplace` python3. 重新编译之后需要更新下面的动态链接库:    
```
mv _mask.so _mask.cpython-27m.so
cp _mask.cpython-34m.so _mask.so
```

## 测试 Mask_RCNN 安装是否完成.   
运行 `jupyter notebook` 后在浏览器中运行 `demo.ipynb` 文件.   

## 训练自己的数据集   
1. 下载 coco 数据集, 放到 ./data 目录中, 然后运行 python download.py 和 convert_data.py 来生成 tf-records, 这需要花费一段时间.   
2. 下载 resnet50 的预训练模型, wget http://download.tensorflow.org/models/resnet_v1_50_2016_08_28.tar.gz, 解压到 ./data/pretrained_models/ 目录下.   
3. 切换到 ./libs 目录下编译: `make -j` .   
4. run `python train/train.py` 开始训练.   

**Note:**   
可能会出现一些 Bugs.   

## 遇到的问题   
1. jupyter 安装完成后运行 jupyter notebook 时出现以下错误:    
```
klm@klm:~$ jupyter notebook
Traceback (most recent call last):
  File "/usr/local/bin/jupyter-notebook", line 7, in <module>
    from notebook.notebookapp import main
  ...
  ...
  File "/usr/local/lib/python3.4/dist-packages/tornado/gen.py", line 1295, in <module>
    _wrap_awaitable = asyncio.ensure_future
AttributeError: 'module' object has no attribute 'ensure_future'
```
**解决办法:**   
(1) 安装 python3 的 asyncio 包.   
```
sudo pip3 install asyncio
```
(2) 修改 /usr/local/lib/python3.4/dist-packages/tornado/gen.py 文件的 1295 行:  
```
wrap_awaitable = asyncio.ensure_future
# 将上面这行改为如下:   
_wrap_awaitable = asyncio.async   # edit by klm, it works...
```
