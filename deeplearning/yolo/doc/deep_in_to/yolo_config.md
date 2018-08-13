## YOLO 实践篇 - darknet 安装, 训练  

继 R-CNN 系列的 object detection 之后， YOLO 是一种新的目标检测方法，该方法的特点是实现快速检测的同时还达到较高的准确率。作者将目标检测任务看作目标区域预测和类别预测的回归问题。该方法采用单个神经网络直接预测物品边界和类别概率，实现端到端的物体检测。同时，该方法检测速非常快， 基础版可以达到 45fps 的实时检测，Fast YOLO 可以达到 155fps。   

与当前最好系统相比，虽然 YOLO 网络模型检测得到的目标区域定位误差更大，但是背景预测的假阳性优于当前最好的方法（YOLO 采用全图信息来进行预测。与滑动窗口方法和 region proposal-based 方法不同，YOLO 在训练和预测过程中可以利用全图信息。Fast R-CNN 检测方法会错误的将背景中的斑块检测为目标， 原因在于 Fast R-CNN 在检测中无法看到全局图像。所以相对于 Fast R-CNN，YOLO背景预测错误率低一半）。  

## YOLO-v1 的主要特点  

1） 速度快，能够达到实时的要求。在 Titan X 的 GPU 上 能够达到 45 帧每秒。  
2） 使用全图作为 Context 信息，背景错误（把背景错认为物体）比较少。  
3） 泛化能力强：在自然图像上训练好的结果在艺术作品中的依然具有很好的效果。   

YOLO-v1 在 Pascal VOC-2012 数据集上训练，能识别如下 20 种物体类别：  

1）person  
2）bird, cat, cow, dog, horse, sheep   
3）aeroplane, bicycle, boat, bus, car, motorbike, train   
4）bottle, chair, dining table, potted plant, sofa, tv/monitor  

## darknet 的主要特点   

1）darknet 完全由 C 语言实现，没有任何依赖项。为了更好的可视化，可以使用 OpenCV 来显示图片；  
2）darknet 支持 CPU 和 GPU（CUDA/cuDNN）；  
3）因为较为轻型，没有像 TensorFlow 那般强大的 API， 有另一种味道的灵活性，更为方便的从底层对其进行改进与扩展；  
4）darknet 的实现与 caffe 的实现存在相似的地方，熟悉了 darknet，相信对上手 caffe 有帮助；  

本文不再叙述过多的理论知识， 主要目的是整理 YOLO 的实践过程，以 “理论+实践” 的方式学习 yolo。本文的实践部分主要参考 darknet（https://pjreddie.com/darknet/），一个基于C语言和CUDA的开源神经网络框架。此外，github 上还有一些开源的 tensorflow 版本，感兴趣的读者可以实践一下。

## YOLO 安装   

> 详细的安装过程请参考 darknet 官网：https://pjreddie.com/darknet/install/   

1） 本文实践环境：  

darknet 是由 C 和 CUDA 开发的， 不需要配置其他深度学习的框架（如，tensorflow、caffe 等）， 支持 CPU 和 GPU 运算，而且安装过程非常简单。本文使用的 CUDA 和 OpenCV 的版本如下所示： 

CUDA：8.0  
CUDNN：5.1-8.0  
OpenCV-3.3.0  

2） 下载 github 源码：  

git clone https://github.com/pjreddie/darknet.git  

3） 编译 darknet 

默认的 Makefile 是使用 CPU。如果你配置好了 CUDA 等相关环境，想使用 GPU 跑模型， 修改 Makefile 文件:   

~~~conf
GPU=1    # 使用 GPU
CUDNN=1  # 使用 CUDNN
OPENCV=1 # 使用 OPENCV
OPENMP=0
DEBUG=0
~~~

修改完成之后，根目录下次执行 make 命令进行编译，编译成功后提示信息如下所示：   

~~~bash
cd darknet
make -j8
~~~

说明：编译成功后会生成一个 darknet 的可执行文件，执行 ./darknet 就可以运行。

