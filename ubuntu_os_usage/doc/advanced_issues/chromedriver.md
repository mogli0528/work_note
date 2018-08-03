# 在 Ubuntu 上安装 ChromeDriver

## 一、安装 Chrome 浏览器

1、安装依赖  

~~~bash
sudo apt-get install libxss1 libappindicator1 libindicator7
~~~

2、下载Chrome安装包  

~~~bash
wget https://dl.google.com/linux/direct/google-chrome-stable_current_amd64.deb
~~~

3、安装  

~~~bash
sudo dpkg -i google-chrome*.deb
sudo apt-get install -f
~~~

## 二、安装 ChromeDriver
 
1、安装xvfb以便我们可以无头奔跑地运行Chrome  

~~~bash
sudo apt-get install xvfb
~~~

2、安装依赖  
~~~bash
sudo apt-get install unzip
~~~

3、下载安装包  

一定要下载和浏览器版本对应的 chromedriver.   

> chromedriver 地址: http://npm.taobao.org/mirrors/chromedriver   
> 版本对照表:  https://blog.csdn.net/u013274435/article/details/78973353   

~~~cpp
wget -N http://chromedriver.storage.googleapis.com/2.26/chromedriver_linux64.zip
~~~

4、解压缩+添加执行权限  

~~~bash
unzip chromedriver_linux64.zip
~~~

5、移动  

~~~bash
sudo mv -f chromedriver /usr/local/share/chromedriver
~~~

6、建立软连接   

~~~cpp
sudo ln -s /usr/local/share/chromedriver /usr/local/bin/chromedriver
sudo ln -s /usr/local/share/chromedriver /usr/bin/chromedriver
~~~

## 三、无头运行Chrome

1、安装Python依赖   

~~~bash
pip3 install selenium
pip3 install pyvirtualdisplay
~~~

2、开干   
~~~python
from pyvirtualdisplay import Display
from selenium import webdriver
 
display = Display(visible=0, size=(800, 600))　　# 初始化屏幕
display.start()　　
driver = webdriver.Chrome()　　# 初始化Chrome
driver.get('http://www.cnblogs.com/x54256/')
print driver.title
~~~