# Ubuntu 清理内存命令     

## 1. 检查内存使用情况     

~~~bash
watch -n 3 free -m
watch -n 3 cat /proc/meminfo
~~~
 
## 2. 清理    

~~~bash
# 释放页缓存
echo 1 > /proc/sys/vm/drop_caches
# 腾出 Dentries 和 Inode
echo 2 > /proc/sys/vm/drop_caches
# 释放页面缓存，工具和索引节点
echo 3 > /proc/sys/vm/drop_caches

# 刷新文件系统缓冲区
sync
~~~
  

## 3. 释放内存空间   

~~~bash
sudo sysctl -w vm.drop_caches=3
~~~

