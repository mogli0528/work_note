# 如何与深度学习服务器优雅的交互？

小夕有一台自己负责的GPU服务器，她可让小夕操碎了心呐。最近好不容易把这娇气的小公举拉扯大了，下面就向各位服务器宝宝的爸爸妈妈们传授一下育女经验，让她早日成长为一个省心的深度学习服务器。

下面小夕将依次介绍：
- ssh一键免密登录
- 内网穿透（跨网段访问服务器）
- 文件传输与实时同步
- 多开发环境管理
- 多任务管理（并行调参）
- 睡觉调参模式（串行调参）
- 关于Jupyer Notebook
- 单任务霸占GPU模式

## 一键免密远程登录
远程登录最最方便的当然就是ssh啦。看小夕的一键登录！    
第一行黄色的是小夕的用户名、电脑名、当前目录。小夕设置的命令就是 sshdlnlp，敲上这个命令直接进入服务器！    
实现这个非常简单，分两步：   
首先，将你的登录命令写入你的pc端的bash启动脚本中。Mac系统为 ~/.bash_profile，linux系统为~/.bashrc。例如你的服务器用户名为dlnlp，ip为102.10.60.23，那么就把这句登录命令写进去：   
```bash
alias sshdlnlp="ssh dlnlp@102.10.60.23"
```
当然，登录命令叫 sshdlnlp，你也可以改成别的。保存后别忘:     
```bash
source ~/.bash_profile
# 或者
source ~/.bashrc
```
然后，经过第一步后，只需要再敲密码就可以进入啦。但是懒癌至深的我们怎么能容忍敲密码这么麻烦的事情呢！（划掉，应该是小仙女怎么能容忍敲密码这种事情呢）但是我们又不能牺牲服务器的安全性，那怎么办呢？考验大学里计算机网络基础的时候到了～   
也很简单，把你 PC 端的 ssh 公钥写入服务器的 ssh 信任列表里就可以啦。首先用`ssh-keygen`命令生成 rsa 密钥对（生成一只私钥和一只公钥），一路 enter 即可，但是注意：    
之前有已经生成过的同学在此处就选择n吧，没有生成过的同学就一路next～   
然后去`~/.ssh/`文件夹下将公钥发送到服务器上的某文件夹里.然后去服务器上，把你 PC 端的公钥丢进 ssh 信任列表：
```bash
cat id_rsa.pub >> ~/.ssh/authorized_keys
```
好啦～搞定啦，再回到你的 PC 端登录试试吧，是不是连输入密码都省掉啦。    

## 内网穿透（跨网段访问服务器）
但是注意哦，如果你的服务器是在局域网内，那你的PC离开这个局域网的时候当然就找不到你的服务器啦。想要在家里用GPU服务器？很简单，小夕教你分分钟内网穿透！

在内网穿透方面，小夕试了好几种方案后，感觉还是花生壳对新手最友好也最稳定。我们的内网穿透只需要将服务器内网ip以及22端口号（即ssh端口号）映射到外网ip的某个端口号。这个过程使用花生壳非常简单，在网上有很多教程，小夕就不啰嗦啦。之后我们要做的就是将这个外网ip和端口号也封装成一条命令，比如花生壳分配给我们的外网ip是103.44.145.240，端口是12560，那么只需要把这个写入客户端shell启动脚本：

alias sshdlnlp_remote="ssh -p 12560 dlnlp@103.44.145.240"
（别忘用source刷新启动脚本）

之后就可以在世界各地用一条命令访问你的gpu服务器啦。哦对了，网址https://hsk.oray.com/download/

## 文件传输与同步
对于一次性的文件传输，这方面最简单的当然还是直接使用scp命令啦，文件夹和文件都能轻松传输。

但是我们做深度学习的话，在服务器端大面积改代码、重量级调试的话还是不方便，毕竟服务器上没有图形界面，大部分人还是用不惯vim的，那么能不能在PC端用漂亮的编辑器修改代码，将修改结果实时的同步到服务器端呢？当然可以！这里小夕推荐文件同步神器syncthing。

官网丢过来https://syncthing.net/ ，剩下的就是傻瓜式配置啦。记得要更改文件夹刷新频率哦（默认是60秒，我们可以改的短一点，比如3秒），这样在客户端我们用漂亮的文本编辑器对代码的改动就能实时的同步到服务器上啦，在服务器端就只需要负责运行就可以咯。

## 多开发环境管理
如果不幸你的GPU服务器并不是你一个人用，那么这时多人（尤其是混入小白多话）经常把服务器默认的python环境弄的乌烟瘴气，比如有人用python2，有人用python3，有人用tensorflow1.3，有人用0.12等...最后导致大家的程序全跑崩了。

所以在服务器端管理深度学习的开发环境是极其必要的，这里anaconda直接搞定！每个人建立和管理自己的开发环境，包括python版本、各种库的版本等，互不干扰。而且在发布project时，也方便直接将环境导出为requirements文件，免得自己去手写啦。官网丢过来https://anaconda.org/

