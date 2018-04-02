# 关于Ubuntu同时安装opencv2和opencv3
我用的是ubuntu14.04,之前已经安装了OpenCV2（默认的安装路径是/usr/local）,现在需要安装OpenCV3.2，但是又不想将opencv2删除，怎么办？？？   
对，在别的地方安装。   
1. 首先，下载好opencv3.2(download: https://github.com/opencv/opencv/archive/3.2.0.zip) 后，将源码解压    
2. 修改 CMakeLists.txt 文件, 大约在 33 行的位置.    
```
if(WIN32)
    ...
else()
    set(CMAKE_INSTALL_PREFIX "/usr/local" CACHE PATH "Installation Directory")
endif()
```    
将默认的安装路径：`/usr/local` 修改为你要安装的路径 `/path/to/opencv3`   
```
set(CMAKE_INSTALL_PREFIX "/path/to/opencv3" CACHE PATH "Installation Directory") 
```　　　　　　　　　 
3. 接着编译，很简单.   
```
sudo apt-get install liblapacke-dev

cmake -DCMAKE_BUILD_TYPE=RELEASE -DWITH_LAPACK=OFF -DCMAKE_INSTALL_PREFIX=/usr/local/opencv3  ..
make
make install
```
你就会在指定安装的路径下找到对应的头文件和库（时间很长，耐心等待就好了）。    
4. 最后就是如何在工程中加入 opencv3.2 的库和头文件。这里需要改变一下，否则很容易链接到位于 /usr/local 下的 opencv2 。在工程文件中的 CMakeLists.txt 加上：    
```
set(OpenCV_DIR "/path/to/opencv3") 
```
来重新制定 opencv 的安装路径， 因为安装 opencv2 时已经生成了系统变量 `OpenCV_DIR = /usr/local/share/opencv`, 所以需要用 set 来改变这个变量的值，新的路径为与文件 OpencvConfig.cmake 路径相同。   
注意：这个 OpencvConfig.cmake 文件里面定义了一堆变量，可以在这里面查阅使用。如 OpenCV_INCLUDE_DIRS 、OpenCV_LIBS 等.   


## 出现的问题  
1. lapack 包的问题.  
```
../../lib/libopencv_core.so.3.2.0: undefined reference to `dgeqrf_'
../../lib/libopencv_core.so.3.2.0: undefined reference to `sposv_'
../../lib/libopencv_core.so.3.2.0: undefined reference to `sgesdd_'
../../lib/libopencv_core.so.3.2.0: undefined reference to `sgeqrf_'
../../lib/libopencv_core.so.3.2.0: undefined reference to `dgesv_'
../../lib/libopencv_core.so.3.2.0: undefined reference to `sgels_'
../../lib/libopencv_core.so.3.2.0: undefined reference to `sgesv_'
../../lib/libopencv_core.so.3.2.0: undefined reference to `dgetrf_'
../../lib/libopencv_core.so.3.2.0: undefined reference to `sgetrf_'
../../lib/libopencv_core.so.3.2.0: undefined reference to `spotrf_'
../../lib/libopencv_core.so.3.2.0: undefined reference to `dgels_'
../../lib/libopencv_core.so.3.2.0: undefined reference to `dgesdd_'
../../lib/libopencv_core.so.3.2.0: undefined reference to `dposv_'
../../lib/libopencv_core.so.3.2.0: undefined reference to `dpotrf_'
```
使用 -DWITH_LAPACK=OFF 编译选项.   