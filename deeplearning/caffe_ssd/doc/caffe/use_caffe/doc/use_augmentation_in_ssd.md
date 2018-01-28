# 在 caffe 中添加 ssd 中使用的增广方式   
1. 更改 load_batch() 函数   
在 data_layer.cpp 文件中.   
```
# 在函数开头作如下声明:
const TransformationParameter& transform_param = this->layer_param_.transform_param();」

# 在第 89 行处添加以下语句:   
if (transform_param.has_distort_param())
{
	this->data_transformer_->DistortImage(datum, &datum);
}
```
然后重新编译 ssd 源码(make -j8).   
2. 要使用上面的增广方式需要在创建 lmdb 的时候指定下面的命令行参数.    
```
--encode_type=jpg --encoded   
```
3. 最后需要在 train_val.prototxt 文件的 transform_param 参数中指定 distort_param 参数即可.   
```
transform_param {
	distort_param {
		brightness_prob: 0.5
		brightness_delta: 32.0
		contrast_prob: 0.5
		contrast_lower: 0.5
		contrast_upper: 1.5
		hue_prob: 0.5
		hue_delta: 18.0
		saturation_prob: 0.5
		saturation_lower: 0.5
		saturation_upper: 1.5
		random_order_prob: 0.0
	}

	mirror: true
	crop_size: 224
	mean_file: "/home/lsn/data/jst_full/mean.binaryproto"
}
```