## 多任务管理（并行调参）
如果你的服务器上有多个GPU，或者你的任务消耗GPU资源不多，那么并行的训练模型调参数是极大提高开发效率的！这里小夕给出几种场景下的常用方案：

1、比如我们在服务器上除了训练还要接着干别的事情（比如还要捣鼓一下贪吃蛇什么的），或者仅仅不希望ssh断开后导致训练任务终止，那么我们就可以直接将训练任务挂后台。具体如下。

在linux中，在命令后面加上&符号可以将命令在后台执行，为了能看到训练日志，我们当时还需要输出重定向（否则会打印到屏幕上干扰正常工作的），所以比如我们调batchsize参数时可以这样：

python train.py --batchsize=16 > log_batch16.txt &

当然再挂上其他batchsize大小，如：

python train.py --batchsize=16 > log_batch16.txt &
python train.py --batchsize=64 > log_batch64.txt &
python train.py --batchsize=128 > log_batch128.txt &

通过jobs命令可以看到后台任务的运行状况（running、stopped等），通过bg [任务号]可以让后台stopped的命令继续running，通过fg [任务号]可以让后台的任务来前台执行。对于前台已经执行起来的任务，可以ctrl+z来丢进后台（丢后台时stop了的话用bg让其run起来）。

2、如果我们特别着急，不仅要并行挂着很多训练任务，而且都要实时的监控它们的训练进展，那么使用screen命令吧，这个命令就相当于可以让你同时开很多个窗口（就像桌面上那样，你可以开很多应用程序的很多窗口），而且多个窗口之间可以轻松切换，同样这种方法不会因为ssh的断开而停止训练任务。

具体的操作可以直接在linux下man screen来查看screen命令的帮助文档。英文恐惧症的童鞋可以看本文参考文献[1]。

## 睡觉调参模式（串行调参）
大部分场合下我们没有那么多充裕的GPU可以用，我们一般只能一次挂一个任务，但是我们又有很重的调参任务，那怎么办呢？

依然很简单啦，首先，装好python-fire这个工具，github链接：

https://github.com/google/python-fire

它可以非常轻松的将你的python程序变成命令行程序，并且可以轻松的将你要调的参数封装成命令行参数的形式。

然后，写一个调参shell脚本，把你要调的参数全都写进去！比如就像这样：


（当然别忘在代码里将训练的summary写到某个文件里）

然后就可以挂上这个脚本去睡觉啦～睡到天亮发现各个最优参数都找到了，超级开心有木有。

## 关于jupyter notebook
这个神器小夕在历史文章中写过啦，也是一个重量级调参神器！或者直接可以说深度学习神器！在服务器端依然犀利的无可替代，只需要如下的tricks。

1、服务器端开启 jupyter notebook 后

然后复制最后那一行的token=xxx，这个token就是远程访问的密码！同时记下最后那行显示的端口号8888（因为如果服务器上同时开多个的话，端口号就不一定是8888了哦），然后去PC端做一个端口映射！即通过ssh隧道来将服务器端的8888端口号映射到本地（PC端）的某个端口（如1234）：
```python
ssh -L 1234:localhost:8888 dlnlp@102.10.60.23
# 以下是我的机器上的指令
ssh -L 1234:localhost:8888 lsn@192.168.16.111 -f -N
```
这时就可以在PC端的浏览器   
```
http://localhost:1234
```
直接访问服务器上的jupyter notebook啦～当然，访问时会让你输入密码，这时就输入之前记下的那个token哦。

2、让jupyer notebook跟anaconda开发环境融合。

默认的情况下jupyter notebook是运行在系统默认环境里的，如果要让它运行在我们自己用ananconda创建的环境中，要进入那个环境中，然后安装nb_conda这个库：

conda install nb_conda

这时再开启jupyter notebook就能选择在我们这个环境里运行代码啦。

## 单任务全霸占模式
有时我们的训练任务非常重要且急迫，且绝对不允许被别人挤崩，或者我们明知要占用全部GPU资源了，那么这时我们就可以。。。emmm事先说明，非必要时刻请勿频繁使用哦：

使用linux中的run-one命令，这个命令可以保证同一条命令最多同时运行一个。比如 run-one python xxx 就会只允许运行一个python程序，后来的python程序在这个python程序执行完毕前是得不到执行的（一执行就会出错返回）。所以我们可以写入.bashrc：

alias python='run-one python'
（别忘source激活）

这时：

看，我通过将第一个python挂到后台了，后面的python完全执行不起来。除非前一个python结束。（所以其他小伙伴可能以为自己的程序出问题了，然后emmm陷入了无尽的困惑）

(天啊，微信改版的不会用了，图片总是被压缩的特别模糊特别小怎么破QAQ疯了疯了，能手敲的全改成手敲了，明天这篇文章去知乎上排版重发一遍，晚安啦）


