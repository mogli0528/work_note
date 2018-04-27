# 安装 pytorch   

> 系统环境:    

- Ubuntu 1404   
- python 3.5   
- pip3   

## 1. 准备工作  
确保你的 python3 版本为 Python 3.5 及以上. 同时 pip3 指向的是 python3.5. 使用下面的命令:    
```bash
$ python3 -V
Python 3.5.3

$ sudo pip3 -V
pip 10.0.1 from /usr/local/lib/python3.5/dist-packages/pip (python 3.5)
```

## 2. 一键安装      
新的安装命令已经不需要区分 python 版本和 cuda 版本了.   
```bash
sudo pip3 install torch torchvision
```

## error   
1. 安装 scimage 出错.    
```
Cannot uninstall 'six'. It is a distutils installed project and thus we cannot accurately determine which files belong to it which would lead to only a partial uninstall.
```
那么可以选择不安装 six.      
```bash
sudo pip3 install scikit-image --ignore-installed six   
```