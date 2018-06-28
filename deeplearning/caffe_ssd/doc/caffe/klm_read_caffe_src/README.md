# Caffe 源码梳理  
几个原则:  
- 从源头到结束, 按照流程来熟悉各个组件;   
- 抽离其中用到的数学库和其他开源库, 以便将来用到自己的项目中;  

搞清楚流程之后再深入细节.   
## caffe 组件 
### 1. 基本组件  
- [ protobuf 的使用](./doc/caffe_component/protobuf.md)   
- [ Caffe 的入口](./doc/caffe_component/caffe_main.md)   
- [ Caffe 中的矩阵计算](./doc/caffe_component/math_functions.md)  
- [ Solver 求解器的实现](./doc/caffe_component/solver.md)   
- [ Synced Memory 同步内存的实现](./doc/caffe_component/synced_memory.md)   
- [ BlobFlow ](./doc/caffe_component/blob.md)   
- [ Layer 的实现](./doc/caffe_component/layer.md)   
- [ layer_factory 的实现](./doc/caffe_component/layer_factory.md)   
- [ Caffe 中实现的所有 Layers](./doc/caffe_component/all_caffe_layers.md)   
- [ 卷积层的实现](./doc/caffe_component/conv_layer.md)   
- [ Net 的实现](./doc/caffe_component/conv_layer.md)   
### 2. 线程管理    
- [ 线程系统 - Internal Thread ](./doc/caffe_component/thread/internal_thread.md)   
- [ 全局线程管理器 ](./doc/caffe_component/thread/global_thread_management.md)  

## caffe 对于泛化工程的作用   
- [ 算法运行时间统计](./doc/caffe_for_other_projects/time_consume.md)   
- [ C++ 禁用拷贝构造函数和赋值运算符](./doc/caffe_for_other_projects/disable_copy-assignment_operator.md)   

## caffe 中 cpp 接口到 python 接口的封装   
- [caffe cpp to python 的接口封装](./doc/caffe/klm_read_caffe_src/doc/caffe_for_other_projects/caffe.cpp2pycaffe.md)  