测试一下 OpenCV 是否安装并编译成功：  

./darknet imtest data/eagle.jpg

配置好 OpenCV 后，直接显示图像.   

## 实践过程中主要修改的文件   

1） Makefile   
2） ./cfg 目录下的参数配置文件  
3） ./data 目录下的数据  

## 实践步骤（文末附有问题集锦，不断更新）   

### 1. 基于 yolo.weights 模型参数测试单张图像
 
0) 下载预训练的模型参数 yolo.weights：    

~~~bash
wget https://pjreddie.com/media/files/yolov3.weights   
~~~

1） 基于 yolo.weights 模型参数测试单张图像：  

~~~bash
./darknet detect cfg/yolov3.cfg yolov3.weights data/dog.jpg   
~~~

2） 基于 yolov3.weights 模型参数测试多张图像（Multiple Images）   

不需要给定测试图像的路径，直接输入如下命令，然后程序会提示你输入测试图像路径，直到使用 “ctrl + C” 退出程序。  

~~~bash
./darknet detect cfg/yolov3.cfg yolov3.weights
~~~

3） 使用 “-thresh” 参数控制显示的 bounding-box 个数   

darknet 默认只显示被检测的物体中 confidence 大于或等于 0.25 的 bounding-box，可以通过参数 "-thresh <value>" 改变。   

例如，“-thresh 0” 表示显示出所有的 bounding-box。    

tiny yolo 是 yolo 的 fast 版本，网络结构有所改变（卷积层减少了），速度高但是牺牲了准确率。感兴趣的读者可以参考 darknet 官网。    

### 2. 基于 yolov3.weights 模型参数，使用网络摄像头（Webcam）进行实时检测    

1） 网络摄像头，如有多个摄像头可以使用 “-c <num>” 参数进行选择，默认参数为 0。  

2） CUDA 和 OpenCV  

万事俱备后，执行如下命令，你将看到当前的 FPS、框出物体的bounding-box、所属类别的概率。  

~~~bash
./darknet detector demo cfg/coco.data cfg/yolov3.cfg yolov3.weights
~~~

### 3. 基于 yolov3.weights 模型参数，测试一段视频（CUDA、OpenCV、Video），官网有提供一段检测的视频。   

~~~bash
./darknet detector demo cfg/coco.data cfg/yolov3.cfg yolov3.weights <video file>
~~~

### 4. 基于 darknet 在 VOC 数据集上训练 YOLO：   

参照博客: [Training YOLO on VOC](training_yolo_on_voc.md).  

### 5. 基于 darknet 在 COCO 数据集上训练 YOLO 

1）获取 COCO 数据集，在 darknet 根目录下执行如下命令：  

~~~bash
cp scripts/get_coco_dataset.sh data
cd data
bash get_coco_dataset.sh
~~~

2） 修改 “cfg/coco.data” 和 “cfg/yolo.cfg” 配置文件  

首先修改 “./cfg/coco.data” 文件:   

~~~conf

classes= 80
train  = /home/hcq/document/deepLearning/github/darknet/data/coco/trainvalno5k.txt
#valid  = coco_testdev
valid = /home/hcq/document/deepLearning/github/darknet/data/coco/5k.txt
names = data/coco.names
backup = backup
eval=coco

然后修改 “./cfg/yolov3.cfg” 文件:   

~~~conf
[net]
# Testing
# batch=1
# subdivisions=1
# Training
batch=64
subdivisions=8
....
~~~

3） 训练模型：

~~~bash
./darknet detector train cfg/coco.data cfg/yolov3.cfg darknet19_448.conv.23
~~~

## YOLO 论文细节

《You only look once: Unified, Real-Time Object Detection》 原文中还提到很多其他 Detection System，如下：  

