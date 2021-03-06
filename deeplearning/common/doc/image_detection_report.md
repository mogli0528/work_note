# 图像检测算法发展概述     

[基于候选区域的目标检测器](https://mp.weixin.qq.com/s?__biz=MzA3MzI4MjgzMw==&mid=2650741534&idx=1&sn=02dc164ffcedbf22124b97841ba67fe5&chksm=871adf60b06d567690fa2328b161c012a464687768e50f812a51b5533a7d68b99af1cf8f02b8&mpshare=1&scene=1&srcid=0502bxw5ni18dgAgWNwNLMml&pass_ticket=NzCwwBSPdP24YN41TZYcAAPBVkhYakzUVgSY%2BFROycimwoBfT%2FiIb2KS3AEb9T%2BG#rd)

## 1. 滑动窗口检测器   

我们根据滑动窗口从图像中剪切图像块。由于很多分类器只取固定大小的图像，因此这些图像块是经过变形转换的。但是，这不影响分类准确率，因为分类器可以处理变形后的图像。    
变形图像块被输入 CNN 分类器中, 提取出 4096 个特征. 之后我们使用 SVM 分类器识别类别和该边界框的另一个线性回归器。 之后, 我们使用 SVM 分类器识别类别和该边界框的另一个线性回归器。    
由此可知, 特征提取和分类(回归)是分为两个阶段进行的.    

> 我们创建很多窗口来检测不同位置的不同目标。要提升性能，一个显而易见的办法就是减少窗口数量。    

## 2. 选择性搜索     

如果不使用暴力方法, 那么最好使用候选区域方法(region proposal method) 创建目标检测的感兴趣区域(ROI)。     

在选择性搜索(selective search, SS)中, 我们首先将每个像素作为一组。然后计算每一组的 `纹理`，并将两个最接近的组结合起来。但是为了避免单个区域吞噬其他区域，我们首先对较小的组进行分组。我们继续合并区域，直到所有区域都结合在一起。      

下图第一行展示了如何使区域增长，第二行中的蓝色矩形代表合并过程中所有可能的 ROI。    

![](https://mmbiz.qpic.cn/mmbiz_png/KmXPKA19gW8H2Xghj59KZqesWzoictPPAef4WDUpnFGWib6CaISP6ht9IZoa2NDHn56oKSwv5p6icA1nyRnNQ4gvw/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1)  

## 3. R-CNN (region proposal - CNN)    

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

## 4. 边界框回归器    
 
候选区域方法有非常高的计算复杂度。为了加速这个过程，我们通常会使用计算量较少的候选区域选择方法构建 ROI，并在后面使用线性回归器（使用全连接层）进一步提炼边界框。     

![使用回归方法将蓝色的原始边界框提炼为红色的](https://mmbiz.qpic.cn/mmbiz_jpg/KmXPKA19gW8H2Xghj59KZqesWzoictPPAsO56GOPxEN5t3zMBGcOsEunnJQxuZeLqbpXrD0YVMmxAiavXibPFsYCw/640?wx_fmt=jpeg&tp=webp&wxfrom=5&wx_lazy=1)   

## 4. Fast R-CNN    

R-CNN 需要非常多的候选区域以提升准确度，但其实有很多区域是彼此重叠的，因此 R-CNN 的训练和推断速度非常慢。如果我们有 2000 个候选区域，且每一个都需要独立地馈送到 CNN 中，那么对于不同的 ROI，我们需要重复提取 2000 次特征。

此外，CNN 中的特征图以一种密集的方式表征空间特征，那么我们能直接使用特征图代替原图来检测目标吗？    

![](https://mmbiz.qpic.cn/mmbiz_png/KmXPKA19gW8H2Xghj59KZqesWzoictPPAYlN0qibbvBoaaLCaXtjNGE2icFHfWUjajfKvDpgMiajibBDPEaVmjh7M3Q/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1)

![直接利用特征图计算 ROI](https://mmbiz.qpic.cn/mmbiz_jpg/KmXPKA19gW8H2Xghj59KZqesWzoictPPAM6S59jKSOvE0pPqooETv5RtQJzsaddyR8oMMxk4KOXYibTyUDgMqMUg/640?wx_fmt=jpeg&tp=webp&wxfrom=5&wx_lazy=1)

Fast R-CNN 使用特征提取器（CNN）先提取整个图像的特征，而不是从头开始对每个图像块提取多次。然后，我们可以将创建候选区域的方法直接应用到提取到的特征图上。例如，Fast R-CNN 选择了 VGG16 中的卷积层 conv5 来生成 ROI，这些关注区域随后会结合对应的特征图以裁剪为特征图块，并用于目标检测任务中。我们使用 ROI 池化将特征图块转换为固定的大小，并馈送到全连接层进行分类和定位。因为 Fast-RCNN 不会重复提取特征，因此它能显著地减少处理时间。    
