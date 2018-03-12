# 评估模型的准确率和性能    

1. 测试模型准确率    
使用 test_lmdb 进行测试.   
```bash
$ /path/to/caffe/build/tools/caffe test -model lente_test.prototxt -weights mnist_aug_lenet_iter_60000.caffemodel -gpu 0 -iterations 100
```
其中:   
- 第一个输入参数由 train 变成了 test;    
- -model 参数指定一个测试用的 .prototxt 文件;   
- -weights 用来从一个 caffemodel 文件中读取参数的值;   
- -gpu 用来指定测试使用的 GPU 的序号;     
- -iterations 和 solver.prototxt 中的 test_iter 意思相似, 默认为50. iterations * batch_size 是最终测试的样本数量 ;   
程序中对每个 batch 的准确率都进行了计算, 最后得到一个总的准确率. 对于生成模型存档很多的模型评估, 可以使用一个自动化脚本获取所有模型的准确率, 根据最小的 loss 值和准确率的最大值选取一个最优模型.   
2. 评估模型的性能    
评估模型性能的主要工作是评估运行速度和内存占用.   
```bash
/path/to/caffe/build/tools/caffe time -model lenet.prototxt -gpu 0   
```
以 GPU 作为运算单元进行测试.   
```bash
/path/to/caffe/build/tools/caffe time -model lenet.prototxt   
```
去掉 -gpu 选项. 使用 cpu 进行运算速度评估, 因为 GPU 的成本和功耗问题, 所有一般都是先使用 GPU 进行训练, 得到模型后使用 cpu 或 FPGA 进行部署.   