# ubuntu 14.04 安装 ssd 以及出现的问题

>声明： 如果没有出现问题，直接过就好。如果出现问题了，可按照文章末尾提供的问题解决方案来做。    

## 第一部分，准备材料（NVIDIA官网下载）：
`显卡驱动`：NVIDIA-Linux-x86_64-367.44.run,(可以不使用这个，直接使用sudo apt-get install nvidia-367)    
`Cuda8.0`:cuda_8.0.44_linux.run   
下载网址：https://developer.nvidia.com/cuda-downloads   
`Cudnn`：cudnn-8.0-linux-x64-v5.1.tgz   
下载网址：https://developer.nvidia.com/cudnn    
## 第二部分，安装步骤   
2.1 系统安装   
系统选择 ubuntu 14.04，下载后 ultrISO 制作到 U盘 安装，不细说了。关闭系统更新。   
2.2 安装编译工具   
```bash
$ sudo apt-get install build-essential # basic requirement
$ sudo apt-get install cmake git
$ sudo apt-get update    #update source
```
2.3 安装依赖项   
```bash
$ sudo apt-get install libprotobuf-dev libleveldb-dev libsnappy-dev libopencv-dev libhdf5-serial-dev protobuf-compiler 
$ sudo apt-get install --no-install-recommends libboost-all-dev
$ sudo apt-get install libopenblas-dev liblapack-dev libatlas-base-dev
$ sudo apt-get install libgflags-dev libgoogle-glog-dev liblmdb-dev
$ sudo apt-get install python-numpy python-scipy python-matplotlib
```
2.4 安装`cuda8.0`   
进入cuda_8.0.44_linux.run所在目录   
```bash
$ cd /home/smith/Downloads
$ sudo chmod +x cuda_8.0.44_linux.run 
$ sudo ./cuda_8.0.44_linux.run
```
按`q`键退出RELU文档，按照如下选择，显卡驱动一定要选`n`，不装     
```
Do you accept the previously read EULA?   
accept/decline/quit: accept   

Install NVIDIA Accelerated Graphics Driver for Linux-x86_64 361.62?   
(y)es/(n)o/(q)uit: n   
Install the CUDA 8.0 Toolkit?   
(y)es/(n)o/(q)uit: y   
Enter Toolkit Location   
[ default is /usr/local/cuda-8.0 ]:   
Do you want to install a symbolic link at /usr/local/cuda?    
(y)es/(n)o/(q)uit: y    
Install the CUDA 8.0 Samples?    
(y)es/(n)o/(q)uit: y    
Enter CUDA Samples Location    
[ default is /home/zhou ]:    
Installing the CUDA Toolkit in /usr/local/cuda-8.0 …    
```
完成后看到     
```
Driver: Not Selected
Toolkit: Installed in /usr/local/cuda-8.0
Samples: Installed in /home/zhou, but missing recommended libraries
```
最后，配置环境变量，直接放在系统配置文件`profile`里面:    
```
$ sudo gedit /etc/profile   
在最后面加入两行代码:   
export PATH=/usr/local/cuda-8.0/bin:$PATH   
export LD_LIBRARY_PATH=/usr/local/cuda-8.0/lib64:$LD_LIBRARY_PATH   
```
保存退出.     
执行：`$ sudo ldconfig`   
此时，显卡驱动没装，等待下一步显卡驱动装好后检查 cuda8.0 是否装好。   
**`卸载cuda`**：   
```bash
$ cd /usr/local/cuda-8.0/bin
$ sudo ./uninstall_cuda_8.0.pl
```
这里可能会出现链接问题   
```
/usr/lib/nvidia-375/libEGL.so.1 is not a symbolic link
/usr/lib32/nvidia-375/libEGL.so.1 is not a symbolic link
/usr/local/cuda-8.0/targets/x86_64-linux/lib/libcudnn.so.5 is not a symbolic link
```
这里是软链接被拷贝为源文件了，需要重建软链接：   
```
sudo ln -s /usr/lib/nvidia-375/libEGL.so.375.66 /usr/lib/nvidia-375/libEGL.so.1
sudo ln -s /usr/lib32/nvidia-375/libEGL.so.375.66 /usr/lib32/nvidia-375/libEGL.so.1
sudo ln -sf libcudnn.so.5.1.10 libcudnn.so.5
sudo ln -sf libcudnn.so.5 libcudnn.so
```
2.5 显卡驱动安装   
进入显卡驱动目录   
```bash
$ sudo apt-get install nvidia-367
或者(推荐用使用上面的方式,如果有更新的版本,可以使用nvidia-xxx)：
$ cd /home/smith/Downloads
$ sudo su
$ sudo ./NVIDIA-Linux-x86_64-367.44.run
```
一路按照提示选择安装，具体不记得了，主要有接受协议，在系统内核注册，用新路径注册，更新`X-server`，安装完成后会自动回到命令行，重启电脑。   
2.6 检查之前的安装    
此时在home目录下会出现文件夹`NVIDIA_CUDA-8.0_Samples`,打开终端，进入该目录:   
```bash
$ sudo make -j8  #编译samples，我电脑8线程，全开编译
```
等待 2 分钟左右，编译完成，执行下条指令：   
```bash
$ sudo ./1_Utilities/deviceQuery/deviceQuery
```
出现如下信息，cuda8.0安装成功（忘记截图了，下面信息是gtx670装cuda6.5的）   
```bash
./deviceQuery Starting...  

 CUDA Device Query (Runtime API) version (CUDART static linking)

Detected 1 CUDA Capable device(s)

Device 0: "GeForce GTX 1080 Ti"
  CUDA Driver Version / Runtime Version          8.0 / 8.0
  CUDA Capability Major/Minor version number:    6.1
  Total amount of global memory:                 11164 MBytes (11706630144 bytes)
  (28) Multiprocessors, (128) CUDA Cores/MP:     3584 CUDA Cores
  GPU Max Clock rate:                            1582 MHz (1.58 GHz)
  Memory Clock rate:                             5505 Mhz
  Memory Bus Width:                              352-bit
  L2 Cache Size:                                 2883584 bytes
  Maximum Texture Dimension Size (x,y,z)         1D=(131072), 2D=(131072, 65536), 3D=(16384, 16384, 16384)
  Maximum Layered 1D Texture Size, (num) layers  1D=(32768), 2048 layers
  Maximum Layered 2D Texture Size, (num) layers  2D=(32768, 32768), 2048 layers
  Total amount of constant memory:               65536 bytes
  Total amount of shared memory per block:       49152 bytes
  Total number of registers available per block: 65536
  Warp size:                                     32
  Maximum number of threads per multiprocessor:  2048
  Maximum number of threads per block:           1024
  Max dimension size of a thread block (x,y,z): (1024, 1024, 64)
  Max dimension size of a grid size    (x,y,z): (2147483647, 65535, 65535)
  Maximum memory pitch:                          2147483647 bytes
  Texture alignment:                             512 bytes
  Concurrent copy and kernel execution:          Yes with 2 copy engine(s)
  Run time limit on kernels:                     Yes
  Integrated GPU sharing Host Memory:            No
  Support host page-locked memory mapping:       Yes
  Alignment requirement for Surfaces:            Yes
  Device has ECC support:                        Disabled
  Device supports Unified Addressing (UVA):      Yes
  Device PCI Domain ID / Bus ID / location ID:   0 / 1 / 0
  Compute Mode:
     < Default (multiple host threads can use ::cudaSetDevice() with device simultaneously) >

deviceQuery, CUDA Driver = CUDART, CUDA Driver Version = 8.0, CUDA Runtime Version = 8.0, NumDevs = 1, Device0 = GeForce GTX 1080 Ti
Result = PASS
``` 
可以看到，最后出现了PASS，安装`cuda`完成。   
还可以查看nvcc版本   
```bash
$ nvcc -V i
nvcc: NVIDIA (R) Cuda compiler driver
Copyright (c) 2005-2016 NVIDIA Corporation
Built on Tue_Jan_10_13:22:03_CST_2017
Cuda compilation tools, release 8.0, V8.0.61
```
显示显卡信息:   
```bash
$ nvidia–smi  

Wed Sep  6 15:02:50 2017       
+-----------------------------------------------------------------------------+
| NVIDIA-SMI 375.66                 Driver Version: 375.66                    |
|-------------------------------+----------------------+----------------------+
| GPU  Name        Persistence-M| Bus-Id        Disp.A | Volatile Uncorr. ECC |
| Fan  Temp  Perf  Pwr:Usage/Cap|         Memory-Usage | GPU-Util  Compute M. |
|===============================+======================+======================|
|   0  GeForce GTX 108...  Off  | 0000:01:00.0      On |                  N/A |
| 61%   74C    P2   207W / 250W |  10064MiB / 11164MiB |     48%      Default |
+-------------------------------+----------------------+----------------------+
                                                                               
+-----------------------------------------------------------------------------+
| Processes:                                                       GPU Memory |
|  GPU       PID  Type  Process name                               Usage      |
|=============================================================================|
|    0      1251    G   /usr/lib/xorg/Xorg                             218MiB |
+-----------------------------------------------------------------------------+
```
2.7 Atlas安装   
ATLAS是做线性代数运算的，还有俩可以选：一个是Intel 的 `MKL`，这个要收费，还有一个是`OpenBLAS`，这个比较麻烦；运行效率为 `ATLAS < OpenBLAS < MKL`.     
```bash
$ sudo apt-get install libatlas-base-dev
```
实际上这步在之前安装依赖项时已经安装过了, 这里确保安装完成即可。   
2.8 `cuDNN`安装   
```bash
$ tar -zxvf cudnn-8.0-linux-x64-v5.1.tgz  
$ cd cuda 
$ sudo cp lib64/lib* /usr/local/cuda/lib64/  
$ sudo cp include/cudnn.h /usr/local/cuda/include/
```
更新软连接：   
```bash
$ cd /usr/local/cuda/lib64/
$ sudo chmod +r libcudnn.so.5.1.5 
$ sudo ln -sf libcudnn.so.5 libcudnn.so
$ sudo ln -sf libcudnn.so.5.1.5 libcudnn.so.5
```
配置运行环境使其他程序可以调用`CUDA`库，在`/etc/ld.so.conf.d目录`新建`caffe.conf`         
```bash
$ sudo gedit /etc/ld.so.conf.d/caffe.conf
```
添加：`/usr/local/cuda/lib64`
保存退出，执行:
```bash
$ sudo ldconfig
```
2.9 安装 python 的 pip 和 easy_install，方便安装软件包(超慢的下载...)   
```
$ sudo wget --no-check-certificate https://bootstrap.pypa.io/ez_setup.py 
$ sudo python ez_setup.py --insecure
$ wget https://bootstrap.pypa.io/get-pip.py
$ sudo python get-pip.py
```
2.10 安装 python 依赖(路径根据自己的目录可能要调一下)    
```bash
$ cd caffe/python
$ sudo su
$ for req in $(cat requirements.txt); do pip install $req; done
```
这步安装也有点慢，别急，等会儿，先去干点别的...    
安装完成之后退出 root 用户。    
2.11 下载/编译 ssd 代码    
Get the code. We will call the directory that you cloned Caffe into $CAFFE_ROOT.    
```bash
git clone https://github.com/weiliu89/caffe.git
cd caffe
git checkout ssd
```
2.12 回到 caffe 目录,修改配置文件.   
```
cd ~/caffe
cp Makefile.config.example Makefile.config

# 修改配置文件
gedit Makefile.config
# 这里仅需修改4处：
# 1) 使用cuDNN:这里去掉#，取消注释为
USE_CUDNN := 1 
# 2) 修改python包目录，这句话
PYTHON_INCLUDE := /usr/include/python2.7 \
　　/usr/lib/python2.7/dist-packages/numpy/core/include
# 改为
PYTHON_INCLUDE := /usr/include/python2.7 \
　　/usr/local/lib/python2.7/dist-packages/numpy/core/include
3) 开启GPU # CPU_ONLY :=1;
4) USE_LMDB := 1；
```
因为新安装的`python`包目录在这里： `/usr/local/lib/python2.7/dist-packages/`。  
**Note:**    
如果你的系统是 ubuntu 16.04, 在接下来的`make py`的时候，会遇到以下这个错误:   
```
/usr/bin/ld: cannot find -lhdf5_hl
/usr/bin/ld: cannot find -lhdf5
collect2: error: ld returned 1 exit status
```
按照以下两个步骤操作即可:    
(1) 在 `Makefile.config` 文件的第 85 行，添加 /usr/include/hdf5/serial/ 到 INCLUDE_DIRS，也就是把下面第一行代码改为第二行代码。   
```
INCLUDE_DIRS := $(PYTHON_INCLUDE) /usr/local/include
INCLUDE_DIRS := $(PYTHON_INCLUDE) /usr/local/include /usr/include/hdf5/serial/
```
(2) 在 `Makefile` 文件的第 173 行，把 hdf5_hl 和 hdf5 修改为 hdf5_serial_hl 和 hdf5_serial，也就是把下面第一行代码改为第二行代码。   
```
LIBRARIES += glog gflags protobuf boost_system boost_filesystem m hdf5_hl hdf5
LIBRARIES += glog gflags protobuf boost_system boost_filesystem m hdf5_serial_hl hdf5_serial
```
2.13 切换到 $CAFFE_ROOT 目录下, 编译 ssd.      
```
sudo make -j8
# 确认 PYTHONPATH 中添加了 $CAFFE_ROOT/python 路径.
sudo make py
sudo make test -j8
# (Optional)
sudo make runtest -j8
```

