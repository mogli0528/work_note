# 自定义训练的数据准备  

## 1. 标注数据   

使用 label_tool.py 工具对数据集进行标注, 标注完成后得到 .xml 文件和 .xml.txt 文件;   

其中, .xml.txt 文件是标注工具用于缓存标注信息的; 我们将使用 .xml 文件生成训练 labels.  


## 2. 生成 train.txt 和 labels

这个文件所在的路径在 cfg/miners.data 配置文件中指定;  

使用 custom_data_labels.py 文件生成 train.txt 文件和 labels.  

