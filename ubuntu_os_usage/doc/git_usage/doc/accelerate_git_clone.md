# 加速 git clone xxx_url   

上网查找改善方法解决了问题：

思路：git clone 特别慢是因为github.global.ssl.fastly.Net域名被限制了。只要找到这个域名对应的ip地址，然后在hosts文件中加上ip–>域名的映射，刷新DNS缓存就可以了

步骤：

1、查询域名 global-ssl.fastly.Net 和 github.com 公网地址.

可以用这个 https://www.ipaddress.com/ 查。或者用 [Dns检测|Dns查询 - 站长工具](http://tool.chinaz.com/dns/?type=1&host=github.global.ssl.fastly.net&ip=), 查找检测列表里的 TTL 值最小的 IP.  

分别查找

github.global.ssl.fastly.net
github.com

这两个域名的 ip 地址.

2、将ip地址添加到hosts文件

~~~conf
sudo gedit /etc/hosts
# 写入下面两行
192.30.253.112 github.com
151.101.185.194 github.global.ssl.fastly.Net
~~~

保存退出

3、

修改完hosts还不会立即生效，你需要刷新DNS缓存，告诉电脑我的hosts文件已经修改了。

输入指令：sudo /etc/init.d/networking restart 即可。



windows下刷新DNS的方法：

打开CMD

输入ipconfig /flushdns


4、重新运行git clone



此方法也可以解决访问github网站慢的问题