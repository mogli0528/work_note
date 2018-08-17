# Qt 无法使用中文输入法  

Qt 无法输入中文的问题， 都是在 fcitx 遇到.  

## 编译生成支持中文输入法的 fcitx 库 

1)  设置 qmake 的路径.    

在 ~/.bashrc 文件中添加以下两行.   

```
export PATH="/home/klm/opt/Qt5.10.0/5.10.0/gcc_64/bin":$PATH
export LD_LIBRARY_PATH=/home/klm/opt/Qt5.10.0/5.10.0/gcc_64/lib:$LD_LIBRARY_PATH  
```

2) 下载源码.   

```
git clone https://github.com/fcitx/fcitx-qt5.git
```

3) 编译生成库文件  

```
cd fcitx-qt5
mkdir build &7 cd build 
cmake ..
make -j8
```

不需要 `sudo make install`, 然后把 libfcitxplatforminputcontextplugin.so 库复制到安装目录下的 platforminputcontexts 中:   

```
sudo cp platforminputcontext/libfcitxplatforminputcontextplugin.so /home/klm/opt/Qt5.10.0/5.10.0/gcc_64/plugins/platforminputcontexts
sudo cp platforminputcontext/libfcitxplatforminputcontextplugin.so /home/klm/opt/Qt5.10.0/Tools/QtCreator/lib/Qt/plugins/platforminputcontexts/
```

至此就完成了，重启一下 qtcreator 就可以使用中文输入法了。  

## 编译过程中出现的问题  

1) 缺少 ECMConfig.cmake  

~~~bash
CMake Error at CMakeLists.txt:8 (find_package):
  Could not find a package configuration file provided by "ECM" (requested
  version 1.4.0) with any of the following names:

    ECMConfig.cmake
    ecm-config.cmake

  Add the installation prefix of "ECM" to CMAKE_PREFIX_PATH or set "ECM_DIR"
  to a directory containing one of the above files.  If "ECM" provides a
  separate development package or SDK, be sure it has been installed.


-- Configuring incomplete, errors occurred!
See also "/path/to/fcitx-qt5/CMakeFiles/CMakeOutput.log".
~~~

解决办法如下:  

~~~bash
sudo apt install extra-cmake-modules 
cmake ..
~~~

2)  缺少 XKBCommon_XKBCommon   

~~~bash
-- Could NOT find XKBCommon_XKBCommon (missing: XKBCommon_XKBCommon_LIBRARY XKBCommon_XKBCommon_INCLUDE_DIR) 
CMake Error at /usr/share/cmake-3.9/Modules/FindPackageHandleStandardArgs.cmake:137 (message):
  Could NOT find XKBCommon (missing: XKBCommon_LIBRARIES XKBCommon) (Required
  is at least version "0.5.0")
Call Stack (most recent call first):
  /usr/share/cmake-3.9/Modules/FindPackageHandleStandardArgs.cmake:377 (_FPHSA_FAILURE_MESSAGE)
  cmake/FindXKBCommon.cmake:30 (find_package_handle_standard_args)
  CMakeLists.txt:33 (find_package)

~~~

解决方法如下:   

~~~bash
sudo apt install libxkbcommon-dev

cmake ..
~~~

3) 缺少 FcitxConfig.cmake  

~~~bash
-- Found XKBCommon_XKBCommon: /usr/lib/x86_64-linux-gnu/libxkbcommon.so (found version "0.7.1") 
-- Found XKBCommon: /usr/lib/x86_64-linux-gnu/libxkbcommon.so (found suitable version "0.7.1", minimum required is "0.5.0") found components:  XKBCommon 
CMake Error at CMakeLists.txt:36 (find_package):
  By not providing "FindFcitx.cmake" in CMAKE_MODULE_PATH this project has
  asked CMake to find a package configuration file provided by "Fcitx", but
  CMake did not find one.

  Could not find a package configuration file provided by "Fcitx" (requested
  version 4.2.8) with any of the following names:

    FcitxConfig.cmake
    fcitx-config.cmake

  Add the installation prefix of "Fcitx" to CMAKE_PREFIX_PATH or set
  "Fcitx_DIR" to a directory containing one of the above files.  If "Fcitx"
  provides a separate development package or SDK, be sure it has been
  installed.
~~~

~~~
sudo apt install fcitx-libs-dev 

cmake ..
make -j8
~~~

手别抖不要惯性 sudo make install，不需要。现在 platforminputcontext 目录下应该已经有了新鲜出炉的 libfcitxplatforminputcontextplugin.so 了.   

## RStudio

接下来一样，在 RStudio 菜单的关于里看了下，基于 QT-5.4.0，那就下载qt-everywhere-opensource-src-5.4.0.tar.xz好了。
你以为帖子完了，怎么可能，Naive。

~~~bash
./configure --prefix=/opt/qt.5.4.0 -no-openssl -qt-xcb 直接报错：

