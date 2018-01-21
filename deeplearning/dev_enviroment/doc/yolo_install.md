# yolo 环境配置  
> 平台: Ubuntu 14.04   

YOLO: Real-Time Object Detection    

You only look once (YOLO) is a state-of-the-art, real-time object detection system. On a Titan X it processes images at 40-90 FPS and has a mAP on VOC 2007 of 78.6% and a mAP of 48.1% on COCO test-dev.


Model	Train	Test	mAP	FLOPS	FPS	Cfg	Weights
Old YOLO	VOC 2007+2012	2007	63.4	40.19 Bn	45	link
SSD300	VOC 2007+2012	2007	74.3	-	46	link
SSD500	VOC 2007+2012	2007	76.8	-	19	link
YOLOv2	VOC 2007+2012	2007	76.8	34.90 Bn	67	cfg	weights
YOLOv2 544x544	VOC 2007+2012	2007	78.6	59.68 Bn	40	cfg	weights
Tiny YOLO	VOC 2007+2012	2007	57.1	6.97 Bn	207	cfg	weights
SSD300	COCO trainval	test-dev	41.2	-	46	link
SSD500	COCO trainval	test-dev	46.5	-	19	link
YOLOv2 608x608	COCO trainval	test-dev	48.1	62.94 Bn	40	cfg	weights
Tiny YOLO	COCO trainval	-	-	7.07 Bn	200	cfg	weights
How It Works

Prior detection systems repurpose classifiers or localizers to perform detection. They apply the model to an image at multiple locations and scales. High scoring regions of the image are considered detections.

We use a totally different approach. We apply a single neural network to the full image. This network divides the image into regions and predicts bounding boxes and probabilities for each region. These bounding boxes are weighted by the predicted probabilities.



Our model has several advantages over classifier-based systems. It looks at the whole image at test time so its predictions are informed by global context in the image. It also makes predictions with a single network evaluation unlike systems like R-CNN which require thousands for a single image. This makes it extremely fast, more than 1000x faster than R-CNN and 100x faster than Fast R-CNN. See our paper for more details on the full system.

What's New in Version 2?

YOLOv2 uses a few tricks to improve training and increase performance. Like Overfeat and SSD we use a fully-convolutional model, but we still train on whole images, not hard negatives. Like Faster R-CNN we adjust priors on bounding boxes instead of predicting the width and height outright. However, we still predict the x and y coordinates directly. The full details are in our paper.!

Detection Using A Pre-Trained Model

This post will guide you through detecting objects with the YOLO system using a pre-trained model. If you don't already have Darknet installed, you should do that first. Or instead of reading all that just run:
## 1. 下载编译   
```
git clone https://github.com/pjreddie/darknet
cd darknet
```
1.1 使用 CPU 版   
不用对 Makefile 做任何修改. 直接运行下面的命令.    
```
make -j8
```
Easy!
1.2 使用 GPU 版   
需要修改 Makefile 文件.   
```
# 将 GPU=0 改为 GPU=1, 使用 GPU
GPU=1
# 将 CUDNN=0 改为 CUDNN=1, 使用 cudnn 库加速
CUDNN=1
# 将 OPENCV=0 改为 OPENCV=1, 使用 opencv
OPENCV=1
# 将 OPENMP=0 改为 OPENMP=1, 使用多线程
OPENMP=1
```
然后运行下面的命令开始编译.    
```
make -j8
```
编译完成后会得到 darknet 可执行文件.   