1）Deformable parts models（DPM）：在深度神经网络之前，早期的 Object detection 方法是通过提取图像的一些 robust 的特征如（ Haar，SIFT，HOG ）等特征，使用 DPM 模型，用 silding window 的方式来预测具有较高 score 的 bounding box。DPM 模型把物体看成了多个组成的部件（比如人脸的鼻子、嘴巴等），用部件间的关系来描述物体，这个特性非常符合自然界很多物体的非刚体特征。DPM可以看做是HOG+SVM的扩展，很好的继承了两者的优点，在人脸检测、行人检测等任务上取得了不错的效果，但是DPM相对复杂，检测速度也较慢，从而也出现了很多改进的方法。正当大家热火朝天改进DPM性能的时候，基于深度学习的目标检测横空出世，迅速盖过了DPM的风头，很多之前研究传统目标检测算法的研究者也开始转向深度学习。（更详细的理论知识可以参考这篇 博文）  

Felzenszwalb P F, Girshick R B, McAllester D, et al. Object detection with discriminatively trained part-based models[J]. Pattern Analysis and Machine Intelligence, IEEE Transactions on, 2010, 32(9): 1627-1645.  

2）R-CNN Series：包括 R-CNN、SPP-net、Fast R-CNN、Faster R-CNN、R-FCN 等经典的网络模型。  

[1] R. Girshick, J. Donahue, T. Darrell, J. Malik. Rich feature hierarchies for accurate object detection and semantic segmentation. ImageNet Large-Scale Visual Recognition Challenge workshop, ICCV, 2013.  
[2] R. Girshick, J. Donahue, T. Darrell, J. Malik. Rich feature hierarchies for accurate object detection and semantic segmentation. IEEE Conference on Computer Vision and Pattern Recognition (CVPR), 2014.  
[3] R. Girshick, J. Donahue, T. Darrell, J. Malik. Region-Based Convolutional Networks for Accurate Object Detection and Segmentation. IEEE Transactions on Pattern Analysis and Machine Intelligence, May. 2015.  
[4] K. He, X. Zhang, S. Ren, and J. Sun. Spatial pyramid pooling in deep convolutional networks for visual recognition. In ECCV. 2014.  
[5] S. Ren, K. He, R. Girshick, J. Sun. Faster R-CNN: Towards Real-Time Object Detection with Region Proposal Networks. Advances in Neural Information Processing Systems 28 (NIPS), 2015.  

3）Deep MultiBox：Szegedy et al. train a convolutional neural network to predict regions of interest instead of using Selective Search. （Szegedy 等人训练一个卷积神经网络来预测感兴趣的区域，而不是使用 SelectiveSearch 算法直接获得候选区）YOLO 也是通过训练一个 CNN 预测 bounding-box。相对 MultiBox，YOLO 是一个完整的检测系统。  

D. Erhan, C. Szegedy, A. Toshev, and D. Anguelov. Scalable object detection using deep neural networks. In Computer Vision and Pattern Recognition (CVPR), 2014 IEEE Conference on, pages 2155–2162. IEEE, 2014.  

4）OverFeat：用CNN统一来做分类、定位和检测的经典之作，作者是深度学习大神之一，Yann Lecun。此外，OverFeat 也是 ILSVRC 2013 任务3（分类+定位）的冠军得主。OverFeat 使用 CNN 做目标检测的早期工作，主要思想是采用了多尺度滑动窗口来做分类、定位和检测。虽然是多个任务但重用了模型前面几层，这种模型重用的思路也是后来 R-CNN 系列不断沿用和改进的经典做法。OverFeat 可以看做是 R-CNN的一个特殊情况，只需要把 Selective Search 换成多尺度的滑动窗口，每个类别的边框回归器换成统一的边框回归器，SVM 换为多层网络即可。但是 OverFeat 实际比R-CNN快9倍，这主要得益于卷积相关的共享计算。  

P. Sermanet, D. Eigen, X.Zhang, M. Mathieu, R. Fergus, and Y. LeCun. OverFeat: Integrated recognition, localization and detection using convolutional networks. In ICLR, 2014.  


