# Qt 无法使用中文输入法  
我安装的Qt版本很多，但是都会遇到无法输入中文的问题，但是都是在fcitx遇到，使用ibus是可以输入中文的，在这里我是使用的fcitx,搜狗输入法，具体是什么原因我们就不讨论了，来看看解决办法。

首先安装 fcitx-frontend-qt5   
```
sudo apt-get install fcitx-frontend-qt5
```
这个好像都是默认安装了的，然后查看 fcitx-frontend-qt5 的安装目录。   
```
dpkg -L fcitx-frontend-qt5
/usr/lib/x86_64-linux-gnu/qt5/plugins/platforminputcontexts/libfcitxplatforminputcontextplugin.so
```
然后把这个库复制到安装目录下的 platforminputcontexts 中:   
```
sudo cp /usr/lib/x86_64-linux-gnu/qt5/plugins/platforminputcontexts/libfcitxplatforminputcontextplugin.so  /home/klm/opt/Qt5.10.0/5.10.0/gcc_64/plugins/platforminputcontexts
sudo cp /usr/lib/x86_64-linux-gnu/qt5/plugins/platforminputcontexts/libfcitxplatforminputcontextplugin.so  /home/klm/opt/Qt5.10.0/Tools/QtCreator/lib/Qt/plugins/platforminputcontexts/
```
至此就完成了，重启一下 qtcreator 就可以使用中文输入法了。   

`---------------------------------------------------------`
上面的操作亲测不可用   
`---------------------------------------------------------`


1.   
2. 设置 qmake 的路径.    
在 ~/.bashrc 文件中添加以下两行.   
```
export PATH="/home/klm/opt/Qt5.10.0/5.10.0/gcc_64/bin":$PATH
export LD_LIBRARY_PATH=/home/klm/opt/Qt5.10.0/5.10.0/gcc_64/lib:$LD_LIBRARY_PATH  
```
3. 下载源码.   
```
git clone git@github.com:fcitx/fcitx-qt5.git
```
4. 
```
cd fcitx-qt5
cmake .
make
sudo make install
```