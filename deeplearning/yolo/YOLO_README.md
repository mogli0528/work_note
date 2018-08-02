# YOLOv3 简介   

2015 年, R-CNN 横空出世, 目标检测 DL 世代大幕拉开。  

各路豪杰快速迭代, 陆续有了 SPP, fast, faster 版本, 至 R-FCN, 速度与精度齐飞, 区域推荐类网络大放异彩。  

奈何, 未达实时检测之基准, 难获工业应用之青睐。  

此时, 凭速度之长, 网格类检测异军突起, 先有 YOLO, 继而 SSD, 更是摘实时检测之桂冠, 与区域推荐类二分天下。然 YOLO 准确率却时遭世人诟病。  

遂有 JR 一鼓作气, 并 coco, 推 v2, 增加输出类别, 成就 9000。此后一年, 作者隐遁江湖, 逍遥 twitter。偶获灵感, 终推 v3, 横扫武林！    

## 准确率不再是短板  

自从 YOLO 诞生之日起, 它就被贴上了两个标签：  

* 速度很快  
* 不擅长检测小物体  

而后者, 成为了很多人对它望而却步的原因。  

由于原理上的限制, YOLO 仅检测最后一层卷积输出层, 小物体像素少, 经过层层卷积, 在这一层上的信息几乎体现不出来, 导致难以识别。   

YOLOv3 的提升:  

* 对于小物体的识别, 提高非常明显。  
* 对于紧凑密集或者高度重叠目标的检测, 提高非常明显. 比如对合影的人群检测, v3 一次检测到图中 90% 的人, 还增加了 tie（领带）这个新类别, 非常惊艳！  
* 提高了模型的泛化能力. 模型泛化能力很强的副作用, 就是分类结果跑偏.  
* 训练和检测速度都很快;  

和前辈们比, YOLO 的速度非常快, 比 R-CNN 快 1000 倍, 比 Fast R-CNN 快 100 倍。  

和同辈们比, YOLOv3-608 检测准确率比 DSSD 更高, 接近 FPN, 但是检测时间却只用了后面两者的三分之一不到。  

原因如论文中所说, 它在测试时观察整张图像, 预测会由图像中的全局上下文（global context）引导。   

它还通过单一网络评估做出预测, 而不像 R-CNN 这种系统, 一张图就需要成千上万次预测。

## YOLOv3 用了哪些 Tricks？  

### 1. 多级预测

终于为 YOLO 增加了 top down 的多级预测, 解决了 YOLO 颗粒度粗, 对小目标无力的问题。  

v2 只有一个 detection, v3 一下变成了 3 个, 分别是一个下采样(feature map 为 `13*13`), 2 个上采样的 eltwise sum, feature map 为 `26*26, 52*52`, 也就是说 v3 的 416 版本已经用到了 52 的 feature map, 而 v2 把多尺度考虑到训练的 data 采样上, 最后也只是用到了 13 的 feature map, 这应该是对小目标影响最大的地方。 

在论文中从单层预测五种 boundingbox 变成每层 3 种 boundongbox。  


### 2. loss不同  

作者 v3 替换了 v2 的 softmax loss 变成 logistic loss, 由于每个点所对应的 bounding box 少并且差异大, 每个 bounding 与 ground truth 的 matching 策略变成了 1 对 1。  

当预测的目标类别很复杂的时候, 采用 logistic regression 进行分类是更有效的, 比如在 Open Images Dataset 数据集进行分类。  

在这个数据集中, 会有很多重叠的标签, 比如女人、人, 如果使用 softmax 则意味着每个候选框只对应着一个类别, 但是实际上并不总是这样。复合标签的方法能对数据进行更好的建模。  

### 3. 加深网络  

采用简化的 residual block 取代了原来 1×1 和 3×3 的 block（其实就是加了一个 shortcut, 也是网络加深必然所要采取的手段）。   

这和上一点是有关系的, v2 的 darknet-19 变成了 v3 的 darknet-53, 为啥呢？就是需要上采样啊, 卷积层的数量自然就多了, 另外作者还是用了一连串的 `3*3`、`1*1` 卷积, `3*3` 的卷积增加 channel, 而 `1*1` 的卷积在于压缩 `3*3` 卷积后的特征表示。   

### 4. Router

由于 top down 的多级预测, 进而改变了 router（或者说 concatenate）时的方式, 将原来诡异的 reorg 改成了 upsample。   

## 下一代YOLO长啥样？  

1. mAP 会继续提高：随着模型训练越来越高效, 神经网络层级的不断加深, 信息抽象能力的不断提高, 以及一些小的修修补补, 未来的目标检测应用 mAP 会不断提升。    

2. 实时检测会成为标配：目前所谓的“实时”, 工业界是不认可的。为什么呢, 因为学术圈的人, 验证模型都是建立在 TitanX 或者 Tesla 这类强大的独立显卡上, 而实际的潜在应用场景中, 例如无人机/扫地/服务机器人/视频监控等, 是不会配备这些“重型装备”的。所以, 在嵌入式设备中, 如 FPGA, 轻量级 CPU 上, 能达到的实时, 才是货真价实的。    

3. 模型小型化成为重要分支：类似于 tiny YOLO 的模型分支会受到更多关注。模型的小型化是应用到嵌入式设备的重要前提。而物联网机器人无人机等领域还是以嵌入式设备为主的。模型剪枝/二值化/权值共享等手段会更广泛的使用。   

说点题外话. YOLO 让人联想到龙珠里的沙鲁（cell）, 不断吸收同化对手, 进化自己, 提升战斗力.   

YOLOv1 吸收了 SSD 的长处（加了 BN 层, 扩大输入维度, 使用了 Anchor, 训练的时候数据增强）, 进化到了 YOLOv2； 

YOLOv2 吸收 DSSD 和 FPN 的长处, 仿 ResNet 的 Darknet-53, 仿 SqueezeNet 的纵横交叉网络, 又进化到 YOLOv3。  

但是, 我相信这一定不是最终形态。让我们拭目以待吧！

## YOLOv3 实景大片儿  

https://zhuanlan.zhihu.com/p/36478097

注：本视频基于 coco 数据集, YOLOv3 cfg 和 weights。检测视频没有声音, 音轨是后加的。

存在一些识别错误, 但是整体来说, 亮点很多。截取几个瞬间:  

* 性能提升最大的是小物体或者远处的物体识别  
* 重叠遮挡物体的识别  
* 运动模糊物体的识别  
* 高密度汽车的识别  
* 密集人群识别  

### 相关链接

[1]. 论文 https://pjreddie.com/media/files/papers/YOLOv3.pdf  

[2]. YOLOv3 全文翻译 https://zhuanlan.zhihu.com/p/34945787  

[3]. 代码 https://github.com/pjreddie/darknet  

[4]. 官网 https://pjreddie.com/darknet/yolo/  

[5]. YouTube https://www.youtube.com/watch?v=MPU2HistivI  

[6]. 旧版 https://pjreddie.com/darknet/yolov2/  
         https://pjreddie.com/darknet/yolov1/  

[7]. 源码分享 https://github.com/muyiguangda/darknet  

[8]. YOLOv3 在Windows下的配置（无GPU） https://blog.csdn.net/baidu_36669549/article/details/79798587  
