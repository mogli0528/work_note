# caffe + ssd 笔记   

`-------------------------------我是分割线------------------------------------`
## 1. caffe 笔记     
[1. caffe 源码阅读笔记 - langb ](./doc/caffe/read_caffe_source_code/)    
[2. 学习 caffe 系列 - 徐其华](./doc/caffe/learning_caffe/)     
[3. 使用 caffe ](./doc/caffe/use_caffe/)     
[4. 在 caffe 中添加 heatmap 层](./doc/caffe/heatmap/)     
[5. 在 caffe 中添加 Weighted_Softmax_Loss 层](./doc/caffe/weighted_softmax/)     

`-------------------------------我是分割线------------------------------------`
## 2. SSD 笔记

### 2.1 SSD 理论/训练
[1. 完整的 ssd 训练教程 ](./doc/ssd/doc/ssd_training_example.md)   
[2. 更为安全/不间断的训练 ssd 模型 ](./doc/ssd/doc/train_ssd_models_safely.md)  
[3. ssd 训练 python 脚本源码分析 ](./doc/ssd/doc/read_ssd_training_script.md)   
[4. ssd 训练 python 脚本中使用到的 skills ](../../dev_tools/python/doc/note/skills_in_ssd_training_script.md) 
[5. ssd 论文阅读 ](./doc/ssd/doc/ssd_paper_read.md)   
  
`-------------------------------我是分割线------------------------------------`
## 3. 从 caffe 到 ssd  
主要分析`ssd`基于`caffe`做了哪些改进工作。以下是在 ssd 中新添加的层.   
[1. annotated_data_layer](./doc/caffe_to_ssd/doc/annotated_data_layer.md)  
[2. data_transformer](./doc/caffe_to_ssd/doc/data_transformer.md)  
[3. detection_evaluate_layer](./doc/caffe_to_ssd/doc/detection_evaluate_layer.md)  
[4. multibox_loss_layer](./doc/caffe_to_ssd/doc/multibox_loss_layer.md)  
[5. proior_box_loss_layer](./doc/caffe_to_ssd/doc/proior_box_loss_layer.md)  

`-------------------------------我是分割线------------------------------------`
## 4. caffe/ssd 通用功能    
[1. 编译 caffe/ssd 源码中提供的文档 ](./doc/common/build_docs_of_caffe_or_ssd.md)   
[2. caffe/ssd 中的脚本列表 ](./doc/common/scripts_of_ssd.md)   
[3. caffe/ssd 中的术语 ](./doc/common/terms_in_caffe_or_ssd.md)   
[4. protobuf 使用方法](./doc/common/protobuf_compile.md)  
[5. datum 和 blob 的区别](./doc/common/datum_blob.md)  

[1. layers 官方文档](./doc/common/official_layers.md)  