# 使用自己的数据集训练 YOLOv3   

> 这里假设数据集为 `mine`, 一共有 5 类数据;  

## 1. 创建 cfg/yolo-mine.cfg   

最快的方法是复制 cfg/yolov3.cfg 为 cfg/yolo-mine.cfg, 然后做如下修改:  

* batch=64;   
* subdivisions=8;   
* 将 3 个 [yolo]-layers 中的 classes=80 改为 classes=5;    
* 将 3 个 [yolo]-layers 前面卷积层的 filters=255((80+5)*3) 改为 filters=30, 计算公式为 filters=(classes + 5)x3;   

**filters=(classes + 5)x3 计算公式的由来**:   

通常情况下, filters 的个数由 classes (即类别个数), coords 个数和 masks 个数决定. 也就是说 filters=(classes + coords + 1)*<number of mask>, where mask is indices of anchors. If mask is absence, then filters=(classes + coords + 1)*num)

例如,对于有 5 类目标的 yolo-mine.cfg 而言, 3 个 [yolo]-layers 分别是:  

~~~conf
[convolutional]
filters=21

[region]
classes=5
~~~

## 2. 创建 data/mine.names   

每个类别占一行.   

## 3. 创建 cfg/mine.data 

~~~conf
classes= 5
train  = <path-to-data>/mine/train.txt
valid  = <path-to-data>/mine/test.txt
names = data/mine.names
backup = backup/
~~~

## 4. 准备数据  

将训练数据 (.jpg) 放在 <path-to-data>/mine/ 目录下.   

## ５. 准备 labels   

数据中的目标都是提前被标注好的. 标注工具可以选择[Yolo_mark](https://github.com/AlexeyAB/Yolo_mark), 用于标注 Yolo v2 & v3 的 annotation 文件.  

标注工具为每个图片文件在同一目录下生成一个对应的 .txt 文件, 文件名的前缀和图片名一致, 最终会直接生成以下格式的文件:   

~~~xml
<object-class> <x> <y> <width> <height>
~~~

其中:  

* <object-class> - 类别序号数字, 从 0 到 (classes-1);  
* <x> <y> <width> <height> - 标注框参数相对于原图宽和高的浮点数, 范围是(0.0 to 1.0]. 
* <x> = <absolute_x> / <image_width> or <height> = <absolute_height> / <image_height>
* 注意: <x> <y> - 是标注框的中心, 而不是左上角点.  

例如, 一张包含 3 个目标的标注图生成的 .txt 文件可能是:   

~~~txt
1 0.716797 0.395833 0.216406 0.147222
0 0.687109 0.379167 0.255469 0.158333
1 0.420312 0.395833 0.140625 0.166667
~~~

## 6. 生成 train.txt 文件  

文件应该位于 <path-to-data>/mine/ 目录下.   

train.txt 文件中每一行是一个数据文件的路径(这里的图片路径可以使用 ./darknet 的相对路径, 也可使用绝对路径), 比如:

~~~txt
data/mine/img1.jpg
data/mine/img2.jpg
data/mine/img3.jpg
~~~

## 7. 下载预训练模型   

预训练模型可以很好的对初始化权重参数, 下载地址: [convolutional layers (154 MB)](https://pjreddie.com/media/files/darknet53.conv.74).   

## 8. 开始训练   

~~~bash
sudo ./darknet detector train data/mine.data yolo-mine.cfg darknet53.conv.74
~~~


(file yolo-mine_xxx.weights will be saved to the build\darknet\x64\backup\ for each 100 iterations) (
    

如果使用不带显示器的电脑(比如 Amazaon EC2 云服务器), 可以关闭 loss 显示:   

~~~bash
sudo ./darknet detector train data/mine.data yolo-mine.cfg darknet53.conv.74 -dont_show 
~~~

## 9. 训练结束后测试  
 
训练结束后会在 backup/ 目录下得到 yolo-mine_final.weights 文件, 使用下面的命令进行测试.   

~~~bash
sudo ./darknet detector test data/mine.data yolo-mine.cfg yolo-mine_final.weights -thresh 0.25
~~~

yolo 也支持断点训练. 比如你训练了 2000 次之后停止, 之后再训练的时候只需要:   

~~~bash
sudo ./darknet detector train data/mine.data yolo-mine.cfg yolo-mine_2000.weights
~~~

## 总结 & 注意事项    

1: 如果训练过程中发现 avg (loss) 出现了 nan 值, 说明训练过程中出错了, 如果 nan 值在其他行, 则训练正常.  

2: 如果要改变 cfg/yolo-mine.cfg 文件中的 width= 或 height= 选项时, 那么设置后的 width 和 height 必须可以被 32 整除.  

3: 如果总是出现内存不足( Out of memory)错误, 则应该增大 cfg/yolo-mine.cfg 配置文件中的 subdivisions=16, 32 或 64.  
