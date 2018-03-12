# caffe/ssd 中的脚本列表    
如未特别说明,则表示该脚本在 caffe 或 ssd 中是通用的.   

## 所有脚本列表    

|脚本名 | 功能|
|-----|-----|
| build_docs.sh | 编译 caffe/ssd 文档,可以在浏览器中显示 |
| gather_examples.sh | 将 `build_docs.sh` 中用到的文档收集起来 |
| parse_log.py | 将训练得到的 .log 文件解析为 两个.csv 文件(train 和 test) |
| resize_and_crop_images.py | resize 和 crop 图片的 center square |
| launch_resize_and_crop_images.sh | 一个调用 `resize_and_crop_images.py` 的例子 |
| plot_training_log.py.example | 根据 .log 文件绘制训练的曲线图 |

## 脚本功能详细介绍    
1. 关于 `plot_training_log.py.example`   
这个脚本只是一个基本脚本, 直接复制一份为 plot_training_log.py 就可以直接使用.
将来的 .log 文件中一些 field 可能会和现在的不同, 注意将这部分内容考虑进去.   
**Usage:**    
```bash
python plot_training_log.py chart_type[0-%s] /where/to/save.png /path/to/first.log ...   
```   
其中:  
- chart_type 是图的类型;   
- 第二个参数是生成图片的保存路径;  
- 第三个参数是 log 文件的路径;   
**Notes:**  
(1) 支持绘制多个 log 文件的 loss 曲线.   
(2) Log 文件名的后缀一定是小写字母的 .log .   
