# 如何提高 Yolo 的检测性能  

## Before training

* .cfg 文件中设置 `random=1` - 可以提高不同分辨率下的检测精度;   
* .cfg 文件中提高图像分辨率, (如 height=608, width=608 或其他 32 倍数的值); 
* Ensure that your training dataset include images with objects at diffrent: scales, rotations, lightings, from different sides, on different backgrounds  
* Ensure that your training dataset include images with non-labeled objects that you do not want to detect - negative samples without bounded box (empty .txt files)   

* [yolov3 中不支持] for training with a large number of objects in each image, add the parameter max=200 or higher value in the last layer [region] in your cfg-file  
* [yolov3 中不支持] to speedup training (with decreasing detection accuracy) do Fine-Tuning instead of Transfer-Learning, set param stopbackward=1 in one of the penultimate convolutional layers before the 1-st [yolo]-layer, for example here: https://github.com/AlexeyAB/darknet/blob/0039fd26786ab5f71d5af725fc18b3f521e7acfd/cfg/yolov3.cfg#L598  
* [yolov3 中不支持]在 .cfg 文件中设置的宽和高条件下, 重新计算数据集的 anchors,  
~~~bash
sudo ./darknet detector calc_anchors data/obj.data -num_of_clusters 9 -width 416 -height 416 
~~~
then set the same 9 anchors in each of 3 [yolo]-layers in your cfg-file

## After training - for detection  

Increase network-resolution by set in your .cfg-file (height=608 and width=608) or (height=832 and width=832) or (any value multiple of 32) - this increases the precision and makes it possible to detect small objects: link

you do not need to train the network again, just use .weights-file already trained for 416x416 resolution
if error Out of memory occurs then in .cfg-file you should increase subdivisions=16, 32 or 64: link


## How to use Yolo as DLL

To compile Yolo as C++ DLL-file yolo_cpp_dll.dll - open in MSVS2015 file build\darknet\yolo_cpp_dll.sln, set x64 and Release, and do the: Build -> Build yolo_cpp_dll

You should have installed CUDA 9.1
To use cuDNN do: (right click on project) -> properties -> C/C++ -> Preprocessor -> Preprocessor Definitions, and add at the beginning of line: CUDNN;
To use Yolo as DLL-file in your C++ console application - open in MSVS2015 file build\darknet\yolo_console_dll.sln, set x64 and Release, and do the: Build -> Build yolo_console_dll

you can run your console application from Windows Explorer build\darknet\x64\yolo_console_dll.exe
or you can run from MSVS2015 (before this - you should copy 2 files yolo-voc.cfg and yolo-voc.weights to the directory build\darknet\ )
after launching your console application and entering the image file name - you will see info for each object: <obj_id> <left_x> <top_y> <width> <height> <probability>
to use simple OpenCV-GUI you should uncomment line //#define OPENCV in yolo_console_dll.cpp-file: link
you can see source code of simple example for detection on the video file: link
