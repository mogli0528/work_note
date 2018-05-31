# Caffe 源码梳理  
几个原则:  
- 从源头到结束, 按照流程来熟悉各个组件;   
- 抽离其中用到的数学库和其他开源库, 以便将来用到自己的项目中;  
- 熟悉 cpp-python 的接口调用方式;  

搞清楚流程之后再深入细节.   
## caffe 组件 
[0. protobuf 的使用](./doc/caffe_component/protobuf.md)   
[1. Caffe 的入口](./doc/caffe_component/caffe_main.md)   
[2. Caffe 中的矩阵计算](./doc/caffe_component/math_functions.md)  
[3. Solver 求解器的实现](./doc/caffe_component/solver.md)   
[4. Synced Memory 同步内存的实现](./doc/caffe_component/synced_memory.md)   
[5. Blob 的实现](./doc/caffe_component/blob.md)   
[6. Layer 的实现](./doc/caffe_component/layer.md)   
[7. layer_factory 的实现](./doc/caffe_component/layer_factory.md)   
[8. Caffe 中实现的所有 Layers](./doc/caffe_component/all_caffe_layers.md)   
[9. 卷积层的实现](./doc/caffe_component/conv_layer.md)   
[10. Net 的实现](./doc/caffe_component/conv_layer.md)   



## caffe 对于泛化工程的作用   

[1. 算法运行时间统计](./doc/caffe_for_other_projects/time_consume.md)   
[2. C++ 禁用拷贝构造函数和赋值运算符](./doc/caffe_for_other_projects/disable_copy-assignment_operator.md)   
