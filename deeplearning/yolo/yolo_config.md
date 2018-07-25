## YOLO 实践篇（darknet）：You Only Look Once  

最近在学习 object detection 领域的相关知识，从传统的目标检测如 DPM 模型，到基于候选区的R-CNN系列模型，再到 single network && end to end 的 yolo 模型，还有针对 yolo 模型的缺陷进一步完善的 SSD 模型等等。一系列的理论和实践需要学习，希望能够记录学习过程中的点点滴滴，不断修改、完善和总结，系统的学习一遍。当然，最后还是要付诸于实践，才能更深入的掌握相关知识。



继 R-CNN 系列的 object detection 之后， YOLO 是一种新的目标检测方法，该方法的特点是实现快速检测的同时还达到较高的准确率。作者将目标检测任务看作目标区域预测和类别预测的回归问题。该方法采用单个神经网络直接预测物品边界和类别概率，实现端到端的物体检测。同时，该方法检测速非常快，基础版可以达到 45fps 的实时检测，Fast YOLO 可以达到 155fps。与当前最好系统相比，虽然 YOLO 网络模型检测得到的目标区域定位误差更大，但是背景预测的假阳性优于当前最好的方法（YOLO 采用全图信息来进行预测。与滑动窗口方法和 region proposal-based 方法不同，YOLO 在训练和预测过程中可以利用全图信息。Fast R-CNN 检测方法会错误的将背景中的斑块检测为目标，原因在于Fast R-CNN在检测中无法看到全局图像。所以相对于Fast R-CNN，YOLO背景预测错误率低一半）。



## YOLO-v1 的主要特点  

1）速度快，能够达到实时的要求。在 Titan X 的 GPU 上 能够达到 45 帧每秒。
2）使用全图作为 Context 信息，背景错误（把背景错认为物体）比较少。
3）泛化能力强：在自然图像上训练好的结果在艺术作品中的依然具有很好的效果。 

YOLO-v1 在 Pascal VOC-2012 数据集上训练，能识别如下 20 种物体类别：

1）person

2）bird, cat, cow, dog, horse, sheep 

3）aeroplane, bicycle, boat, bus, car, motorbike, train 

4）bottle, chair, dining table, potted plant, sofa, tv/monitor

## darknet 的主要特点  

1）darknet 完全由C语言实现，没有任何依赖项。为了更好的可视化，可以使用 OpenCV 来显示图片；
2）darknet 支持 CPU 和 GPU（CUDA/cuDNN）；
3）因为较为轻型，没有像TensorFlow那般强大的API，有另一种味道的灵活性，更为方便的从底层对其进行改进与扩展；
4）darknet 的实现与 caffe 的实现存在相似的地方，熟悉了darknet，相信对上手caffe有帮助；

《You only look once: Unified, Real-Time Object Detection》原文 中还提到很多其他 Detection System，如下：

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

本文不再叙述过多的理论知识，主要目的是整理 YOLO 的实践过程，以“理论+实践”的方式学习 yolo。本文的实践部分主要参考darknet（https://pjreddie.com/darknet/），一个基于C语言和CUDA的开源神经网络框架。此外，github 上还有一些开源的tensorflow 版本，感兴趣的读者可以实践一下。

## Reference

- yolo实践参考资料

yolo（darknet）官网：https://pjreddie.com/darknet/yolo/

github 源码：https://github.com/pjreddie/darknet

YOLO源码解析（tensorflow）：https://zhuanlan.zhihu.com/p/25053311

- yolo理论参考资料

YOLO 原文链接：https://arxiv.org/pdf/1506.02640.pdf

基于深度学习的目标检测（附有参考文献）：https://www.cnblogs.com/gujianhan/p/6035514.html

YOLO - 实时快速目标检测：https://zhuanlan.zhihu.com/p/25045711?refer=shanren7

You Only Look Once: Unified, Real-Time Object Detection

- darknet 源码解析：

github 地址：https://github.com/hgpvision/darknet

- keras 版本的yolo-v2

