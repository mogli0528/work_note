# meshlab 的安装   
1. 添加软件安装源.   
在终端中输入:   
```
sudo add-apt-repository ppa:zarquon42/meshlab
```
按下回车-回车键，将密钥导入信任度数据库.   
2. 更新软件.   
继续在终端中输入:   
```
sudo apt-get update
```
更新系统信息。    
3. 安装 meshlab.   
继续在终端中输入:   
```
sudo apt-get install meshlab
```
4. 卸载    
通过终端卸载有两个步骤，第一个步骤是卸载软件，第二个步骤是将PPA从信任度系统中移除。   
1) `sudo apt-get purge meshlab`   
2) `sudo apt-get install ppa-purge`    
`sudu ppa-purge ppa:zarquon42/meshlab`   
卸载完成。（这两条指令就是移除PPA的操作）。   
