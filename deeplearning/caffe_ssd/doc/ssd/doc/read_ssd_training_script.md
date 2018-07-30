# ssd 训练脚本源码分析   

## 1 控制训练的参数   
以下参数是控制训练过程的一些细节.    
```python
run_soon = True   			# 是否在生成全部文件之后开始训练
resume_training = True      # 是否使用最近的 snapshot 进行训练
remove_old_models = False   # 是否移除旧的 .caffemodel 模型文件   

# 设置了 7 个 batch_sampler 
use_batchnorm = False       # 是否使用了 batch norm ,目前并没有测试 batch norm 版本

# Divide the mini-batch to different GPUs.

# Evaluate on whole test set.
num_test_image = 22237 
test_batch_size = 16 
# Ideally test_batch_size should be divisible by num_test_image,
# otherwise mAP will be slightly off the true value.

# 理想情况下, test_batch_size 应该可以被 num_test_image 整除,否则 mAP 将会比真值小  
test_iter = int(math.ceil(float(num_test_image) / test_batch_size))

// accumulate gradients over `iter_size` x `batch_size` instances
optional int32 iter_size = 36 [default = 1];
```
**Note:**
1. 脚本中有自带的日志 log 功能和 job 脚本文件.   
job_dir = "jobs/VGGNet/jst_new_one/{}".format(job_name)   
2. 所有的 gpus 会平分 batch_size;    

## 2. skills
[ssd 训练脚本中使用到的 skills ](../../../dev_tools/python/doc/note/skills_in_ssd_training_script.md) 

