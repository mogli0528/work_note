# 在 Ubuntu 上安装 Nodejs 最新版本


## 1. 从 nodejs.org 下载对应的最新 linux 版本  

我下载的是: node-v8.3.1-linux-x64.tar.xz    

## 2. 执行以下命令，把 node 解压到 /usr/local   
 
~~~bash
sudo tar -C /usr/local --strip-components 1 -xJf node-v6.9.2-linux-x64.tar.xz
~~~

## 3. 确认解压后程序的安装结果   

~~~bash
ls -l /usr/local/bin/node
ls -l /usr/local/bin/npm
~~~

## 4. 安装 npm 镜像 cnpm   

~~~bash
sudo npm install -g cnpm --registry=https://registry.npm.taobao.org
~~~

其中, --registry 参数用于指定下载软件包的地址.   
