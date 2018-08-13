# 深度学习系列

## dataSets

### 1. 3D PASCAL VOC 

http://cvgl.stanford.edu/projects/pascal3d.html   

### 2. KITTI   

A project of Karlsruhe Institute of Technology and Toyota Technological Institute at Chicago.  
http://www.cvlibs.net/datasets/kitti/eval_object.php   

### 3. NYC3DCars 

a database of 3D vehicles in geographic context.  

http://nyc3d.cs.cornell.edu/

### 4. 视频监控中的目标追踪

Coarse Gaze Estimation in Visual Surveillance.  

http://www.robots.ox.ac.uk/ActiveVision/Research/Projects/2009bbenfold_headpose/project.html  


Complex-YOLO: An Euler-Region-Proposal for Real-time 3D Object Detection on Point Clouds: https://www.arxiv-vanity.com/papers/1803.06199/

`-------------------------------我是分割线------------------------------------`
## 1. 环境搭建   
[1. caffe/ssd/tensorflow 环境搭建](./dev_enviroment/)    

`-------------------------------我是分割线------------------------------------`
## 2. 深度学习框架
[ 吴恩达系列课程学习笔记](./common/doc/Andrew_Ng)   
[ caffe/ssd 笔记](./caffe_ssd/)    
[ tensorflow ](./tensorflow/)   
[ Yolo ](./yolo/YOLO_README.md)   