## 第三部分:问题汇总    
1. 运行 make 之后出现如下错误    
```
/usr/include/boost/property_tree/detail/json_parser_read.hpp:257:264: error: ‘type name’ declared as function returning an array 
escape 
^ 
/usr/include/boost/property_tree/detail/json_parser_read.hpp:257:264: error: ‘type name’ declared as function returning an array 
make: * [.build_release/cuda/src/caffe/layers/detection_output_layer.o] Error 1 
make: * Waiting for unfinished jobs….
```
办法：   
修改`json_parser_read.hpp`：打开文件夹`Document`，选中`computer`，在搜索 json_parser_read.hpp，找到该文件的路径之后用如下命令打开   
```
sudo gedit /usr/include/boost/property_tree/detail/json_parser_read.hpp
```
将257行开始的`escape`代码段注释掉即可，如下：   
```
/*escape
=   chset_p(detail::widen<Ch>("\"\\/bfnrt").c_str())
      [typename Context::a_escape(self.c)]
|   'u' >> uint_parser<unsigned long, 16, 4, 4>()
      [typename Context::a_unicode(self.c)]
;*/

```
2. 在`make py`的时候，遇到了这个错误(ubuntu 16.04 中的问题，在ubuntu14.04中应该不会遇到):   
```
/usr/bin/ld: cannot find -lhdf5_hl
/usr/bin/ld: cannot find -lhdf5
collect2: error: ld returned 1 exit status
```
Step 1   
在Makefile.config文件的第85行，添加/usr/include/hdf5/serial/ 到 INCLUDE_DIRS，也就是把下面第一行代码改为第二行代码。   
```
INCLUDE_DIRS := $(PYTHON_INCLUDE) /usr/local/include
INCLUDE_DIRS := $(PYTHON_INCLUDE) /usr/local/include /usr/include/hdf5/serial/
```
Step 2   
在Makefile文件的第173行，把 hdf5_hl 和hdf5修改为hdf5_serial_hl 和 hdf5_serial，也就是把下面第一行代码改为第二行代码。   
```
LIBRARIES += glog gflags protobuf boost_system boost_filesystem m hdf5_hl hdf5
LIBRARIES += glog gflags protobuf boost_system boost_filesystem m hdf5_serial_hl hdf5_serial
```
3. 训练过程中遇到:`Check failed: error == cudaSuccess (10 vs. 0)  invalid device ordinal`   
 是因为GPU个数的原因，枚举设备时候出错。在`ssd_pascal_xxx.py`330行附近，将`gpus = "0,1,2,3,4"`改为`gpus = "0"`;   
