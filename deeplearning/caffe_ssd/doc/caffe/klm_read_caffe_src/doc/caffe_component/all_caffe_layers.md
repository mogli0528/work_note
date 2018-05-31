# Caffe 中所有的 Layers   
Vision Layers   
Convolution   
Pooling   
Local Response Normalization LRN   
im2col   
Loss Layers   
Softmax   
Sum-of-Squares Euclidean   
Hinge Margin   
Sigmoid Cross-Entropy   
Infogain   
Accuracy and Top-k   
Activation Neuron Layers   
ReLU Rectified-Linear and Leaky-ReLU   
Sigmoid   
TanH Hyperbolic Tangent   
Absolute Value   
Power   
BNLL   
Data Layers   
Database   
In-Memory   
HDF5 Input   
HDF5 Output   
Images   
Windows   
Dummy   
Common Layers   
Inner Product   
Splitting   
Flattening   
Reshape   
Concatenation   
Slicing   
Elementwise Operations   
Argmax   
Softmax   
Mean-Variance Normalization   

要想创建一个 Caffe 模型，需要在 prototxt 中定义一个 model architecture(模型架构). Caffe 自带的 Layer 及其参数被定义在 caffe.proto 中. 

## ------------------------------ Vision Layers -------------------------------
头文件： ./include/caffe/vision_layers.hpp   
Vision layers 通常以图片images作为输入，运算后产生输出的也是图片images。对于图片而言，可能是单通道的(c=1)，例如灰度图，或者三通道的 (c=3)，例如RGB图。但是，对于Vision layers而言，最重要的特性是输入的spatial structure(空间结构)。2D的几何形状有助于输入处理，大部分的Vision layers工作是对于输入图片中的某一个区域做一个特定的处理，产生一个相应的输出。与此相反，其他大部分的layers会忽略输入的空间结构，而只是将输入视为一个很大的向量，维度为： c*h*w。   

## Convolution
类型(type)：Convolution(卷积层)   
CPU 实现： ./src/caffe/layers/convolution_layer.cpp  
CUDA、GPU实现： ./src/caffe/layers/convolution_layer.cu  
1. 参数 (convolution_param)：  
1.1 必要：   
 1) num_output (c_o): the number of filters(滤波器数目)  
 2) kernel_size (or kernel_h and kernel_w): specifies height and width of each filter(每一个滤波器的大小)   
强烈推荐：   
 1) weight_filler [default type: ‘constant’ value: 0)[滤波器的初始化权重，默认为 0)  
1.2 可选：  
 1) bias_term [default true]: specifies whether to learn and apply a set of additive biases to the filter outputs(是否添加bias-偏置项，默认为True)  
 2) pad (or pad_h and pad_w) [default 0]: specifies the number of pixels to (implicitly) add to each side of the input(为输入添加边界的像素大小，默认为0)  
 3) stride (or stride_h and stride_w) [default 1]: specifies the intervals at which to apply the filters to the input(每一次使用滤波器处理输入图片时，前后两次处理区域的间隔，即“步进”，默认为1)  
 4) group (g) [default 1]: If g > 1, we restrict the connectivity of each filter to a subset of the input. Specifically, the input and output channels are separated into g groups, and the ith output group channels will be only connected to the ith input group channels.(默认为1，如果大于1：将限制每一个滤波器只与输入的一部分连接。输入、输出通道会被分隔为不同的g个groups，并且第i个输出group只会与第i个输出group相关)  
2. 输入(Input)   
n * c_i * h_i * w_i   
3. 输出(Output)  
n * c_o * h_o * w_o   
其中 h_o = (h_i + 2 * pad_h - kernel_h) / stride_h + 1； w_o 计算方法类似.  
4. 例子   
(详见 ./examples/imagenet/imagenet_train_val.prototxt)   
```proto
layer {
  name: "conv1"                  # 名称：conv1
  type: "Convolution"            # 类型：卷积层
  bottom: "data"                 # 输入层：数据层
  top: "conv1"                   # 输出层：卷积层1
  # 滤波器(filters)的学习速率因子和衰减因子
  param { lr_mult: 1 decay_mult: 1 }
  # 偏置项(biases)的学习速率因子和衰减因子
  param { lr_mult: 2 decay_mult: 0 }
  convolution_param {
    num_output: 96               # 96个滤波器(filters)
    kernel_size: 11              # 每个滤波器(filters)大小为11*11
    stride: 4                    # 每次滤波间隔为4个像素
    weight_filler {
      type: "gaussian"           # 初始化高斯滤波器(Gaussian)
      std: 0.01                  # 标准差为0.01， 均值默认为0
    }
    bias_filler {
      type: "constant"           # 初始化偏置项(bias)为零
      value: 0
    }
  }
}
```
卷积层(The Convolution layer)利用一系列具有学习功能的滤波器(learnable filters)对输入的图像进行卷积操作，每一个滤波器(filter)对于一个特征(feature )会产生一个输出图像(output image)。   

