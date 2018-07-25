# Ubuntu 14.04 下设置静态 IP   

一开始我按照网上的步骤进行配置静态IP，方法如下 ：  

## 1、vi /etc/network/interfaces   

添加内容：

~~~bash
auto eth0
iface eth0 inet static
address 192.168.1.180    
netmask 255.255.255.0
gateway 192.168.1.1
dns-nameservers 8.8.8.8
~~~

`dns-nameservers 8.8.8.8` 这句一定需要有，因为以前是 DHCP 解析，所以会自动分配 DNS 服务器地址。

而一旦设置为静态 IP 后就没有自动获取到 DNS 服务器了，需要自己设置一个.   

设置完重启电脑后， /etc/resolv.conf 文件中会自动添加 nameserver 8.8.8.8.   

可以根据访问速度，选择合适的公共 DNS.    


## 2、重启网络：sudo /etc/init.d/networking restart

发现网络并不通.   

后来发现出现这个错误的原因，是因为中的网卡名称不是 eth0。具体是网卡名称可以通过 ifocnfig 命令查看，我的机子的网卡名称为enp0s25，有些机子的网卡名称可能是 ens33，将 eth0 改成自己的机子的网卡名称，然后保存，重启网络，发现成功了！大功告成。   

## 总结   

其实就是先获取自己的网卡名字，再进行配置。但是新手如果照着网上的教程照抄，就容易出现类似我这样的错误。   
