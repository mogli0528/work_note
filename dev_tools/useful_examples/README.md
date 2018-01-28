# 一些有用的小工具  

根据使用中积累起来的有用脚本，尝试使用 python 解决工程中遇到的问题。  

## 1 To Training Better   
[1. 统计一个训练样本的分布情况](./doc/code/train_eassy/calc_examples_amount.py)   
[2. 将 SSD 中标注好的图片分类裁剪为 ROI](./doc/code/train_eassy/extractROIWithXml.py)   
[3. 从大量的标注样本中选取固定数量样本](./doc/code/train_eassy/copy_fixed_amount_files.py)  
[4. 在图片上将标注好的框显示出来](./doc/code/train_eassy/drawRectWithXml.py)  
[5. 老杜的 lab 标注工具](./doc/code/lab.py)  
[6. 数据增广小工具](./doc/code/opencv/image_augmentation.py)  
[7. 删除经过 ROI 挑选后存在错误的样本](./doc/code/train_eassy/delete_error_files.py)  
[8. 使用新标注的样本完全替换原来的样本(+rename)](./doc/code/train_eassy/replace_bad_data_dirs.py)  
[9. 从 JPEGImages 和 Annotations 目录中恢复 class](./doc/code/train_eassy/recover_classes_from_JPEGImages.py)  
[10. 向现有的样本库中添加补录的数据 ](./doc/code/train_eassy/add_data_for_classification.py)  
[11. 使用旧的 label_txt 文件生成新的 label_txt 文件 ](./doc/code/train_eassy/generate_new_label_txt.py)  
[12. 将满足数量的类别数据拷贝出来 ](./doc/code/train_eassy/copy_availiable_data.py)  
[13. 提取一定数量的数据 ](./doc/code/train_eassy/calc_amount_and_extract_fixed_amount_data.py)  
[14. 结算台样本增广小工具 ](./doc/code/train_eassy/jst_augmentation.py)  
[15. 统计样本数量并排序 ](./doc/code/train_eassy/list_files_amount.py)  
[16. 合并两个人挑选后的样本 ](./doc/code/train_eassy/merge_two_data.py)  
[17. 裁剪背景样本 ](./doc/code/train_eassy/crop_background.py)  
[18. 从 voc 中提取固定数量的样本(jpg和xml) ](./doc/code/train_eassy/extract_fixed_files_in_VOC.py)  
[19. 删除经过 ROI 挑选后存在错误的样本(v2.0) ](./doc/code/train_eassy/delete_error_files_v2.py)  
[20. 删除多余的 xml 或 jpg 文件 ](./doc/code/train_eassy/remove_odd_jpgs_xmls.py)  

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