GitHub 地址：https://github.com/allanzelener/YAD2K

## YOLO 实践   

准备工作

1）本文实践环境：

darknet 是由 C 和 CUDA 开发的，不需要配置其他深度学习的框架（如，tensorflow、caffe 等），支持 CPU 和 GPU 运算，而且安装过程非常简单。本文使用的 CUDA 和 OpenCV 的版本如下所示：

CUDA：8.0
CUDNN：5.1-8.0
OpenCV-3.3.0

如果还没搭建好环境的读者请参考如下博文：

Ubuntu 常用软件安装：Matlab + XX-net + ... + TeamViewer + Tensorflow + Caffe

Ubuntu-16.06 + OpenCV-3.3.0 + 一些常见的问题

2）下载 github 源码：

git clone https://github.com/pjreddie/darknet.git

3）进入 darknet 目录并编译：
cd darknet
make
说明：编译成功后会生成一个 darknet 的可执行文件，执行 ./darknet 就可以运行。你可以通过修改 Makefile 的参数，切记，每次修改完 Makefile 文件后记得重新 make 一下才能生效。
如果编译过程执行 make 时，出现了如下错误：

~~~bash
lab406@lab406-yang:~/hcq/deep_learning/github/darknet$ make
gcc -Iinclude/ -Isrc/ -DOPENCV `pkg-config --cflags opencv`  -DGPU -I/usr/local/cuda/include/ -DCUDNN  -Wall -Wno-unknown-pragmas -Wfatal-errors -fPIC -Ofast -DOPENCV -DGPU -DCUDNN -c ./src/gemm.c -o obj/gemm.o
Package opencv was not found in the pkg-config search path.
Perhaps you should add the directory containing `opencv.pc'
to the PKG_CONFIG_PATH environment variable
No package 'opencv' found
In file included from ./src/utils.h:5:0,
                 from ./src/gemm.c:2:
include/darknet.h:25:43: fatal error: opencv2/highgui/highgui_c.h: 没有那个文件或目录
compilation terminated.
Makefile:85: recipe for target 'obj/gemm.o' failed
make: *** [obj/gemm.o] Error 1
~~~

解决方案（详情请参考文末的“错误集锦”）：   

~~~bash
sudo gedit ~/.bashrc  # 打开环境变量配置文件
# added by hcq 20171206 opencv
export PKG_CONFIG_PATH=/home/lab406/hcq/opencv/lib/pkgconfig:$PKG_CONFIG_PATH  # 现在是使用这个路径，可行
export PKG_CONFIG_PATH=/home/lab406/hcq/opencv/lib:$PKG_CONFIG_PATH  # 之前是使用这个路径，但不行
source ~/.bashrc  # 生效配置文件的修改
~~~

默认的 Makefile 是使用 CPU。如果你配置好了 CUDA 等相关环境，想使用 GPU 跑模型，那么修改 Makefile 如下：

~~~makefile
GPU=1  # 从 0 修改为 1
CUDNN=1  # 从 0 修改为 1
OPENCV=1  # 从 0 修改为 1
OPENMP=0
DEBUG=0
~~~

修改完成之后，根目录下再次执行 make 命令进行编译，编译成功后提示信息如下所示（CUDA 已经编译好了）：   

~~~
### 省略一大堆 warning
/home/hcq/document/opencv/include/opencv2/videoio/videoio_c.h:547:15: warning: ‘CV_FOURCC’ defined but not used [-Wunused-function]
 CV_INLINE int CV_FOURCC(char c1, char c2, char c3, char c4)
               ^
gcc -Iinclude/ -Isrc/ -DOPENCV `pkg-config --cflags opencv`  -DGPU -I/usr/local/cuda-8.0/include/ -DCUDNN  -Wall -Wno-unknown-pragmas -Wfatal-errors -fPIC -Ofast -DOPENCV -DGPU -DCUDNN obj/captcha.o obj/lsd.o obj/super.o obj/art.o obj/tag.o obj/cifar.o obj/go.o obj/rnn.o obj/segmenter.o obj/regressor.o obj/classifier.o obj/coco.o obj/yolo.o obj/detector.o obj/nightmare.o obj/attention.o obj/darknet.o libdarknet.a -o darknet -lm -pthread  `pkg-config --libs opencv`  -L/usr/local/cuda-8.0/lib64 -lcuda -lcudart -lcublas -lcurand -lcudnn -lstdc++  libdarknet.a
~~~

详细的安装过程请参考 darknet 官网：https://pjreddie.com/darknet/install/   

darknet 官网还是很牛逼的... ...   


测试一下 OpenCV 是否安装并编译成功：

./darknet imtest data/eagle.jpg

配置好 OpenCV 后，直接显示图像.  


如果出现如下错误：

~~~bash
lab406@lab406-yang:~/hcq/deep_learning/github/darknet$ ./darknet detector train cfg/voc.data cfg/yolo-voc.cfg darknet19_448.conv.23
./darknet: error while loading shared libraries: libopencv_highgui.so.3.3: cannot open shared object file: No such file or directory
~~~

解决方案：将 opencv/lib 路径加到配置环境中，如下图所示：

1. 新建一个文件 /etc/ld.so.conf.d/opencv.conf, 写入 opencv 库的安装路径：   

~~~vim
/path/to/your/opencv/lib
~~~

2. 将 opencv 库的路径加入 LD_LIBRARY_PATH 环境变量中, 在 ~/.bashrc 文件中.   

~~~bash
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/path/to/your/opencv/lib
~~~

参考网址：https://stackoverflow.com/questions/12335848/opencv-program-compile-error-libopencv-core-so-2-4-cannot-open-shared-object-f    


## yolo（darknet） 代码结构

~~~bash
lab406@lab406-yang:~/hcq/deep_learning/github/darknet$ tree -L 2
.
├── cfg
│   ├── alexnet.cfg
│   ├── cifar.cfg
│   ├── cifar.test.cfg
│   ├── coco.data
│   ├── combine9k.data
│   ├── darknet19_448.cfg
│   ├── darknet19.cfg
│   ├── darknet9000.cfg
│   ├── darknet.cfg
│   ├── densenet201.cfg
│   ├── extraction22k.cfg
│   ├── extraction.cfg
│   ├── extraction.conv.cfg
│   ├── go.cfg
│   ├── go.test.cfg
│   ├── gru.cfg
│   ├── imagenet1k.data
│   ├── imagenet22k.dataset
│   ├── imagenet9k.hierarchy.dataset
│   ├── jnet-conv.cfg
│   ├── resnet152.cfg
│   ├── resnet50.cfg
│   ├── rnn.cfg
│   ├── rnn.train.cfg
│   ├── strided.cfg
│   ├── t1.test.cfg
│   ├── tiny.cfg
│   ├── tiny-yolo.cfg
│   ├── tiny-yolo-voc.cfg
│   ├── vgg-16.cfg
│   ├── vgg-conv.cfg
│   ├── voc.data
│   ├── writing.cfg
│   ├── yolo.2.0.cfg
│   ├── yolo9000.cfg
│   ├── yolo.cfg
│   ├── yolov1
│   ├── yolo-voc.2.0.cfg
│   └── yolo-voc.cfg
├── data
│   ├── 9k.labels
│   ├── 9k.names
│   ├── 9k.tree
│   ├── coco9k.map
│   ├── coco.names
│   ├── dog.jpg
│   ├── eagle.jpg
│   ├── giraffe.jpg
│   ├── goal.txt
│   ├── horses.jpg
│   ├── imagenet.labels.list
│   ├── imagenet.shortnames.list
│   ├── inet9k.map
│   ├── labels
│   ├── person.jpg
│   ├── scream.jpg
│   └── voc.names
├── examples
│   ├── art.c
│   ├── attention.c
│   ├── captcha.c
│   ├── cifar.c
│   ├── classifier.c
│   ├── coco.c
│   ├── darknet.c
│   ├── detector.c
│   ├── detector.py
│   ├── detector-scipy-opencv.py
│   ├── dice.c
│   ├── go.c
│   ├── lsd.c
│   ├── nightmare.c
│   ├── regressor.c
│   ├── rnn.c
│   ├── rnn_vid.c
│   ├── segmenter.c
│   ├── super.c
│   ├── swag.c
│   ├── tag.c
│   ├── voxel.c
│   ├── writing.c
│   └── yolo.c
├── include
│   └── darknet.h
├── LICENSE
├── LICENSE.fuck
├── LICENSE.gen
├── LICENSE.gpl
├── LICENSE.meta
├── LICENSE.mit
├── LICENSE.v1
├── Makefile
├── python
│   ├── darknet.py
│   └── proverbot.py
├── README.md
├── scripts
│   ├── dice_label.sh
│   ├── gen_tactic.sh
│   ├── get_coco_dataset.sh
│   ├── imagenet_label.sh
│   └── voc_label.py
└── src
    ├── activation_kernels.cu
    ├── activation_layer.c
    ├── activation_layer.h
    ├── activations.c
    ├── activations.h
    ├── avgpool_layer.c
    ├── avgpool_layer.h
    ├── avgpool_layer_kernels.cu
    ├── batchnorm_layer.c
    ├── batchnorm_layer.h
    ├── blas.c
    ├── blas.h
    ├── blas_kernels.cu
    ├── box.c
    ├── box.h
    ├── classifier.h
    ├── col2im.c
    ├── col2im.h
    ├── col2im_kernels.cu
    ├── compare.c
    ├── connected_layer.c
    ├── connected_layer.h
    ├── convolutional_kernels.cu
    ├── convolutional_layer.c
    ├── convolutional_layer.h
    ├── cost_layer.c
    ├── cost_layer.h
    ├── crnn_layer.c
    ├── crnn_layer.h
    ├── crop_layer.c
    ├── crop_layer.h
    ├── crop_layer_kernels.cu
    ├── cuda.c
    ├── cuda.h
    ├── data.c
    ├── data.h
    ├── deconvolutional_kernels.cu
    ├── deconvolutional_layer.c
    ├── deconvolutional_layer.h
    ├── demo.c
    ├── demo.h
    ├── detection_layer.c
    ├── detection_layer.h
    ├── dropout_layer.c
    ├── dropout_layer.h
    ├── dropout_layer_kernels.cu
    ├── gemm.c
    ├── gemm.h
    ├── gru_layer.c
    ├── gru_layer.h
    ├── im2col.c
    ├── im2col.h
    ├── im2col_kernels.cu
    ├── image.c
    ├── image.h
    ├── layer.c
    ├── layer.h
    ├── list.c
    ├── list.h
    ├── local_layer.c
    ├── local_layer.h
    ├── lstm_layer.c
    ├── lstm_layer.h
    ├── matrix.c
    ├── matrix.h
    ├── maxpool_layer.c
    ├── maxpool_layer.h
    ├── maxpool_layer_kernels.cu
    ├── network.c
    ├── network.h
    ├── normalization_layer.c
    ├── normalization_layer.h
    ├── option_list.c
    ├── option_list.h
    ├── parser.c
    ├── parser.h
    ├── region_layer.c
    ├── region_layer.h
    ├── reorg_layer.c
    ├── reorg_layer.h
    ├── rnn_layer.c
    ├── rnn_layer.h
    ├── route_layer.c
    ├── route_layer.h
    ├── shortcut_layer.c
    ├── shortcut_layer.h
    ├── softmax_layer.c
    ├── softmax_layer.h
    ├── stb_image.h
    ├── stb_image_write.h
    ├── tree.c
    ├── tree.h
    ├── utils.c
    └── utils.h
~~~

实践过程中主要修改的文件：   

1）Makefile  

2）./cfg 目录下的参数配置文件  

3）./data 目录下的数据  


## 实践步骤（文末附有问题集锦，不断更新）   


### 1. 基于 yolo.weights 模型参数测试单张图像
 
0) 下载预训练的模型参数 yolo.weights：    

~~~bash
wget https://pjreddie.com/media/files/yolo.weights   
~~~

1）基于 yolo.weights 模型参数测试单张图像：

~~~bash
./darknet detect cfg/yolo.cfg yolo.weights data/dog.jpg   
~~~


2）基于 yolo.weights 模型参数测试多张图像（Multiple Images）：不需要给定测试图像的路径，直接输入如下命令，然后程序会提示你输入测试图像路径，直到使用“ctrl + C”退出程序。

~~~bash
./darknet detect cfg/yolo.cfg yolo.weights
~~~

3）基于 yolo.weights 模型参数，使用“-thresh”参数控制显示的 bounding-box 个数。darknet 默认只显示被检测的物体中 confidence 大于或等于0.25的bounding-box，可以通过参数 "-thresh <value>" 改变。例如，“-thresh 0”表示显示出所有的bounding-box。你们可以根据实际所需，设置不同的值。    

值得一提的是，上述使用的是普通版的yolo（normal yolo），检测速度大概是 40-90 fps，在 VOC-2017 数据集上 mAP = 78.6%，COCO 数据集上 mAP = 48.1%。   

tiny yolo 是 yolo 的 fast 版本，网络结构有所改变（卷积层减少了），速度高达 155 fps，但是牺牲了准确率。本文不再介绍 tiny yolo 版本，感兴趣的读者可以参考 darknet 官网。   

### 2. 基于yolo.weights 模型参数，使用网络摄像头（Webcam）进行实时检测   

1）网络摄像头，如有多个摄像头可以使用 “-c <num>”参数进行选择，默认参数为0。

2）CUDA 和 OpenCV

万事俱备后，执行如下命令，你将看到当前的 FPS、框出物体的bounding-box、所属类别的概率。

~~~bash
./darknet detector demo cfg/coco.data cfg/yolo.cfg yolo.weights
~~~


### 3. 基于 yolo.weights 模型参数，测试一段视频（CUDA、OpenCV、Video），官网有提供一段检测的视频。     

~~~bash
./darknet detector demo cfg/coco.data cfg/yolo.cfg yolo.weights <video file>
~~~

### 4. 基于 darknet 在 VOC 数据集上训练 YOLO：   

1）下载 Pascal VOC 2007 和 2012 的数据集，数据集的目录可以参考上文的代码结构：

~~~bash
wget https://pjreddie.com/media/files/VOCtrainval_11-May-2012.tar
wget https://pjreddie.com/media/files/VOCtrainval_06-Nov-2007.tar
wget https://pjreddie.com/media/files/VOCtest_06-Nov-2007.tar
tar xf VOCtrainval_11-May-2012.tar
tar xf VOCtrainval_06-Nov-2007.tar
tar xf VOCtest_06-Nov-2007.tar
~~~

2）生成 VOC 数据集的标签，darknet 需要一个 “.txt” 文件，每行表示一张图像的信息，包括（x, y, w, h）如下所示：

~~~json
<object-class> <x> <y> <width> <height>
~~~

darknet 官网提供了一个针对 VOC 数据集，处理标签的脚本，执行如下命令：   

~~~bash
# 获取脚本
wget https://pjreddie.com/media/files/voc_label.py  

# 执行脚本，生成所需的“.txt”文件
python voc_label.py  
~~~

标签处理完成后，在 darknet 的根目录下，你应该能看到如下几个 ".txt" 文件.   

其中，2007_train.txt 和 2012_train.txt 分别表示2007年和2012年的训练数据。在本例中，我们将结合2007年和2012年的训练数据进行训练（测试数据使用2007年的测试数据），执行如下命令整合到 train.txt 文件中：

~~~bash
cat 2007_train.txt 2007_val.txt 2012_*.txt > train.txt
~~~

3）修改 “./cfg/voc.data” 配置文件

~~~conf
classes= 20  # 类别总数
train  = /home/hcq/document/deepLearning/github/darknet/train.txt  # 训练数据所在的位置
valid  = /home/hcq/document/deepLearning/github/darknet/2007_test.txt  # 测试数据所在的位置
names = data/voc.names
backup = backup  # 备份yolo模型参数的位置
~~~

4）下载预训练的参数（卷积权重），“darknet19_448.conv.23” 是使用 Imagenet 数据集进行预训练：

~~~bash
# 下载预训练的网络模型参数
wget https://pjreddie.com/media/files/darknet19_448.conv.23  
~~~

当然，如果硬件条件足够的话，你也可以自己进行预训练。首先下载 “Darknet19 448x448 model” 网络模型，然后执行如下命令（由于硬件条件限制，本文没有实践此部分）：   

~~~bash
./darknet partial cfg/darknet19_448.cfg darknet19_448.weights darknet19_448.conv.23 23
~~~

5）在darknet根目录下，创建一个“backup”文件夹：

~~~bash
mkdir backup
~~~

6）输入如下命令，训练模型：

~~~
./darknet detector train cfg/voc.data cfg/yolo-voc.cfg darknet19_448.conv.23
~~~

### 5. 基于 darknet 在 COCO 数据集上训练 YOLO

1）获取 COCO 数据集，在 darknet 根目录下执行如下命令：

~~~bash
cp scripts/get_coco_dataset.sh data
cd data
bash get_coco_dataset.sh
~~~

2）修改“cfg/coco.data”和“cfg/yolo.cfg”配置文件：

- “./cfg/coco.data”:   

~~~conf

classes= 80
train  = /home/hcq/document/deepLearning/github/darknet/data/coco/trainvalno5k.txt
#valid  = coco_testdev
valid = /home/hcq/document/deepLearning/github/darknet/data/coco/5k.txt
names = data/coco.names
backup = backup
eval=coco

- “./cfg/yolo.cfg”:   

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

3）训练模型：

~~~bash
./darknet detector train cfg/coco.data cfg/yolo.cfg darknet19_448.conv.23
~~~


## 问题集锦

1. 【OpenCV：to the PKG_CONFIG_PATH environment variable】

参考网址：http://blog.csdn.net/xuesen_lin/article/details/6124688

原因：没有配置 PKG_CONFIG_PATH 环境。缺省情况下，pkg-config首先在prefix/lib/pkgconfig/中查找相关包（譬如opencv）对应的相应的文件（opencv.pc）。在linux上上述路径名为 /usr/lib/pkconfig/。若是没有找到，它也会到PKG_CONFIG_PATH这个环境变量所指定的路径下去找。若是没有找到，它就会报错。

解决方案：找到 opencv.pc 所在的目录，正常情况下是在如下路径：

./opencv/lib/pkgconfig  # opencv 的编译请参考：http://blog.csdn.net/houchaoqun_xmu/article/details/78565077
然后打开环境变量的配置文件，将路径加上去，一系列命令如下所示：

~~~bash
sudo gedit ~/.bashrc  # 打开环境变量配置文件
# added by hcq 20171206 opencv
export PKG_CONFIG_PATH=/home/lab406/hcq/opencv/lib/pkgconfig:$PKG_CONFIG_PATH  # 现在是使用这个路径，可行
export PKG_CONFIG_PATH=/home/lab406/hcq/opencv/lib:$PKG_CONFIG_PATH  # 之前是使用这个路径，但不行
source ~/.bashrc  # 生效配置文件的修改
~~~

## 2. 【darknet: ./src/cuda.c:36: check_error: Assertion `0' failed】  

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

说明：也有可能是训练数据或者测试数据路径的问题造成的（网上有类似的，但本文还未遇到过）   


3. 【darknet: ./src/cuda.c:21: check_error: Assertion `0' failed.】   

这种情况网上比较多类似的问题，主要都是说 “ARCH= --gpu-architecture=compute_xx  --gpu-code=compute_xx” 的问题。直接对应自己的显卡型号到网上搜搜看就有答案了。     