ln -s libQt5Widgets.so.5.4.0 libQt5Widgets.so
ln -s libQt5Widgets.so.5.4.0 libQt5Widgets.so.5
ln -s libQt5Widgets.so.5.4.0 libQt5Widgets.so.5.4
rm -f ../../lib/libQt5Widgets.so.5.4.0
mv -f libQt5Widgets.so.5.4.0  ../../lib/ 
rm -f ../../lib/libQt5Widgets.so
rm -f ../../lib/libQt5Widgets.so.5
rm -f ../../lib/libQt5Widgets.so.5.4
mv -f libQt5Widgets.so ../../lib/ 
mv -f libQt5Widgets.so.5 ../../lib/ 
mv -f libQt5Widgets.so.5.4 ../../lib/ 
make[3]: Leaving directory '/home/adam/Downloads/Persepolis/qt-everywhere-opensource-src-5.4.0/qtbase/src/widgets'
make[2]: Leaving directory '/home/adam/Downloads/Persepolis/qt-everywhere-opensource-src-5.4.0/qtbase/src'
Makefile:45: recipe for target 'sub-src-make_first' failed
make[1]: *** [sub-src-make_first] Error 2
make[1]: Leaving directory '/home/adam/Downloads/Persepolis/qt-everywhere-opensource-src-5.4.0/qtbase'
Makefile:70: recipe for target 'module-qtbase-make_first' failed
make: *** [module-qtbase-make_first] Error 2
~~~

一头雾水，连报错信息都基本没有。二话不说，Google，靠谱的办法试试看，比如这个帖子：Build Qt Static Make Error - [SOLVED]， 官方论坛官方回答，看着靠谱。哦：

~~~bash
./configure --prefix=/opt/qt.5.4.0 -release -opensource -confirm-license -static -qt-xcb -no-openssl -no-glib -no-pulseaudio -no-alsa -opengl desktop -nomake examples -nomake tests

make -j4
~~~

# 燃烧吧 CPU。Winter is Coming!!!!!!


rm -f ../../lib/libQt5Widgets.a
mv -f libQt5Widgets.a ../../lib/ 
make[3]: Leaving directory '/home/adam/Downloads/Persepolis/qt-everywhere-opensource-src-5.4.0/qtbase/src/widgets'
make[2]: Leaving directory '/home/adam/Downloads/Persepolis/qt-everywhere-opensource-src-5.4.0/qtbase/src'
Makefile:45: recipe for target 'sub-src-make_first' failed
make[1]: *** [sub-src-make_first] Error 2
make[1]: Leaving directory '/home/adam/Downloads/Persepolis/qt-everywhere-opensource-src-5.4.0/qtbase'
Makefile:70: recipe for target 'module-qtbase-make_first' failed
make: *** [module-qtbase-make_first] Error 2
还是上面那个报错…我也不知道为啥了，好吧老实点先把不知道的从命令拿掉，重新./configure --prefix=/opt/qt.5.4.0 -release -opensource -confirm-license -no-openssl -qt-xcb -nomake examples -nomake tests。
报错依然，上网一顿查，Google 看了 N 多都是交叉编译的问题，而且错误和我不完全一样。百度，各种论坛都是提问题的没有回答的。

N 久无果，中间 2~3个小时过去了。

我开始思索是不是我哪里做法有问题。

这时我突然记起来之前尝试编译 RStudio 的时候，从 GitHub 上安装依赖的脚本里看到编译 RStudio 的时候会依照里面的设置从他们自己的 AWS 服务器上下载他们精（魔）简（改）的 QT binary 的。这洋一想我直接去用他们的 QT 编译岂不是更好。二话不说去 GitHub 看他们的 QT 放在哪儿。你看他们的 rstudio/dependencies/linux/install-qt-sdk里写的：

~~~
# presume 5.4.0
QT_VERSION=5.4.0

# test for libgstreamer
which apt-cache > /dev/null
if [ $? == 0 ]; then
  # debian (currently no test for CentOS based systems)
  apt-cache show libgstreamer1.0 > /dev/null
  if [ $? == 0 ]; then
    QT_VERSION=5.4.2
  fi
fi

QT_SDK_BINARY=QtSDK-${QT_VERSION}-${QT_ARCH}.tar.gz
QT_SDK_URL=https://s3.amazonaws.com/rstudio-buildtools/$QT_SDK_BINARY

# set Qt SDK dir if not already defined
if [ -z "$QT_SDK_DIR" ]; then
  QT_SDK_DIR=~/Qt${QT_VERSION}
fi

if ! test -e $QT_SDK_DIR
then
   # download and install
   wget $QT_SDK_URL -O /tmp/$QT_SDK_BINARY
   cd `dirname $QT_SDK_DIR`
   tar xzf /tmp/$QT_SDK_BINARY
   rm /tmp/$QT_SDK_BINARY
else
   echo "Qt $QT_VERSION SDK already installed"
fi
~~~

暴力暴力，够社会。

直接自己拼接出 QtSDK-5.4.0 的地址下下来了。由于这个已经是 binary 了就不需要我再编译了，直接用就行。
然后就是跟前面差不多了。解压他们的 QT 放到 /opt/qt.5.4.0，然后重新编译 fictx-qt5，得到libfcitxplatforminputcontextplugin.so。

注意，刚刚是 Mendeley 所以最后 libfcitxplatforminputcontextplugin.so 就拷贝到/opt/mendeleydesktop/plugins/qt/plugins/platforminputcontexts/，即谁要给谁。同理，RStudio 就应该拷贝到/usr/lib/rstudio/bin/plugins/platforminputcontexts/ 了。

然后试了下 RStudio 终于，Fcitx 起来了。   