# 安装系统 -- 高级   

## 在安装系统的过程中使用 dos 命令转换分区格式   

- 进入DISKPART命令模式，输入 list disk回车，列出当前磁盘信息；
- 要转换磁盘 0 格式，则输入 select disk 0 回车;   
- 输入clean，删除磁盘分区；
- 输入convert mbr，回车，将磁盘转换为MBR，输入convert gpt则转为GPT；
- 最后输入exit回车退出命令提示符，返回安装界面继续安装系统。


## 让 ubuntu 搜索到 windows 

1. 确定 uefi 所在的磁盘分区.  

~~~bash
Disk /dev/sdc: 3.7 TiB, 4000787030016 bytes, 7814037168 sectors
...

Device          Start        End    Sectors   Size Type
/dev/sdc1        2048     923647     921600   450M Windows recovery environment
/dev/sdc2      923648    1128447     204800   100M EFI System
/dev/sdc3     1128448    1161215      32768    16M Microsoft reserved
/dev/sdc4     1161216  319490047  318328832 151.8G Microsoft basic data
/dev/sdc5   319490048 1548290047 1228800000   586G Microsoft basic data
/dev/sdc6  1548290048 7814037134 6265747087   2.9T Linux filesystem
~~~

可以看到 uefi 分区是在 /dev/sdc2 分区上.   

2. 将机械硬盘的 uefi 分区挂载到 /mnt/uefi_sys 目录下:   

~~~bash
sudo mount /dev/sdc2  uefi_sys/
~~~

3. 将 /mnt/uefi_sys/EFI/Microsoft/Boot/ 目录全部拷贝到已经安装好 windows 的 C 盘中, C 盘也是使用相同的方法进行挂载.   

~~~bash
cp /mnt/uefi_sys/EFI/Microsoft/Boot/ -aR /media/tdmarco/E49446FD9446D1AA
~~~

C 盘对应的磁盘被挂载到 /media/tdmarco/E49446FD9446D1AA 目录下.    

4. 运行 update-grub 即可:   

~~~bash
tdmarco@tdmarco:~/work/gitwork/dl_ai/darknet$ sudo update-grub
Generating grub configuration file ...
Found linux image: /boot/vmlinuz-4.4.0-130-generic
Found initrd image: /boot/initrd.img-4.4.0-130-generic
Found linux image: /boot/vmlinuz-4.4.0-31-generic
Found initrd image: /boot/initrd.img-4.4.0-31-generic
Found memtest86+ image: /memtest86+.elf
Found memtest86+ image: /memtest86+.bin
Found Windows 10 (loader) on /dev/sdc4
done
~~~

这时候就出现了 windows 10 的启动项.   


~~~grub

insmod part_gpt
insmod fat
insmod search_fs_uuid
insmod chain

chainloader --force /efi/MicroSoft/Boot/bootmgfw.efi
~~~

## 6. 系统层面的后遗症    
6.1 grub rescue (即找不到系统引导).   

错误提示，error: file '/grub/i386-pc/normal.mod' not found   

```
grub rescue>
```

1) 找到包含 ${}/boot/grub 的分区.   

```bash
ls  # 查看当前目录下的所有文件  
(hd0) (hd0,msdos8) (hd0,msdos7) (hd0,msdos6) (hd0,msdos5) (hd0,msdos3) (hd0,msdos2) (hd0,msdos1)
# 依次使用tab 补全的方式查看哪个分区包含有 ${}/boot/grub 目录.   
```

经试验，发现我的引导 grub 在(hd0,msdos6)/boot/grub 下, 所以，只要将 grub 指向这个位置就 ok 了.   
当然，其他人的 grub 位置可能会不同，需要自己用 ls 命令耐心查找。一般人的在 (hd0,msdos5)/grub.   

2) 解决方法.   

```bash
grub rescue> ls  # 查看当前目录下的所有文件  
(hd0) (hd0,msdos8) (hd0,msdos7) (hd0,msdos6) (hd0,msdos5) (hd0,msdos3) (hd0,msdos2) (hd0,msdos1)  
grub rescue> set root=(hd0,msdos6)/boot   # 设置grub的启动分区  
grub rescue> set prefix=(hd0,msdos6)/boot/grub # 设置grub的启动路径  
grub rescue> insmod normal # 加载正常模式  
grub rescue> normal  
```

3) 启动后进入 ubuntu 系统   

现在只是进入了系统，引导还是坏的，所以下一步是在修复 ubuntu 引导 win10.   

进入 terminal 终端输入以下命令:   

```bash
# sudo能使用 某些只有管理员或者管理员授权才能使用的命令  
sudo update-grub  
# grub 安装到指定位置  
sudo grub-install /dev/sda 
``` 
