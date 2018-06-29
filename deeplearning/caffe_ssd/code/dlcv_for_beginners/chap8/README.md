## Prepare Data For Caffe

### step 1
> ./download_mnist.sh

下载 mnist.pkl.gz， 然后运行 *convert_mnist.py* 将 pickle 格式的数据转换为图片.  

如果原链接无法下载可以到下面云盘地址下载：http://pan.baidu.com/s/1bHmm7s

### step 2 生成图片-label 信息

```bash
python3 gen_caffe_imglist.py mnist/train train.txt
python3 gen_caffe_imglist.py mnist/val val.txt
python3 gen_caffe_imglist.py mnist/test test.txt
```

### step 3 产生 lmdb 

得到图片的列表，然后运行：  

```bash
/path/to/caffe/build/tools/convert_imageset ./ train.txt train_lmdb --gray --shuffle
/path/to/caffe/build/tools/convert_imageset ./ val.txt val_lmdb --gray --shuffle
/path/to/caffe/build/tools/convert_imageset ./ test.txt test_lmdb --gray --shuffle
```

## Prepare Data For MXNet 

### step 1 

同上;   

### step 2 生成

```bash
python3 gen_mxnet_imglist.py mnist/train train.lst
python3 gen_mxnet_imglist.py mnist/val val.lst
python3 gen_mxnet_imglist.py mnist/test test.lst
```
### step 3 产生 ImageRecordio 

用于产生图片的列表，然后运行:  

```bash
/path/to/mxnet/bin/im2rec train.lst ./ train.rec color=0
/path/to/mxnet/bin/im2rec val.lst ./ val.rec color=0
/path/to/mxnet/bin/im2rec test.lst ./ test.rec color=0
```

产生 ImageRecordio 文件.  

## MXNet

运行 *train_lenet5.py* 训练模型

运行 *score_model.py* 在测试集上评估模型

运行 *benchmark_model.py* 测试模型性能

运行 *recognize_digit.py* 跟图片路径作为参数用于手写数字图片识别

## Caffe
*lenet_train_val.prototxt* & *lenet_train_val_aug.prototxt* 分别是用原始数据和增加后数据训练模型的网络结构和数据定义文件

*lenet_solver.prototxt* & *lenet_solver_aug.prototxt* 分别是训练原始数据和增加后数据的solver文件

*lenet_test.prototxt* 是用于在测试数据上测试模型的网络结构和数据源定义文件

*lenet.prototxt* 是用于部署的网络结构定义文件

运行 *recognize_digit.py* 接测试文件的列表用来演示手写数字图片识别

## Train Caffe Model  

```bash
/path/to/caffe/build/tools/caffe train -solver lenet_solver.prototxt -gpu 0 -log_dir log_dir
```