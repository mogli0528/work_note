# caffe源码 - second   

## 1. 预备知识   
[1. proto 预备知识](./doc/proto.md)   


## 2. caffe 正文
[1. blob 数据块](./doc/blob.md)   
[2. synced_memory 内存同步模块](./doc/synced_memory.md)   
[3. net 网络](./doc/net.md)   
[4. layer 层](./doc/layer.md)   



## 3. 源码阅读中的疑问    
1. caffe 中的 cpu_data 和 gpu_data 的 copy 流.  
2. caffe 中是如何处理中断信号的?    
```cpp
//-/ 安装 caffe 信号捕捉处理函数(Ctrl+C 中断和 Ctrl+Z 挂起).  
caffe::SignalHandler signal_handler(
    GetRequestedAction(FLAGS_sigint_effect),
    GetRequestedAction(FLAGS_sighup_effect));

shared_ptr<caffe::Solver<float> >
  solver(caffe::SolverRegistry<float>::CreateSolver(solver_param));

solver->SetActionFunction(signal_handler.GetActionFunction());

```
3. 默认参数的使用.   
4. net 的初始化过程.   
5. 前向运算和反向传播的 loss 计算. 
```
for (int top_id = 0; top_id < top.size(); ++top_id) {
  if (!this->loss(top_id)) { continue; }
  const int count = top[top_id]->count();
  const Dtype* data = top[top_id]->cpu_data();
  const Dtype* loss_weights = top[top_id]->cpu_diff();
  loss += caffe_cpu_dot(count, data, loss_weights);
}
```