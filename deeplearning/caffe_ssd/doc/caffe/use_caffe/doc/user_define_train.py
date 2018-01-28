#coding=utf-8

'''
  caffe的官方完美的支持 python 语言的兼容，提供了 pycaffe 的接口。用起来很方便，
首先来看一下最常用到的：caffe 的一个程序跑完之后会在 snapshot 所指定的目录下产生
一个后缀名为 caffemodel 的文件，这里存放的就是我们在训练网络的时候得到的每层的参
数信息，具体访问由 net.params['layerName'][0].data 访问权重参数 (num_filter, 
channel, weight, high), net.params['layerName'][1].data 访问 biase，格式是
(biase,), 这里的net.params使用的是字典格式。
```python
for layer_name, param in net.params.iteritems():
	print layer_name + "\t" + str(param[0].data.shape), str(param[1].data.shape)
```
'''

'''
   当然还有保存网络结构的字典类型 net.blobs['layerName'].data。这里最常用的也就是 
net.blobs['data'] 相关的使用，例如得到输入图片的大小 net.blobs['data'].data.shape 。
改变输入图片的大小 net.blobs['data'].reshape(0,3,227,227)，把图片 fed into网络。
net.blob['data'].data[...]=inputImage， 注意，这里最后一个 data 是一个数组，要是
只有一张图片就这样 net.blob['data'].data[0] = inputImage。

'''


'''
 1028
Caffe提供了python的接口（pycaffe），详见caffe/python文件夹。在python代码中import caffe，可以load models（导入模型），forward and backward（前向、反向迭代）， handle IO（数据输入输出），visualize networks（绘制net），instrument model solving（自定义优化方法）。所有的模型数据、计算参数都暴露在外，可供读写的。
caffe.Net是主要接口，负责导入数据、校验数据、计算模型的。
caffe.Classsifier用于图像分类。
caffe.Detector用于图像检测。
caffe.SGDSolver暴露在外的solver的接口。
caffe.io处理输入输出，数据预处理。
caffe.draw可视化net的结构。
Caffe blobs以numpy ndarrays的形式表示，更加方便、高效。
'''


