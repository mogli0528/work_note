# Ubuntu 14.04 更换软件下载源	 

>  要区分 `ubuntu` 的版本: `trusty`.    

## 163 网易源  

使用下面的行直接替换原来的地址:    
```
deb http://mirrors.163.com/ubuntu/ trusty main universe restricted multiverse
deb-src http://mirrors.163.com/ubuntu/ trusty main universe restricted multiverse
deb http://mirrors.163.com/ubuntu/ trusty-security universe main multiverse restricted
deb-src http://mirrors.163.com/ubuntu/ trusty-security universe main multiverse restricted
deb http://mirrors.163.com/ubuntu/ trusty-updates universe main multiverse restricted
deb http://mirrors.163.com/ubuntu/ trusty-proposed universe main multiverse restricted
deb-src http://mirrors.163.com/ubuntu/ trusty-proposed universe main multiverse restricted
deb http://mirrors.163.com/ubuntu/ trusty-backports universe main multiverse restricted
deb-src http://mirrors.163.com/ubuntu/ trusty-backports universe main multiverse restricted
deb-src http://mirrors.163.com/ubuntu/ trusty-updates universe main multiverse restricted
```

## 阿里云  
```
deb http://mirrors.aliyun.com/ubuntu/ trusty main restricted universe multiverse
deb http://mirrors.aliyun.com/ubuntu/ trusty-security main restricted universe multiverse
deb http://mirrors.aliyun.com/ubuntu/ trusty-updates main restricted universe multiverse
deb http://mirrors.aliyun.com/ubuntu/ trusty-proposed main restricted universe multiverse
deb http://mirrors.aliyun.com/ubuntu/ trusty-backports main restricted universe multiverse
deb-src http://mirrors.aliyun.com/ubuntu/ trusty main restricted universe multiverse
deb-src http://mirrors.aliyun.com/ubuntu/ trusty-security main restricted universe multiverse
deb-src http://mirrors.aliyun.com/ubuntu/ trusty-updates main restricted universe multiverse
deb-src http://mirrors.aliyun.com/ubuntu/ trusty-proposed main restricted universe multiverse
deb-src http://mirrors.aliyun.com/ubuntu/ trusty-backports main restricted universe multiverse
```

   