
git clone太慢
2018年06月07日 15:59:11
阅读数：457
系统是ubuntu16.04，在git clone的时候速度很慢

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
151.101.113.194 github.global.ssl.fastly.Net
151.101.185.194
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


hi clear SyntasticError
hi clear YcmErrorSection
hi clear SyntasticStyleError
hi clear ColorColumn

hi clear 


在最后加入:
    "hi clear SignColumn      " SignColumn should match background
    hi clear SpellBad        " words Spells Bad
    "hi clear LineNr          " Current line number row will have same background color in relative mode
    hi clear CursorLineNr    " Remove hi color from current line number
    hi clear SyntasticError
    hi clear YcmErrorSection
    hi clear SyntasticStyleError0
    hi clear ColorColumn

color monokai

先看看vim编辑器提供的色彩配置方案：
首先进入vim的color目录（/usr/share/vim/vim74/colors,不同的系统目录不同，建议在～/建立.vim目录，然后在些目录里建立对应的文件夹和文件）

$ ls /usr/share/vim/vim62/colors
blue.vim      delek.vim    evening.vim  murphy.vim     README.txt  torte.vim
darkblue.vim  desert.vim   koehler.vim  pablo.vim      ron.vim     zellner.vim
default.vim   elflord.vim  morning.vim  peachpuff.vim  shine.vim

然后再自己的~HOME/建立一个 .vimrc文档 

[$ vi .vimrc
:colorscheme evening 

，假如选择其他的方案 只要替换 evening 就能够了！
然后再打开vim 看一下是不是颜色变得好看了！

 
但我更喜欢desert  
hohoho~~~

colorscheme真的可以我特别不喜欢看代码的时候有那种深蓝色的， 我一般用的是colorscheme desert 。我不知道怎么让vim打开时就是colorscheme desert。 我的解决办法是在/etc/vim/vimrc的最后加上colorscheme desert这行。如果你不喜欢desert主题你可以在一个打开的vim里的命令模式里输入“:colorscheme ” 然后按Tap键，它会自动切换不同主题的名字。