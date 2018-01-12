# caffe/ssd 工程中的术语   

## 1. Layers  
1.1 卷积   
关于卷积层的输入和输出的关系. 比如对于:   
Input: [n * c_i * h_i * w_i]  
Output: [n * c_o * h_o * w_o]   
```
w_o = h_o = (h_i + 2 * pad_h - kernel_h) / stride_h + 1 
```
1.2 Polling  
caffe 默认为最大池化.   
如何使用(s-k-p)这三个参数才能实现 half polling?   
输出的 w_o 和 h_o 的计算方式和卷积层相同.    
1.3 Loss Layers   
关于 loss 的解释很到位的一句话.   
> Loss drives learning by comparing an output to a target and assigning cost to minimize. The loss itself is computed by the forward pass and the gradient w.r.t. to the loss is computed by the backward pass.   
1.4 交叉熵 (Cross-Entropy) 损失  




## 3.2 common 概念   
`LayerSetUp()`实现该层特定的一些参数初始化操作.  




`Reshape()`是根据`bottom`的 `shape` 来 reshape top blob 的 shape.   
一定要分清 `_param 和 (bottom 和 top)` 各自的类型. bottom 和 top 的类型为 Blob.  
.prototxt 中定义的 bottom blob 和 top blob 的个数是确定好的.  因为这在对应层的 .cpp 实现文件中会确定使用.    




## 3.3 不同的 Loss 函数
1. ctc loss: 语音识别.  
2. center loss: 人脸识别.   
3. Continuation Indicator layer: 语音识别.   


## **Note**
1. caffe 的 bottom 和 top   
需要注意的是 bottom 和 top 是可以有多个的, 即可以出现多个输入和多个输出. 而 layer 的实现者在实现 layer 之前一定会确定好 bottom 和 top 的个数,并在 layer 的实现函数中体现出来.