## Pooling   
类型(type)：Pooling(池化层)   
CPU 实现： ./src/caffe/layers/pooling_layer.cpp   
CUDA、GPU实现： ./src/caffe/layers/pooling_layer.cu   
1. 参数 (pooling_param)：   
1.1 必要参数       
kernel_size (or kernel_h and kernel_w): specifies height and width of each filter(每一个滤波器的大小)  
1.2 可选参数   
 1) pool [default MAX]: the pooling method. Currently MAX, AVE, or STOCHASTIC(pooling方法，目前有MAX、AVE,和STOCHASTIC三种，默认为MAX)  
 2) pad (or pad_h and pad_w) [default 0]: specifies the number of pixels to (implicitly) add to each side of the input(为输入添加边界的像素大小，默认为0)  
 3) stride (or stride_h and stride_w) [default 1]: specifies the intervals at which to apply the filters to the input(每一次使用滤波器处理输入图片时，前后两次处理区域的间隔，即“步进”，默认为1)  
2. 输入(Input)   
n * c_i * h_i * w_i   
3. 输出(Output)   
n * c_o * h_o * w_o   
其中 h_o = (h_i + 2 * pad_h - kernel_h) / stride_h + 1；w_o 计算方法类似.   
4. 例子   
(详见 ./examples/imagenet/imagenet_train_val.prototxt)   
```
layer {
  name: "pool1"                 # 名称：pool1
  type: "Pooling"               # 类型：池化层
  bottom: "conv1"               # 输入层：卷积层conv1
  top: "pool1"                  # 输出层：池化层pool1
  pooling_param {
    pool: MAX                   # pool方法：MAX
    kernel_size: 3              # 每次pool区域为3*3像素大小
    stride: 2                   # pool步进为2
  }
}
```

## Local Response Normalization (LRN)

类型(type)：LRN(局部响应归一化层)
CPU 实现： ./src/caffe/layers/lrn_layer.cpp
CUDA、GPU实现： ./src/caffe/layers/lrn_layer.cu
1. 可选参数 (lrn_param)： 
 1) local_size [default 5]: the number of channels to sum over (for cross channel LRN) or the side length of the square region to sum over (for within channel LRN)(对于cross channel LRN，表示需要求和的channel的数量；对于within channel LRN表示需要求和的空间区域的边长；默认为5)   
 2) alpha [default 1]: the scaling parameter(缩放参数，默认为1)   
 3) beta [default 5]: the exponent(指数，默认为5)   
 4) norm_region [default ACROSS_CHANNELS]: whether to sum over adjacent channels (ACROSS_CHANNELS) or nearby spatial locaitons (WITHIN_CHANNEL)(选择基准区域，是ACROSS_CHANNELS => 相邻channels，还是WITHIN_CHANNEL => 同一 channel下的相邻空间区域；默认为ACROSS_CHANNELS)   
LRN Layer 对一个局部的输入区域进行归一化，有两种模式。
 5) ACROSS_CHANNELS 模式，局部区域在相邻的 channels 之间拓展，不进行空间拓展，所以维度是 local_size x 1 x 1。   
 6) WITHIN_CHANNEL 模式，局部区域进行空间拓展，但是是在不同的 channels 中，所以维度是 1 x local_size x local_size。对于每一个输入，都要除以下式:
![)[../pictures/lrn.jpg)
其中 n 是局部区域的大小，求和部分是对该输入值为中心的区域进行求和(必要时候可以补零)。  

## im2col
Im2col 是一个 helper 方法，用于将图片文件 image 转化为列矩阵，详细的细节不需要过多的了解。在 Caffe 中进行卷积操作时会将卷积操作转化为矩阵乘法. 做矩阵乘法时，会用到 Im2col 方法。  
 
## Loss Layers
Caffe 是通过最小化输出 output 与目标 target 之间的 cost(loss) 来驱动学习的。loss 是由 forward pass 计算得出的，loss 的 gradient 是由 backward pass 计算得出的。

## Softmax
类型(type)：SoftmaxWithLoss(广义线性回归分析损失层)
Softmax Loss Layer 计算的是输入的多项式回归损失(multinomial logistic loss of the softmax of its inputs)。可以当作是将一个 softmax layer 和一个 multinomial logistic loss layer 连接起来，但是计算出的 gradient 更可靠。

