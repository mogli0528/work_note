# 编译 caffe/ssd 源码中提供的文档   

`gem install jekyll` 依赖于 ruby 2.0.   

## 1. 安装 ruby2.2(使用brightbox ppa仓库安装)    

这是一个repo方案，已经更新到2.4版本，参考：https://www.brightbox.com/docs/ruby/ubuntu/    
If you’re using Ubuntu 14.04 (Trusty) or newer then you can add the package repository like this   
```bash
$ sudo apt-get install software-properties-common
$ sudo apt-add-repository ppa:brightbox/ruby-ng
$ sudo apt-get update
$ sudo apt-get install ruby2.2 ruby2.2-dev
```
如果出现以下错误:   
```
can't find header files for ruby at /usr/lib/ruby/include/ruby.h
```
说明是没有安装 ruby2.2-dev.    
```bash
$ sudo apt-get install ruby2.2-dev
```

## 2. 安装Jekyll环境   
在终端中输入如下命令安装 `Jekyll`，这个过程比较慢，和源有关：   
```bash
sudo gem install jekyll
```
安装完成后，在终端中输入如下命令，验证 `jekyll` 安装是否成功：   
```bash
jekyll new myblog 
```

## 3. 编译文档并使用   
```
$ ./scripts/build_docs.sh 

usage: build_docs.sh [port]

Configuration file: ./_config.yml
            Source: .
       Destination: _site
 Incremental build: disabled. Enable with --incremental
      Generating... 
                    done in 0.697 seconds.
 Auto-regeneration: enabled for '.'
    Server address: http://127.0.0.1:4000
  Server running... press ctrl-c to stop.
```
根据提示可知,服务器地址为 `http://127.0.0.1:4000`. 