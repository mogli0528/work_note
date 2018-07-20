# 深度学习服务器开发环境配置   

## 安装操作系统

下一步是安装操作系统。我使用的是 Linux，因为大多数 DL 框架都是针对该系统设计的。使用的台式机是 Ubuntu 16.04 LTS, 用 USB 就可安装全部的东西。   

## 设置 SSH

第一步：端口映射

你需要对所有的路由器进行类似处理，如果是 apple 的路由器可以遵循这个指南：https://portforward.com/apple/，操作如下：  

1. 为你的服务器设置一个静态 IP，防止它在每次关闭时更改。  
2. 用 Apple Airport Utility 登录你的路由器。   
3. 为服务器映射端口，完成这一步需要找到服务器的 MAC 地址，如何在 Ubuntu 找到地址可以查看此链接：http://technologyinfinite.blogspot.com/2016/07/three-simple-ways-to-find-mac-address.html。   

第二步：创建一个动态 IP 地址   

我之前为服务器创建过动态 IP 地址，可以允许我在终端远程连接它。你可以通过该网站（http://canyouseeme.org/）验证它的有效性。  

键入一个如下所示的命令连接到我的服务器：  

~~~bash
ssh [my_username]@[my_ip] -L 8888:[dl-rig_static_ip]:8889
~~~

我的服务器在端口 8888 上运行，jupyter notebooks 运行在 8889 上（-L 选项将指定的本机端口重新定向到不同的主机和端口）。这样我们就可以在本地运行我们的设备，与服务器同时测试，以便训练。如果不想这样，在-L 之前把所有东西输入进去即可。在下一节我会解释如何更改运行 jupyter notebooks 的端口。  

## 安装深度学习/机器学习库

现在需要安装所有的与深度学习/机器学习相关的库。 我会将安装脚本进行分节，这样你就容易理解这些内容。   

该脚本是基于 Jeremy Howard』s 编写的 install-gpu.sh（https://github.com/fastai/courses/blob/master/setup/install-gpu.sh），这里边很多东西不仅仅是因为它们是实用性工具，更是因为这也是我们要在 Fastai 中使用的。  

首先，我们需要保证系统是最新的，并且安装了所有我们需要的基础工具包：   

~~~bash
sudo apt-get update
sudo apt-get --assume-yes upgrade
sudo apt-get --assume-yes install tmux build-essential gcc g++ make binutils unzip
sudo apt-get --assume-yes install software-properties-common
sudo apt-get --assume-yes install git
~~~

下一步是下载和安装所有 CUDA GPU 的驱动包：   

~~~bash
mkdir ~/downloadscd ~/downloads

wget http://developer.download.nvidia.com/compute/cuda/repos/ubuntu1604/x86_64/cuda-repo-ubuntu1604_9.0.176-1_amd64.deb
sudo dpkg -i cuda-repo-ubuntu1604_9.0.176-1_amd64.deb
sudo apt-key adv --fetch-keys http://developer.download.nvidia.com/compute/cuda/repos/ubuntu1604/x86_64/7fa2af80.pub
sudo apt-get update
sudo apt-get -y install cuda
sudo apt-get --assume-yes upgrade
sudo apt-get --assume-yes autoremove
sudo apt-get install cuda-toolkit-9.0 cuda-command-line-tools-9-0
~~~

现在我们开始验证是否正确安装了 CUDA：   

~~~bash
sudo modprobe nvidia
nvcc --version
nvidia-smi
~~~

现在开始，我们将 CUDA（Nvidia Deep Learning api） 添加到路径变量：   

~~~bash
cat >> ~/.bashrc << 'EOF'
export PATH=/usr/local/cuda-9.0/bin${PATH:+:${PATH}}
export LD_LIBRARY_PATH=/usr/local/cuda-9.0/lib64\
${LD_LIBRARY_PATH:+:${LD_LIBRARY_PATH}}
EOF
~~~

然后更新启动脚本.   

~~~bash
source ~/.bashrc
~~~

下一步是安装 CuDNN 库（创建神经网络所需）:   

