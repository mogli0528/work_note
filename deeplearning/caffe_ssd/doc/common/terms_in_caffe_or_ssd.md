# caffe/ssd 工程中的术语   

## 3.2 common 概念   
`LayerSetUp()`实现该层特定的一些参数初始化操作.  
`Reshape()`是根据`bottom`的 `shape` 来 reshape top blob 的 shape.   
一定要分清 `_param 和 (bottom 和 top)` 各自的类型. bottom 和 top 的类型为 Blob.  
.prototxt 中定义的 bottom blob 和 top blob 的个数是确定好的.  因为这在对应层的 .cpp 实现文件中会确定使用.      
## 3.3 不同的 Loss 函数
1. ctc loss: 语音识别.  
2. center loss: 人脸识别.   
3. Continuation Indicator layer: 语音识别.   