`-------------------------------我是分割线------------------------------------`
## 3. 深度学习实践--`training skills`   
[1. 训练——知乎专栏](https://zhuanlan.zhihu.com/p/27763696)         

`-------------------------------我是分割线------------------------------------`
## 4. 深度学习理论基础
[1.《神经网络与深度学习》](https://www.gitbook.com/book/tigerneil/neural-networks-and-deep-learning-zh/details)      
[2. 全连接层的作用是什么？](./doc/common/doc/role_of_full_connections.md)   


## 图像检测算法    

[基于候选区域的目标检测器](https://mp.weixin.qq.com/s?__biz=MzA3MzI4MjgzMw==&mid=2650741534&idx=1&sn=02dc164ffcedbf22124b97841ba67fe5&chksm=871adf60b06d567690fa2328b161c012a464687768e50f812a51b5533a7d68b99af1cf8f02b8&mpshare=1&scene=1&srcid=0502bxw5ni18dgAgWNwNLMml&pass_ticket=NzCwwBSPdP24YN41TZYcAAPBVkhYakzUVgSY%2BFROycimwoBfT%2FiIb2KS3AEb9T%2BG#rd)

### 1. 滑动窗口检测器   

我们根据滑动窗口从图像中剪切图像块。由于很多分类器只取固定大小的图像，因此这些图像块是经过变形转换的。但是，这不影响分类准确率，因为分类器可以处理变形后的图像。    
变形图像块被输入 CNN 分类器中, 提取出 4096 个特征. 之后我们使用 SVM 分类器识别类别和该边界框的另一个线性回归器。 之后, 我们使用 SVM 分类器识别类别和该边界框的另一个线性回归器。    
由此可知, 特征提取和分类(回归)是分为两个阶段进行的.    

> 我们创建很多窗口来检测不同位置的不同目标。要提升性能，一个显而易见的办法就是减少窗口数量。    

### 2. 选择性搜索     

如果不使用暴力方法, 那么最好使用候选区域方法(region proposal method) 创建目标检测的感兴趣区域(ROI)。     

在选择性搜索(selective search, SS)中, 我们首先将每个像素作为一组。然后计算每一组的 `纹理`，并将两个最接近的组结合起来。但是为了避免单个区域吞噬其他区域，我们首先对较小的组进行分组。我们继续合并区域，直到所有区域都结合在一起。      

下图第一行展示了如何使区域增长，第二行中的蓝色矩形代表合并过程中所有可能的 ROI。    

![](https://mmbiz.qpic.cn/mmbiz_png/KmXPKA19gW8H2Xghj59KZqesWzoictPPAef4WDUpnFGWib6CaISP6ht9IZoa2NDHn56oKSwv5p6icA1nyRnNQ4gvw/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1)  

### 3. R-CNN (region proposal - CNN)    

R-CNN 利用候选区域方法创建了约 2000 个 ROI。这些区域被转换为固定大小的图像，并分别馈送到卷积神经网络中。该网络架构后面会跟几个全连接层，以实现目标分类并提炼边界框。    

![使用候选区域、CNN、仿射层来定位目标](https://mmbiz.qpic.cn/mmbiz_jpg/KmXPKA19gW8H2Xghj59KZqesWzoictPPAszhOYK9DEFJv9IALfUVQUf2IUQWLnmJXlvstxm5Ijkow1XkmG6YV9g/640?wx_fmt=jpeg&tp=webp&wxfrom=5&wx_lazy=1)   

以下是 R-CNN 整个系统的流程图：

![](https://mmbiz.qpic.cn/mmbiz_png/KmXPKA19gW8H2Xghj59KZqesWzoictPPA8It7ORemXW5SasIicS1tWIiarIBN5cgdX05fwfBEnlPWOO1Bg9h5WFnQ/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1)  

通过使用更少且更高质量的 ROI，R-CNN 要比滑动窗口方法更快速、更准确。
```python
ROIs = region_proposal(image)
for ROI in ROIs
    patch = get_patch(image, ROI)
    results = detector(patch)
```

### 4. 边界框回归器    
 
候选区域方法有非常高的计算复杂度。为了加速这个过程，我们通常会使用计算量较少的候选区域选择方法构建 ROI，并在后面使用线性回归器（使用全连接层）进一步提炼边界框。     

![使用回归方法将蓝色的原始边界框提炼为红色的](https://mmbiz.qpic.cn/mmbiz_jpg/KmXPKA19gW8H2Xghj59KZqesWzoictPPAsO56GOPxEN5t3zMBGcOsEunnJQxuZeLqbpXrD0YVMmxAiavXibPFsYCw/640?wx_fmt=jpeg&tp=webp&wxfrom=5&wx_lazy=1)   

### 4. Fast R-CNN    

R-CNN 需要非常多的候选区域以提升准确度，但其实有很多区域是彼此重叠的，因此 R-CNN 的训练和推断速度非常慢。如果我们有 2000 个候选区域，且每一个都需要独立地馈送到 CNN 中，那么对于不同的 ROI，我们需要重复提取 2000 次特征。

此外，CNN 中的特征图以一种密集的方式表征空间特征，那么我们能直接使用特征图代替原图来检测目标吗？    

![](https://mmbiz.qpic.cn/mmbiz_png/KmXPKA19gW8H2Xghj59KZqesWzoictPPAYlN0qibbvBoaaLCaXtjNGE2icFHfWUjajfKvDpgMiajibBDPEaVmjh7M3Q/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1)

![直接利用特征图计算 ROI](https://mmbiz.qpic.cn/mmbiz_jpg/KmXPKA19gW8H2Xghj59KZqesWzoictPPAM6S59jKSOvE0pPqooETv5RtQJzsaddyR8oMMxk4KOXYibTyUDgMqMUg/640?wx_fmt=jpeg&tp=webp&wxfrom=5&wx_lazy=1)

Fast R-CNN 使用特征提取器（CNN）先提取整个图像的特征，而不是从头开始对每个图像块提取多次。然后，我们可以将创建候选区域的方法直接应用到提取到的特征图上。例如，Fast R-CNN 选择了 VGG16 中的卷积层 conv5 来生成 ROI，这些关注区域随后会结合对应的特征图以裁剪为特征图块，并用于目标检测任务中。我们使用 ROI 池化将特征图块转换为固定的大小，并馈送到全连接层进行分类和定位。因为 Fast-RCNN 不会重复提取特征，因此它能显著地减少处理时间。    

## 高效下载 coco 数据集  

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

## coco 2017 数据集下载链接

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
