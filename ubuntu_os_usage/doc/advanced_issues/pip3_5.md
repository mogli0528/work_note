# 将 pip3 指向 python3.5   
很多深度学习包都需要 python 3.5 及以上的版本, 而 Ubuntu 14.04 默认安装的 python3 是 python 3.4, 因此在安装深度学习环境的时候会出现很多问题. 这就需要将 python3.4 换为 python3.5.    

## 查看版本  
假设已经安装好了 python 3.5, 但是默认的 python3 还是 python3.4.    
```bash
python3 -V
```
Python 3.4.3   

```bash
python3.5 -V
```
Python 3.5.3   

## 将 python3 指向 python3.5   
```bash
sudo rm /usr/bin/python3
sudo ln -s /usr/bin/python3.5 /usr/bin/python3

python3 -V
```
Python 3.5.3

## 将 pip3 也指向 python 3.5   
输入下面的命令:   
```bash
sudo pip3 -V
```
pip 10.0.1 from /usr/local/lib/python3.4/dist-packages/pip (python 3.4)   
可以看到, pip3 默认是指向 python 3.4 的.   
1. 卸载 pip3.    
```
sudo pip3 uninstall pip3
```
2. 重新安装 pip3.    
```cpp
wget https://bootstrap.pypa.io/get-pip.py  
sudo python3.5 get-pip.py 

sudo pip3 install setuptools --upgrade  
sudo pip3 install ipython[all]  
```
3. 再次查看 pip3.    
```bash
pip3 -V
```
pip 10.0.1 from /usr/local/lib/python3.5/dist-packages/pip (python 3.5)
可以看到, pip3 已经指向 python 3.5.   
