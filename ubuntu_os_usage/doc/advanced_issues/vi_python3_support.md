
# vim添加 python3 支持 

## 首先删除旧的的vim   

./configure --enable-perlinterp --enable-python3interp --enable-rubyinterp --enable-cscope --enable-gui=auto --enable-gtk2-check --with-features=huge --enable-multibyte --with-x --with-compiledby="asd" --with-python3 -config-dir=/usr/lib/python3.4/config-3.4m-x86_64-linux-gnu --prefix=/opt/vim74 


--with-features=huge：支持最大特性

--enable-rubyinterp：启用Vim对ruby编写的插件的支持

--enable-pythoninterp：启用Vim对python编写的插件的支持

--enable-luainterp：启用Vim对lua编写的插件的支持

--enable-perlinterp：启用Vim对perl编写的插件的支持

--enable-multibyte：多字节支持 可以在Vim中输入中文

--enable-cscope：Vim对cscope支持

--enable-gui=gtk2：gtk2支持,也可以使用gnome，表示生成gvim

--with-python-config-dir=/usr/lib/python3.4/config-3.4m-x86_64-linux-gnu 指定 python 路径

--prefix=/usr：编译安装路径


## python2 和 python3 依稀运行会不稳定   

vim can't reliably/sanely run with python2 and python3 both linked in (despite the build system letting you try), so we chose python3. I'd encourage you to upgrade your addons to py3 and give that a whirl.

cd vim/src
./configure --with-features=huge \
            --enable-perlinterp  \
            --enable-rubyinterp \ 
            --enable-multibyte \
            --enable-python3interp  \
            --with-python3-config-dir=/usr/lib/python3.4/config-3.4m-x86_64-linux-gnu \
            --enable-perlinterp=yes \
            --enable-luainterp=yes \
            --enable-gui=gtk2 --enable-cscope \ 
            --prefix=/usr/local/vim8  \


./configure --with-features=huge --enable-perlinterp  --enable-rubyinterp  --enable-multibyte --enable-python3interp  --with-python3-config-dir=/usr/lib/python3.4/config-3.4m-x86_64-linux-gnu --enable-perlinterp=yes --enable-luainterp=yes --enable-gui=gtk2 --enable-cscope  --prefix=/usr/local/vim8  

git clone https://github.com/vim/vim.git

sudo make
sudo make install

添加完成。但是还没有进入到环境变量中。可以自行添加。也可以设置：

sudo update-alternatives --install /usr/bin/editor editor /usr/local/vim8/bin/vim 1
sudo update-alternatives --set editor /usr/local/vim8/bin/vim
sudo update-alternatives --install /usr/bin/vim vim /usr/local/vim8/bin/vim 1
sudo update-alternatives --set vim /usr/local/vim8/bin/vim