## 2. 使用 yolo 中的 demo    
2.1 一张图片的测试   
`cfg/` 目录下放置的是 yolo 的配置文件. 在 [这里 (258 MB)](https://pjreddie.com/media/files/yolo.weights)下载 预训练好的权重文件. 或者直接使用命令行下载:   
```bash
wget https://pjreddie.com/media/files/yolo.weights
```
开始运行 detector!   
```bash
./darknet detect cfg/yolo.cfg yolo.weights data/dog.jpg
```
程序运行输出如下:   
```
layer     filters    size              input                output
    0 conv     32  3 x 3 / 1   416 x 416 x   3   ->   416 x 416 x  32
    1 max          2 x 2 / 2   416 x 416 x  32   ->   208 x 208 x  32
    .......
   29 conv    425  1 x 1 / 1    13 x  13 x1024   ->    13 x  13 x 425
   30 detection
Loading weights from yolo.weights...Done!
data/dog.jpg: Predicted in 0.016287 seconds.
car: 54%
bicycle: 51%
dog: 56%
````
Darknet 打印检测出的目标,置信度和检测消耗的时间.如果编译时没有在 Makefile 中设置 OPENCV=1, 那么就不会直接显示检测框.但是它会将检测的预测结果保存在图片 `predictions.png` 中.   
上面的 demo 中使用的图片是 data/dog.jpg, 在 darknet 中还提供了 data/eagle.jpg, data/dog.jpg, data/person.jpg 和 data/horses.jpg, Try them!   
另外, 上面的命令是简化版本,完整的命令应该是:   
```bash
./darknet detector test cfg/coco.data cfg/yolo.cfg yolo.weights data/dog.jpg
```
如果是使用 webcam 做测试的话, 就应该使用完整的命令.    
2.2 多张图片    
将最后一个参数 data/dog.jpg 空着不填就可以连续检测多张图. 在配置文件和权重加载完毕之后会看到输入图片路径的提示.   
```bash
./darknet detect cfg/yolo.cfg yolo.weights
layer     filters    size              input                output
    0 conv     32  3 x 3 / 1   416 x 416 x   3   ->   416 x 416 x  32
    1 max          2 x 2 / 2   416 x 416 x  32   ->   208 x 208 x  32
    .......
   29 conv    425  1 x 1 / 1    13 x  13 x1024   ->    13 x  13 x 425
   30 detection
Loading weights from yolo.weights ...Done!
Enter Image Path:
```
比如输入: data/horses.jpg 就可以开始预测了. 当一个预测完毕后会提示你继续输入, Ctrl-C 退出程序.   
2.3 设置检测器的阈值   
YOLO 默认只显示检测置信度大于 0.25 的目标,用户可以通过 -thresh <val> 来改变检测器的阈值. 例如想要显示所有检测到的目标时可以设置阈值为 0 :   
```bash
./darknet detect cfg/yolo.cfg yolo.weights data/dog.jpg -thresh 0
```
2.4 Tiny YOLO   
Tiny YOLO 网络基于 [Darknet reference network](https://pjreddie.com/darknet/imagenet/#reference), 并且速度比 normal YOLO model 更快(用精度换速度). 下载并使用 VOC 的 tiny-yolo 模型:   
```
wget https://pjreddie.com/media/files/tiny-yolo-voc.weights
./darknet detector test cfg/voc.data cfg/tiny-yolo-voc.cfg tiny-yolo-voc.weights data/dog.jpg
```
Which, ok, it's not perfect, but boy it sure is fast. On GPU it runs at >200 FPS.
2.5 Webcam 实时检测   
Running YOLO on test data isn't very interesting if you can't see the result. Instead of running it on a bunch of images let's run it on the input from a webcam!   

To run this demo you will need to compile Darknet with CUDA and OpenCV. Then run the command:   
```bash
./darknet detector demo cfg/coco.data cfg/yolo.cfg yolo.weights
```
YOLO will display the current FPS and predicted classes as well as the image with bounding boxes drawn on top of it.   
You will need a webcam connected to the computer that OpenCV can connect to or it won't work. If you have multiple webcams connected and want to select which one to use you can pass the flag `-c <num>` to pick (OpenCV uses webcam 0 by default).
```bash
./darknet detector demo cfg/coco.data cfg/yolo.cfg yolo.weights -c 1
```
You can also run it on a video file if OpenCV can read the video:  
```bash
./darknet detector demo cfg/coco.data cfg/yolo.cfg yolo.weights <video file>
```
That's how we made the YouTube video above.   
2.6 What Happened to the Old YOLO Site?   
If you are using YOLO version 1 you can still find the site here: https://pjreddie.com/darknet/yolov1/   
2.7 论文引用   
If you use YOLOv2 in your work please cite our paper!   
```
@article{redmon2016yolo9000,
  title={YOLO9000: Better, Faster, Stronger},
  author={Redmon, Joseph and Farhadi, Ali},
  journal={arXiv preprint arXiv:1612.08242},
  year={2016}
}
```