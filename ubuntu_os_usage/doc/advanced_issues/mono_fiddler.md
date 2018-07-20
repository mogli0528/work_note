# 配置 Fiddler 抓包工具

> 自己测试时, 发现 ubunut 下的 https 网站无法抓包.  

## 一、安装Mono环境，下载地址

~~~bash
sudo apt-get install mono-complete
~~~

## 二、下载ＭonoFiddler包：http://fiddler.wikidot.com/mono，点击 Current Linux build 右侧下载地址  

![](https://raw.githubusercontent.com/magic428/work_note/master/ubuntu_os_usage/pictures/mono_fiddler.png) 


## 三、解压文件，配置 bash 执行   

~~~bash
sudo unzip  MonoFiddler-v4484.zip -d /usr/local/lib/MonoFiddler
sudo sh -c 'echo "nohup sudo /usr/bin/mono /usr/local/lib/MonoFiddler/app/Fiddler.exe > /dev/null 2>&1 &" > /usr/local/bin/fiddler'
sudo chmod +x /usr/local/bin/fiddler
source ~/.bashrc
~~~

## 四、执行fiddler

~~~bash
sudo fiddler
~~~

## 五、配置Fiddler  

1. 在 Tools->Fiddler Options 如下，勾上 Allow remote computers to connect，允许远程计算机链接.  

![](https://raw.githubusercontent.com/magic428/work_note/master/ubuntu_os_usage/pictures/mono_fiddler_config_1.png) 


2. 鼠标移动fiddler右上角电脑图标上，查看当前ip地址，端口默认8888（如上图所示）.  

![](https://raw.githubusercontent.com/magic428/work_note/master/ubuntu_os_usage/pictures/mono_fiddler_config.png) 

## 参考

http://www.jianshu.com/p/4505c732e378
http://fiddler.wikidot.com/mono
http://blog.csdn.net/u013372487/article/details/50590712


file:////root/Fiddler2/Scripts/BrowserPAC.js

/home/klm/Fiddler2/Scripts/BrowserPAC.js