~~~bash
wget http://files.fast.ai/files/cudnn-9.1-linux-x64-v7.tgz
tar xf cudnn-9.1-linux-x64-v7.tgz
sudo cp cuda/include/*.* /usr/local/cuda/include/
sudo cp cuda/lib64/*.* /usr/local/cuda/lib64/
~~~

现在我们为当前用户安装 Anaconda：   

~~~bash
wget "https://repo.continuum.io/archive/Anaconda3-5.0.1-Linux-x86_64.sh"
bash "Anaconda3-5.0.1-Linux-x86_64.sh" -b

cd ~

echo "export PATH=\"$HOME/anaconda3/bin:\$PATH\"" >> ~/.bashrc
export PATH="$HOME/anaconda3/bin:$PATH"
conda install -y bcolz
conda upgrade -y --all
~~~

下一步，安装 Tensorflow 和 Keras：  

~~~bash
pip install --ignore-installed --upgrade https://storage.googleapis.com/tensorflow/linux/gpu/tensorflow_gpu-1.8.0-cp36-cp36m-linux_x86_64.whl

pip install keras
mkdir ~/.keras
echo '{
    "image_dim_ordering": "tf",
    "epsilon": 1e-07,
    "floatx": "float32",
    "backend": "tensorflow"
}' > ~/.keras/keras.json
~~~

接下来，我们将为 Fastai 安装 Python 依赖项：   

~~~bash
mkdir -p ~/development/_training/ml
cd ~/development/_training/ml

git clone https://github.com/fastai/fastai.git
cd fastai
conda env update
~~~

## 配置 jupyter notebook    

# Leaving the next line uncommented will prompt you to provide a password to
# use with your jupyter notebook.
jupass=`python -c "from notebook.auth import passwd; print(passwd())"`
# To hardcode the password to 'jupyter' comment line above and uncomment the line below.
#jupass=sha1:85ff16c0f1a9:c296112bf7b82121f5ec73ef4c1b9305b9e538af

# create ssl cert for jupyter notebook
openssl req -x509 -nodes -days 365 -newkey rsa:1024 -keyout $HOME/mykey.key -out $HOME/mycert.pem -subj "/C=IE"

# configure notebook
echo "c.NotebookApp.certfile = u'/home/{user}/mycert.pem'" >> $HOME/.jupyter/jupyter_notebook_config.py
echo "c.NotebookApp.keyfile = u'/home/{user}/mykey.key'" >> $HOME/.jupyter/jupyter_notebook_config.py
echo "c.NotebookApp.password = u'"$jupass"'" >> $HOME/.jupyter/jupyter_notebook_config.py
echo "c.NotebookApp.ip = '*'" >> $HOME/.jupyter/jupyter_notebook_config.py
echo "c.NotebookApp.open_browser = False" >> $HOME/.jupyter/jupyter_notebook_config.py

要更改默认端口以在（端口 8888）上运行 Jupyter notebook，请取消注释，并输入所需端口。这样就可以在服务器和本地同时运行你的笔记本，也可以在你使用笔记本时让多个用户使用他们自己的笔记本。

#echo "c.NotebookApp.port = 9999" >> $HOME/.jupyter/jupyter_notebook_config.py

现在我们开始配置 tmux，这个工具可以使我们在终端窗口创建多个「窗口」，同时在断开连接后仍能保持程序的运行。因为这个文件能够帮助我们理解 tmux 的所有功能，所以需要检查此文件。它非常的有用，因为你可以在一个窗口中运行你的笔记本，在另一个窗口中监视 GPU 使用，并在第三个窗口中打开一个 linux 终端：

~~~bash
pip install tmuxp
mkdir ~/.tmuxp

cat > $HOME/.tmuxp/fastai.yml << tmuxp-config 
session_name: fastai
windows:
- window_name: dev window
  layout: main-vertical
  options:
    main-pane-width: 140
  shell_command_before:
    # run as a first command in all panes
    - cd ~/development/_training/ml/fastai
    - source activate fastai
  panes:
    - shell_command:
      - clear
    - shell_command:
      - clear
      - jupyter notebook
    - shell_command:
      - watch -n 0.5 nvidia-smi
Tmuxp-config
~~~

接下来，我们将创建 tmuxp 的配置文件，该文件将在一个命令中对开发环境进行设置，这样我们每次想处理事务的时候就无需配置窗口、启用 jupyter notebook 了。对于 fastai 环境，我们将从 tmuxp 加载 fastai 开始。请参见使用 tmuxp 的链接，此处是 bash 脚本中的文档，此链接用于在服务器重启时保存 tmux 会话。现在让我们配置运行环境。

因为我们不再需要这些，所以可以删除安装文件：

~~~
cd ~/downloads
rm -rf cuda-repo-ubuntu1604_9.0.176-1_amd64.deb xf cudnn-9.1-linux-x64-v7.tgz Anaconda3-5.0.1-Linux-x86_64.sh        
~~~

就是这样。在我写这篇文章的时候，服务器一直在全天候运行，无问题、无噪声、非常轻松地通过了训练。  

## 参考资料来源   

https://www.digitalocean.com/community/tutorials/how-to-set-up-ssh-keys--2  

https://towardsdatascience.com/building-your-own-deep-learning-box-47b918aea1eb   

https://medium.com/impactai/setting-up-a-deep-learning-machine-in-a-lazy-yet-quick-way-be2642318850   