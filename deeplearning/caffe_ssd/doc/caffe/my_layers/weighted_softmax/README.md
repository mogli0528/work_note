# 在 caffe 中添加 Weighted_Softmax_Loss 层         
Weighted Softmax Loss Layer for Caffe        

## **添加步骤**      
(1) caffe.proto文件修改以下部分，增加pos_mult（指定某类的权重乘子）和pos_cid（指定的某类的类别编号）两个参数：  
```
// Message that stores parameters used by SoftmaxLayer, SoftmaxWithLossLayer
message SoftmaxParameter {
  enum Engine {
    DEFAULT = 0;
    CAFFE = 1;
    CUDNN = 2;
  }
  optional Engine engine = 1 [default = DEFAULT];

  // The axis along which to perform the softmax -- may be negative to index
  // from the end (e.g., -1 for the last axis).
  // Any other axes will be evaluated as independent softmaxes.
  optional int32 axis = 2 [default = 1];
  optional float pos_mult = 3 [default = 1];
  optional int32 pos_cid = 4 [default = 1];
}
```
(2) 在`include\caffe\layers`文件夹中增加`weighted_softmax_loss_layer.hpp` 文件.  
(3) 在`src\caffe\layers`文件夹中增加`weighted_softmax_loss_layer.cpp`和`weighted_softmax_loss_layer.cu`两个文件.    
(4) 在训练的`prototxt文件`中，按照下面方法使用（比如指定从0数起的第1类，权重加强，乘子为2.0）：    
```
layer {
  name: "loss"
  type: "WeightedSoftmaxWithLoss"
  bottom: "fc_end"
  bottom: "label"
  top: "loss"
  softmax_param {
    pos_cid: 1
    pos_mult: 2.0
  }
}
```