5）MultiGrasp：YOLO 的设计与 MultiGrasp 相似，2014年 Redmon 等人发表了这篇文章。MultiGrasp 主要用于 grasp detection，一个比 object detection 简单很多的任务。对于一张包含单个物体的 image，MultiGrasp 只需要预测一个单一的 graspable region，而不需要预估 object 的大小、位置或者边界，也不需要预测它属于哪个类别。相较之下 YOLO 的任务复杂得多，对于每个 grid cell 需要预测 B 个 bounding-box 以及 P（class[i] | object），对于每个 bounding-box 需要回归（x, y, w, h）和置信度（confidence），而且还是多目标、多类别的图像。  
 
J. Redmon and A. Angelova. Real-time grasp detection using convolutional neural networks. CoRR, abs/1412.3128, 2014  


## 参考资料

- yolo 实践参考资料    

yolo（darknet）官网：https://pjreddie.com/darknet/yolo/   
github 源码：https://github.com/pjreddie/darknet  
YOLO源码解析（tensorflow）：https://zhuanlan.zhihu.com/p/25053311  
 
- yolo 理论参考资料  

YOLO 原文链接：https://arxiv.org/pdf/1506.02640.pdf  
基于深度学习的目标检测（附有参考文献）：https://www.cnblogs.com/gujianhan/p/6035514.html  
YOLO - 实时快速目标检测：https://zhuanlan.zhihu.com/p/25045711?refer=shanren7  

- darknet 源码解析：  

github 地址：https://github.com/hgpvision/darknet  

- keras 版本的 yolo-v2  

GitHub 地址：https://github.com/allanzelener/YAD2K  


## 问题集锦

### 1. OpenCV：to the PKG_CONFIG_PATH environment variable   

参考网址：http://blog.csdn.net/xuesen_lin/article/details/6124688  

原因：没有配置 PKG_CONFIG_PATH 环境。缺省情况下，pkg-config首先在prefix/lib/pkgconfig/中查找相关包（譬如opencv）对应的相应的文件（opencv.pc）。在linux上上述路径名为 /usr/lib/pkconfig/。若是没有找到，它也会到PKG_CONFIG_PATH这个环境变量所指定的路径下去找。若是没有找到，它就会报错。  

解决方案：找到 opencv.pc 所在的目录，正常情况下是在如下路径：./opencv/lib/pkgconfig, opencv 的编译请参考：http://blog.csdn.net/houchaoqun_xmu/article/details/78565077.   

然后打开环境变量的配置文件，将路径加上去，一系列命令如下所示：   

~~~bash
sudo gedit ~/.bashrc  # 打开环境变量配置文件

export PKG_CONFIG_PATH=/home/lab406/hcq/opencv/lib/pkgconfig:$PKG_CONFIG_PATH  # 现在是使用这个路径，可行
export PKG_CONFIG_PATH=/home/lab406/hcq/opencv/lib:$PKG_CONFIG_PATH  # 之前是使用这个路径，但不行
source ~/.bashrc  # 生效配置文件的修改
~~~

### 2. darknet: ./src/cuda.c:36: check_error: Assertion `0' failed  

CUDNN 忘记设置为 1，出现错误：  

~~~bash
mask_scale: Using default '1.000000'
Loading weights from darknet19_448.conv.23...Done!
Learning Rate: 0.001, Momentum: 0.9, Decay: 0.0005
Resizing
576
Loaded: 0.031443 seconds
Region Avg IOU: 0.242979, Class: 0.011581, Obj: 0.442039, No Obj: 0.477931, Avg Recall: 0.066667,  count: 30
CUDA Error: mapping of buffer object failed
darknet: ./src/cuda.c:36: check_error: Assertion `0' failed.
Aborted (core dumped)
hcq@hcq-home:~/document/deepLearning/github/darknet$ 
~~~

修改 Makefile 文件，将 CUDNN 设置为 1，重新编译即可。   


### 3. darknet: ./src/cuda.c:21: check_error: Assertion `0' failed.   

这种情况网上比较多类似的问题，主要都是说 “ARCH= --gpu-architecture=compute_xx  --gpu-code=compute_xx” 的问题。直接对应自己的显卡型号到网上搜搜看就有答案了。     