## Sum-of-Squares / Euclidean
类型(type)：EuclideanLoss(欧式损失层)
Euclidean loss layer计算两个不同输入之间的平方差之和，计算公式

## Hinge / Margin

类型(type)：HingeLoss
CPU 实现： ./src/caffe/layers/hinge_loss_layer.cpp
CUDA、GPU实现： 尚无
1. 可选参数 (hinge_loss_param)：     
norm [default L1]: the norm used. Currently L1, L2(可以选择使用L1范数或者L2范数；默认为L1)  
2. 输入(Input)   
n * c * h * w Predictions(预测值)   
n * 1 * 1 * 1 Labels(标签值)   
3. 输出(Output)
1 * 1 * 1 * 1 Computed Loss(计算得出的loss值)      
4. 例子   
 1) 使用L1范数  
```proto
layer {
  name: "loss"                  # 名称：loss
  type: "HingeLoss"             # 类型：HingeLoss
  bottom: "pred"                # 输入：预测值
  bottom: "label"               # 输入：标签值
}
```
 2) 使用L2范数  
```proto
layer {
  name: "loss"                  # 名称：loss
  type: "HingeLoss"             # 类型：HingeLoss
  bottom: "pred"                # 输入：预测值
  bottom: "label"               # 输入：标签值
  top: "loss"                   # 输出：loss值
  hinge_loss_param {
    norm: L2                    # 使用L2范数
  }
}
```

## Sigmoid Cross-Entropy
类型(type)：SigmoidCrossEntropyLoss
(没有详解)
Infogain

类型(type)：InfogainLoss
(没有详解)
Accuracy and Top-k

类型(type)：Accuracy
计算输出的准确率(相对于target)，事实上这不是一个loss layer，并且也没有backward pass。
Activation / Neuron Layers

激励层的操作都是element-wise的操作(针对每一个输入blob产生一个相同大小的输出)：

输入(Input) 
n * c * h * w
输出(Output) 
n * c * h * w

## ReLU / Rectified-Linear and Leaky-ReLU
类型(type)：ReLU
CPU 实现： ./src/caffe/layers/relu_layer.cpp
CUDA、GPU实现： ./src/caffe/layers/relu_layer.cu

可选参数 (relu_param)：
- negative_slope [default 0]: specifies whether to leak the negative part by multiplying it with the slope value rather than setting it to 0.(但当输入x小于0时，指定输出为negative_slope * x；默认值为0)   
例子(详见 ./examples/imagenet/imagenet_train_val.prototxt)   
```proto
layer {
  name: "relu1"
  type: "ReLU"
  bottom: "conv1"
  top: "conv1"
}
```
给定一个输入值x，ReLU layer的输出为：x > 0 ? x : negative_slope * x，如未给定参数negative_slope 的值，则为标准ReLU方法：max(x, 0)。ReLU layer支持in-place计算，输出会覆盖输入，以节省内存空间。

## Sigmoid
类型(type)：Sigmoid
CPU 实现： ./src/caffe/layers/sigmoid_layer.cpp
CUDA、GPU实现： ./src/caffe/layers/sigmoid_layer.cu

例子(详见 ./examples/mnist/mnist_autoencoder.prototxt)
```proto
layer {
  name: "encode1neuron"
  bottom: "encode1"
  top: "encode1neuron"
  type: "Sigmoid"
}
```
对于每一个输入值x，Sigmoid layer的输出为sigmoid(x)。   

## TanH / Hyperbolic Tangent
类型(type)：TanH
CPU 实现： ./src/caffe/layers/tanh_layer.cpp
CUDA、GPU实现： ./src/caffe/layers/tanh_layer.cu
例子
```proto
layer {
  name: "layer"
  bottom: "in"
  top: "out"
  type: "TanH"
}
```
对于每一个输入值x，TanH layer的输出为tanh(x)。

## Absolute Value
类型(type)：AbsVal
CPU 实现： ./src/caffe/layers/absval_layer.cpp
CUDA、GPU实现： ./src/caffe/layers/absval_layer.cu

例子
```proto
layer {
  name: "layer"
  bottom: "in"
  top: "out"
  type: "AbsVal"
}
```
对于每一个输入值 x，AbsVal layer 的输出为 abs(x)。

## Power
类型(type)：Power
CPU 实现： ./src/caffe/layers/power_layer.cpp
CUDA、GPU实现： ./src/caffe/layers/power_layer.cu
参数 (power_param)：

