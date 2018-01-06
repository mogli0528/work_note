# 一些有用的小工具  

根据使用中积累起来的有用脚本，尝试使用 python 解决工程中遇到的问题。  

## 1 To Training Better   
[1. 统计一个训练样本的分布情况](./doc/code/train_eassy/calc_examples_amount.py)   
[2. 将 SSD 中标注好的图片分类裁剪为 ROI](./doc/code/train_eassy/extractROIWithXml.py)   
[3. 从大量的标注样本中选取固定数量样本](./doc/code/train_eassy/copy_fixed_amount_files.py)  
[4. 在图片上将标注好的框显示出来](./doc/code/train_eassy/drawRectWithXml.py)  
[5. 老杜的 lab 标注工具](./doc/code/lab.py)  
[6. 数据增广小工具](./doc/code/opencv/data_augment.py)  
[7. 删除经过 ROI 挑选后存在错误的样本](./doc/code/train_eassy/delete_error_files.py)  
[8. 使用新标注的样本完全替换原来的样本(+rename)](./doc/code/train_eassy/remove_bad_data_dirs.py)  

## 2 xml文件解析   
[1. 解析 `xml` 文件](./doc/code/parse_xml/parse_xml.py)   
[2. `pycaffe`分类器实例](./doc/code/pycaffe/classify.py)  
[3. 吴恩达`python numpy`使用](./doc/code/numpy/numpyAndrewNG.py)  
[4. 将视频图片截为图片](./doc/code/opencv/splitVideoToImages.py)  

## 3 摄像头操作   
[1. 摄像头分辨率设置和'MPEG'格式下的帧率测试](./doc/code/opencv/camera_res_or_fps.cpp)   
[2. 将.avi格式的视频转换为.mp4格式](./doc/code/opencv/video_format_convert.py)   
[3. 读取摄像头的硬件参数信息](./doc/code/read_camera_hardware_info.cpp)   

## 4 caffemodel 的使用
[1. ssd 和 caffe 模型的使用示例](./doc/code/ssd_call.cpp)   


## 导数求解    
[1. 导数求解](./doc/code/derive.py)   

![](doc/caffe_layers.png)