```
# Solver parameters.
# Defining which GPUs to use.
gpus = "0"
gpulist = gpus.split(",")
num_gpus = len(gpulist)
```
4. 缺少库文件    
编译`cuda-examples`时，   
`/usr/bin/ld: cannot find -lglut`   
```bash
sudo apt-get install freeglut3 freeglut3-dev
```
5. 运行`python`脚本的时候，import caffe出错   
添加:
```bash
import sys
sys.path.append(“/home/smith/caffe/python”)

or:
$ export PYTHONPATH=$PYTHONPATH:/home/smith/caffe/python
```
6. `make runtest -j4` 编译时 `GT740M` 的 `CUDA Capability` 是 2.1 ,而官方的 cudnn 加速是不支持 3.0 以下的版本的,因此会出现:   
```
Check failed: status == CUDNN_STATUS_SUCCESS (6 vs. 0)   
caffe make runtest error（core dumped）Check failed: status == CUDNN_STATUS_SUCCESS (6 vs. 0)   
```
简单讲就是GPU的加速性能不够，CUDNN 只支持 CUDA Capability 3.0 以上的 GPU 加速.make 和 make test 都过了，而 make runtest 时报错，大概是这样滴错误:   
```
<pre name="code" class="plain">[----------] 6 tests from CuDNNConvolutionLayerTest/1, where TypeParam = double
[ RUN      ] CuDNNConvolutionLayerTest/1.TestSimpleConvolutionGroupCuDNN
F1014 08:55:30.083176 23568 cudnn_conv_layer.cpp:30] Check failed: status == CUDNN_STATUS_SUCCESS (6 vs. 0)  CUDNN_STATUS_ARCH_MISMATCH
*** Check failure stack trace: ***
    @     0x2b082d0a8daa  (unknown)
     ...
    @              (nil)  (unknown)
make: *** [runtest] Aborted (core dumped)
```
因此只能在 Makefile.config 中注释掉 USE_CUDNN 这行，重新执行以下   
```
make clean
make all -j4
make test -j4
make runtest -j4
```
最后除了 make runtest 中 2 DISABLED TESTS 之外，没有其他问题。make runtest 中出现几个测试例子不过不影响使用.   
7. libcudart.so.8.0: cannot open shared object file: No such file or directory   
问题描述：   
error while loading shared libraries: libcudart.so.8.0: cannot open shared object file: No such file or directory   
解决办法：   
首先确认`/etc/profile`中的路径包含了`cuda8.0`的安装路径及相应的库文件   
```bash
export PATH=$PATH:/usr/local/cuda-8.0/bin
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/cuda-8.0/lib64
export LIBRARY_PATH=$LIBRARY_PATH:/usr/local/cuda-8.0/lib64
```
执行`$source /etc/profile`使配置文件生效。   
若仍提示相同的错误，则执行以下命令，将相应的库文件复制到`/usr/lib`   
```bash
sudo cp /usr/local/cuda-8.0/lib64/libcudart.so.8.0 /usr/local/lib/libcudart.so.8.0 && sudo ldconfig
sudo cp /usr/local/cuda-8.0/lib64/libcublas.so.8.0 /usr/local/lib/libcublas.so.8.0 && sudo ldconfig
sudo cp /usr/local/cuda-8.0/lib64/libcurand.so.8.0 /usr/local/lib/libcurand.so.8.0 && sudo ldconfig
```
ps. ldconfig 命令是一个动态链接库管理命令，是为了让动态链接库为系统共享.   

