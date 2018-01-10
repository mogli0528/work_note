# caffe/ssd 中的脚本列表    
如未特别说明,则表示该脚本在 caffe/ssd 中是通用的.   

|脚本名 | 功能|
|-----|-----|
| build_docs.sh | 编译 caffe/ssd 文档,可以在浏览器中显示 |
| gather_examples.sh | 将 `build_docs.sh` 中用到的文档收集起来 |
| parse_log.py | 将训练得到的 .log 文件解析为 两个.csv 文件(train 和 test) |
| resize_and_crop_images.py | resize 和 crop 图片的 center square |
| launch_resize_and_crop_images.sh | 一个调用 `resize_and_crop_images.py` 的例子 |
| plot_training_log.py.example | 根据 .log 文件绘制训练的曲线图 |

**Note**
1. 关于 `plot_training_log.py.example`   
这个脚本只是一个基本脚本,因此需要定制化使用.
将来的 .log 文件中一些 field 可能会和现在的不同,注意将这部分内容考虑进去.   
```
Usage:   
    ./plot_training_log.py chart_type[0-%s] /where/to/save.png /path/to/first.log ...   
Notes:   
    1. Supporting multiple logs.   
    2. Log file name must end with the lower-cased.   
```   