可选： 
power [default 1)[指数，默认为1)
scale [default 1)[比例，默认为1)
shift [default 0)[偏移，默认为0)
例子
```proto
layer {
  name: "layer"
  bottom: "in"
  top: "out"
  type: "Power"
  power_param {
    power: 1
    scale: 1
    shift: 0
  }
}
```
对于每一个输入值x，Power layer的输出为(shift + scale * x) ^ power。

## BNLL
类型(type)：BNLL(二项正态对数似然, binomial normal log likelihood)  
CPU 实现： ./src/caffe/layers/bnll_layer.cpp
CUDA、GPU实现： ./src/caffe/layers/bnll_layer.cu
例子
```proto
layer {
  name: "layer"
  bottom: "in"
  top: "out"
  type: BNLL
}
```
对于每一个输入值 x，BNLL layer的输出为 log(1 + exp(x))。

## Data Layers
Data 通过Data Layers进入Caffe，Data Layers位于Net的底部。 
Data 可以来自：   
1. 高效的数据库(LevelDB 或 LMDB)；   
2. 内存；   
3. HDF5 或 image 文件(效率低)。   
基本的输入预处理(例如：减去均值，缩放，随机裁剪，镜像处理)可以通过指定 TransformationParameter 达到。

## Database
类型(type)：Data(数据库)
参数： 
必要： 
source: the name of the directory containing the database(数据库名称)
batch_size: the number of inputs to process at one time(每次处理的输入的数据量)
可选： 
rand_skip: skip up to this number of inputs at the beginning; useful for asynchronous sgd(在开始的时候跳过这个数值量的输入；这对于异步随机梯度下降是非常有用的)
backend [default LEVELDB]: choose whether to use a LEVELDB or LMDB(选择使用LEVELDB 数据库还是LMDB数据库，默认为LEVELDB)

## MemoryData
类型(type)：MemoryData
参数： 
必要： 
batch_size, channels, height, width: specify the size of input chunks to read from memory(4个值，确定每次读取输入数据量的大小)
Memory Data Layer从内存直接读取数据(而不是复制数据)。使用Memory Data Layer之前，必须先调用，MemoryDataLayer::Reset(C++方法)或Net.set_input_arrays(Python方法)以指定一个source来读取一个连续的数据块(4D，按行排列)，每次读取大小由batch_size决定。

## HDF5 Input
类型(type)：HDF5Data
参数： 
必要： 
source: the name of the file to read from(读取的文件的名称)
batch_size(每次处理的输入的数据量)

## HDF5 Output
类型(type): HDF5Output   
必要参数：   
file_name: name of file to write to(写入的文件的名称)
HDF5 output layer 与这部分的其他 layer 的功能正好相反，不是读取而是写入。

## Images
类型(type)：ImageData
参数： 
必要： 
source: name of a text file, with each line giving an image filename and label(一个text文件的名称，每一行指定一个image文件名和label)
batch_size: number of images to batch together(每次处理的image的数据)
可选： 
rand_skip: (在开始的时候跳过这个数值量的输入)
shuffle: [default false)[是否随机乱序，默认为否) 
-new_height, new_width: if provided, resize all images to this size(缩放所有的 image 到新的大小)

## Windows
类型(type)：WindowData
(没有详解)

## Dummy
类型(type)：DummyData
DummyData 用于开发和测试，详见DummyDataParameter(没有给出链接)。

## ------------------- Common Layers -------------------------

## Inner Product   
类型(type)：Inner Product(全连接层)
CPU 实现： ./src/caffe/layers/inner_product_layer.cpp
CUDA、GPU实现： ./src/caffe/layers/inner_product_layer.cu
1. 参数 (inner_product_param)：
1.1 必要：   
num_output (c_o): the number of filters(滤波器数目)
强烈推荐：   
weight_filler [default type: ‘constant’ value: 0)[滤波器权重；默认类型为constant，默认值为0)
1.2 可选：   
bias_filler [default type: ‘constant’ value: 0)[bias-偏置项的值，默认类型为constant，默认值为0)
bias_term [default true]: specifies whether to learn and apply a set of additive biases to the filter outputs(是否添加bias-偏置项，默认为True)
2. 输入(Input)   
n * c_i * h_i * w_i   
3. 输出(Output)   
n * c_o * 1 * 1   
4. 例子   
```proto
layer {
  name: "fc8"                              # 名称：fc8
  type: "InnerProduct"                     # 类型：全连接层
  # 权重(weights)的学习速率因子和衰减因子
  param { lr_mult: 1 decay_mult: 1 }
  # 偏置项(biases)的学习速率因子和衰减因子
  param { lr_mult: 2 decay_mult: 0 }
  inner_product_param {
    num_output: 1000                       # 1000个滤波器(filters)
    weight_filler {
      type: "gaussian"                     # 初始化高斯滤波器(Gaussian)
      std: 0.01                            # 标准差为0.01， 均值默认为0
    }
    bias_filler {
      type: "constant"                     # 初始化偏置项(bias)为零
      value: 0
    }
  }
  bottom: "fc7"                            # 输入层：fc7
  top: "fc8"                               # 输出层：fc8
}
```
InnerProduct layer(全连接层)将输入视为一个 vector, 输出也是一个 vector(height 和 width 被设置为 1 ?--TODO)   

