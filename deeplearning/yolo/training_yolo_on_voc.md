# 使用 VOC 数据集训练 YOLOv3   

> https://pjreddie.com/darknet/yolo/

在 Pascal VOC dataset 上训练 yolov3.   

## 1. 获取 VOC 数据集  

[这里](https://pjreddie.com/projects/pascal-voc-dataset-mirror/) 是 VOC 数据集的下载地址.   

创建一个新文件夹 `~/data/voc` 保存数据, 执行下面的代码下载数据集.   

~~~bash
mkidr ~/data/voc
cd ~/data/voc
wget https://pjreddie.com/media/files/VOCtrainval_11-May-2012.tar
wget https://pjreddie.com/media/files/VOCtrainval_06-Nov-2007.tar
wget https://pjreddie.com/media/files/VOCtest_06-Nov-2007.tar
tar xf VOCtrainval_11-May-2012.tar
tar xf VOCtrainval_06-Nov-2007.tar
tar xf VOCtest_06-Nov-2007.tar
~~~

这时在该目录下就会出现 VOCdevkit/ 文件夹, 里边包含所有的 VOC 数据集.   

## 2. 生成 VOC 图片数据对应的 labels  

训练 darknet 需要一个 .txt 文件, 该文件的每一行都是图片类别和对应的 ground truth, 其格式如下: 

~~~bash
<object-class> <x> <y> <width> <height>
# eg.
14 0.509915014164 0.51 0.974504249292 0.972
~~~ 

运行下面的命令可以为 darknet 生成 labels 文件.   

~~~bash
wget https://pjreddie.com/media/files/voc_label.py
python voc_label.py
~~~

其实, voc_label.py 脚本文件在 darknet 工程的 scripts/ 目录下, 这里为了方便直接下载. 运行 `python voc_label.py` 脚本文件后生成的 labels 文件位于 VOCdevkit/VOC2007/labels/ 和 VOCdevkit/VOC2012/labels/ 目录下, 同时在当前目录 `~/data/voc/` 下发现多了几个 .txt 文件.   

~~~bash
ls
2007_test.txt   VOCdevkit
2007_train.txt  voc_label.py
2007_val.txt    VOCtest_06-Nov-2007.tar
2012_train.txt  VOCtrainval_06-Nov-2007.tar
2012_val.txt    VOCtrainval_11-May-2012.tar
~~~

txt 文件中最重要的信息就是 year 和 image id. darknet 的训练需要你把所有的图片数据最终都列出在一个 .txt 文件中. 在 VOC 的训练中, 除了 VOC2007 test 数据集之外, 我们用其他剩余的所有数据做训练. 运行以下命令将上面包含数据集对应的多个 .txt 文件整合成一个大的的 .txt 文件:   

~~~bash
cat 2007_train.txt 2007_val.txt 2012_*.txt > train.txt
~~~

至此, 数据准备工作完毕.  

## 3. 为 VOC 数据集修改/新建 Cfg 文件   

在 darknet 工程目录下, 修改 `cfg/voc.data` 配置文件中的数据集 path 以指向用户自己的数据集:   

~~~conf
classes= 20
train  = <path-to-voc>/train.txt
valid  = <path-to-voc>/2007_test.txt
names = data/voc.names
backup = backup
~~~

将 <path-to-voc> 替换为 VOC 数据集合在你电脑上的路径.   

然后修改 `cfg/yolov3-voc.cfg` 文件. 这里主要修改 batch 和 subdivisions, 记住是在 `# Training` 阶段.   

~~~conf
[net]
# Testing
# batch=1
# subdivisions=1
# Training
batch=64
subdivisions=16
....
~~~

TODO: 参数解释.  

在 YOLOv3 中，我们的 anchor 由 5 个变为 9 个，当然，也是由K均值产生的。每个尺度分配 3 个 anchor。其中每个尺度下每个位置预测 3 个 bbox(4 个位置输出 + 1 个 objectness + C 个类别的分数)。所以每个位置输出 (1+4+C) * 3 个值，这也就是训练时 yolov3.cfg 里最后一层的 filters 的数量。   


## 4. 下载 ImageNet 的预训练权重值模型   

ImageNet 的预训练权重值模型[下载地址  (76 MB)](https://pjreddie.com/media/files/darknet53.conv.74).   

~~~bash
wget https://pjreddie.com/media/files/darknet53.conv.74
~~~

## 5. 开始训练   

运行以下命令即可开始训练:   

~~~bash
sudo ./darknet detector train cfg/voc.data cfg/yolov3-voc.cfg darknet53.conv.74 
~~~

如果使用多个 GPUs 训练, 运行以下命令:   

~~~bash
./darknet detector train cfg/voc.data cfg/yolov3-voc.cfg darknet19_448.conv.23 -gpus 0,1,2,3
~~~

训练中止后重新开始训练:   

~~~bash
./darknet detector train cfg/voc.data cfg/yolov3-voc.cfg backup/yolo.backup -gpus 0,1,2,3
~~~

保存训练日志:  

~~~bash
sudo nohup ./darknet detector train cfg/voc.data cfg/yolov3-voc.cfg darknet53.conv.74 -gpus 0,1 >> log_dir/voc_train.log 2>&1 &

# 查看日志
tail -f log_dir/voc_train.log   
~~~  

## 6. 测试训练结果   

训练结束的两种可能:  

* 用户观察输出 log, 达到合适的参数后认为停止训练;  
* cfg/yolov3-voc.cfg 中的 `max_batches = 50200` 参数指定了训练的最大次数;  

训练结束后, 进行测试时需要修改 `cfg/yolov3-voc.cfg` 文件. 这里主要修改 batch 和 subdivisions, 记住是在 `# Testing` 阶段.   
~~~conf
[net]
# Testing
batch=1
subdivisions=1
# Training
# batch=64
# subdivisions=16
....
~~~

训练结束后得到的模型保存是 backup/yolov3-voc_final.weights, 另外测试时最好指定阈值:   

~~~bash
sudo ./darknet detector test cfg/voc.data cfg/yolov3-voc.cfg backup/yolov3-voc_final.weights /data/dog.jpg -thresh 0.25
~~~

## 7. darknet 训练过程中的打印信息  

**Note**: If during training you see nan values for avg (loss) field - then training goes wrong, but if nan is in some other lines - then training goes well.  


## 总结  

1. yolo 训练对于数据文件的获取   

* 读取 train.txt 文件中的一行, 这一行代表的是这张图片在硬盘上的绝对路径;  
* 生成的 labels 文件位于 VOCdevkit/VOC2007/labels/ 和 VOCdevkit/VOC2012/labels/ 目录下;  
* 图片的绝对路径中包含了其所属的年份和图片 id(即图片名), 因此根据绝对路径就可以在对应的 labels 目录下找到 .txt 标注文件;   
* 读取 .txt 文件就可以获得图片的 Ground-Truth 信息;   

2. 最终的模型可以检测出小的目标, 但是无法检测出大的目标  


