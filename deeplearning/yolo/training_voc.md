# Training YOLO on VOC   

在 Pascal VOC dataset 上训练 yolo.   

## 1. Get The Pascal VOC Data   

[这里](https://pjreddie.com/projects/pascal-voc-dataset-mirror/)是 VOC 数据集的下载地址. 创建一个新文件夹保存数据.    

~~~bash
wget https://pjreddie.com/media/files/VOCtrainval_11-May-2012.tar
wget https://pjreddie.com/media/files/VOCtrainval_06-Nov-2007.tar
wget https://pjreddie.com/media/files/VOCtest_06-Nov-2007.tar
tar xf VOCtrainval_11-May-2012.tar
tar xf VOCtrainval_06-Nov-2007.tar
tar xf VOCtest_06-Nov-2007.tar
~~~

这时在该目录下就会出现 VOCdevkit/ 文件夹, 里边包含所有的 VOC training data.   

## 2. 生成 VOC 图片数据对应的 labels  

为 Darknet 生成 label 文件. Darknet 需要一个 .txt 文件, 该文件的每一行都是一张图片和对应的 ground truth 表示.  

~~~bash
<object-class> <x> <y> <width> <height>
~~~
Where x, y, width, and height are relative to the image's width and height. To generate these file we will run the voc_label.py script in Darknet's scripts/ directory. Let's just download it again because we are lazy.   

~~~bash
wget https://pjreddie.com/media/files/voc_label.py
python voc_label.py
~~~

After a few minutes, this script will generate all of the requisite files. Mostly it generates a lot of label files in VOCdevkit/VOC2007/labels/ and VOCdevkit/VOC2012/labels/. In your directory you should see:   

~~~bash
ls
2007_test.txt   VOCdevkit
2007_train.txt  voc_label.py
2007_val.txt    VOCtest_06-Nov-2007.tar
2012_train.txt  VOCtrainval_06-Nov-2007.tar
2012_val.txt    VOCtrainval_11-May-2012.tar
~~~

The text files like 2007_train.txt list the image files for that year and image set. Darknet needs one text file with all of the images you want to train on. In this example, let's train with everything except the 2007 test set so that we can test our model. Run:   

~~~bash
cat 2007_train.txt 2007_val.txt 2012_*.txt > train.txt
~~~

Now we have all the 2007 trainval and the 2012 trainval set in one big list. That's all we have to do for data setup!    

## 3. Modify Cfg for Pascal Data   

Now go to your Darknet directory. We have to change the cfg/voc.data config file to point to your data:   

~~~conf
classes= 20
train  = <path-to-voc>/train.txt
valid  = <path-to-voc>/2007_test.txt
names = data/voc.names
backup = backup
~~~

You should replace <path-to-voc> with the directory where you put the VOC data.   

修改 yolo-voc.cfg 文件. 这里主要修改 batch 和 subdivisions, 记住是在 `# Training` 阶段.   

~~~conf
[net]
# Testing
# batch=1
# subdivisions=1
# Training
batch=64
subdivisions=8
....
~~~

## 4. Download Pretrained Convolutional Weights   

For training we use convolutional weights that are pre-trained on Imagenet. We use weights from the Extraction model. You can just download the weights for the convolutional layers [here (76 MB)](https://pjreddie.com/media/files/darknet53.conv.74).   

~~~bash
wget https://pjreddie.com/media/files/darknet53.conv.74
~~~

## 5. Train The Model    

Now we can train! Run the command:   

~~~bash
./darknet detector train cfg/voc-klm.data cfg/yolov3-voc-klm.cfg /home/klm/data_training/darknet/voc/darknet53.conv.74
./darknet detector train cfg/voc-klm.data cfg/yolov3-voc-klm.cfg /home/klm/data/darknet/voc/darknet53.conv.74
~~~

## 6. Training YOLO on COCO  

You can train YOLO from scratch if you want to play with different training regimes, hyper-parameters, or datasets. Here's how to get it working on the [COCO dataset](http://mscoco.org/dataset/#overview).   
6.1 Get The COCO Data   
To train YOLO you will need all of the COCO data and labels. The script `scripts/get_coco_dataset.sh` will do this for you. Figure out where you want to put the COCO data and download it, for example:   

~~~bash
cp scripts/get_coco_dataset.sh data
cd data
bash get_coco_dataset.sh
~~~

Now you should have all the data and the labels generated for Darknet.   
6.2 Modify cfg for COCO   
Now go to your Darknet directory. We have to change the cfg/coco.data config file to point to your data:   

~~~conf
classes= 80
train  = <path-to-coco>/trainvalno5k.txt
valid  = <path-to-coco>/5k.txt
names = data/coco.names
backup = backup
~~~

You should replace <path-to-coco> with the directory where you put the COCO data.   
You should also modify your model cfg for training instead of testing. `cfg/yolo.cfg` should look like this:

~~~conf
[net]
# Testing
# batch=1
# subdivisions=1
# Training
batch=64
subdivisions=8
....
~~~

6.3 Train The Model   

Now we can train! Run the command:   

~~~bash
./darknet detector train cfg/coco.data cfg/yolo.cfg darknet19_448.conv.23
~~~

If you want to use multiple gpus run:   

~~~bash
./darknet detector train cfg/coco.data cfg/yolo.cfg darknet19_448.conv.23 -gpus 0,1,2,3
~~~

If you want to stop and restart training from a checkpoint:   

~~~bash
./darknet detector train cfg/coco.data cfg/yolo.cfg backup/yolo.backup -gpus 0,1,2,3
~~~