## Splitting   
类型(type)：Split
Split layer 用于将一个输入的 blob 分离成多个输出的 blob. 这用于当需要将一个 blob 输入到多个输出 layer 时。

## Flattening   
类型(type)：Flatten   
Flatten layer 用于把一个维度为 `n * c * h * w` 的输入转化为一个维度为 `n * (c*h*w)`的向量输出。   

## Reshape   
类型(type)：Reshape
CPU 实现： ./src/caffe/layers/reshape_layer.cpp
CUDA、GPU实现： 尚无
1. 可选参数 (reshape_param)    
shape(改变后的维度，详见下面解释)
2. 输入(Input)   
a single blob with arbitrary dimensions(一个任意维度的blob)
3. 输出(Output)   
the same blob, with modified dimensions, as specified by reshape_param(相同内容的blob，但维度根据reshape_param改变)
4. 例子   
```proto
layer {
name: "reshape"                       # 名称：reshape
type: "Reshape"                       # 类型：Reshape
bottom: "input"                       # 输入层名称：input
top: "output"                         # 输出层名称：output
reshape_param {
  shape {
    dim: 0  # 这个维度与输入相同
    dim: 2
    dim: 3
    dim: -1 # 根据其他维度自动推测
  }
 }
}
```
Reshape layer 只改变输入数据的维度，但内容不变，也没有数据复制的过程，与 Flatten layer 类似。  
输出维度由 reshape_param 指定，正整数直接指定维度大小，下面两个特殊的值：  
 1) 0 => 表示复制输入的相应维度值。   
 2) -1 => 表示根据其他维度自动推测。reshape_param 中至多只能有一个 `-1`。
再举一个例子：如果指定 reshape_param 参数为：{ shape { dim: 0 dim: -1 } } ，那么输出和 Flattening layer 的输出是完全一样的(因为 dim 的默认值为 0 )。

## Concatenation
类型(type)：Concat(连结层)
CPU 实现： ./src/caffe/layers/concat_layer.cpp
CUDA、GPU实现： ./src/caffe/layers/concat_layer.cu
1. 可选参数 (concat_param)   
axis [default 1]: 0 for concatenation along num and 1 for channels.(0代表连结num，1代表连结channel)   
2. 输入(Input) 
for each input blob i from 1 to K:   
    -n_i * c_i * h * w 
即: 第 i 个 blob 的维度是 n_i * c_i * h * w，共 K 个.   
3. 输出(Output)   
当 axis = 0 时，输出 blob 的维度为 (n_1 + n_2 + … + n_K) * c_1 * h * w, 要求所有 input 的 channel 是相同的.    
当 axis = 1 时，输出 blob 的维度为 n_1 * (c_1 + c_2 + … + c_K) * h * w, 要求所有 input 的 num 是相同的.       
4. 例子
```proto
layer {
  name: "concat"
  bottom: "in1"
  bottom: "in2"
  top: "out"
  type: "Concat"
  concat_param {
    axis: 1
  }
}
```
Concat layer 用于把多个输入 blob 连结成一个输出 blob.   

## Slicing
Slice layer 用于将一个 input layer 分割成多个 output layers, 根据给定的维度(目前只能指定 num 或者 channel)。
类型(type)：Slice
例子
```proto
layer {
  name: "slicer_label"
  type: "Slice"
  bottom: "label"
  ## 假设label的维度是：N x 3 x 1 x 1
  top: "label1"
  top: "label2"
  top: "label3"
  slice_param {
    axis: 1                        # 指定维度为channel
    slice_point: 1                 # 将 label[~][1][~][~] 赋给label1
    slice_point: 2                 # 将 label[~][2][~][~] 赋给label2
  }
}
```
axis 表明是哪一个维度， slice_point 是该维度的索引， slice_point 的数量必须是 top blobs 的数量减 1.    

## Elementwise Operations
类型(type)： Eltwise
(没有详解)
Argmax

类型(type)：ArgMax
(没有详解)
Softmax

类型(type)：Softmax
(没有详解)
Mean-Variance Normalization

类型(type)：MVN
(没有详解)