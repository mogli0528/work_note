# Chrome 浏览器无法调用 GPU 进程    

## Error    
运行 `jupyter-notebook` 命令后, 终端显示:    
```bash
browser_gpu_channel_host_factory.cc(108)] Failed to launch GPU process.    
Created new window in existing browser session.     
```

## Solution    
在 .bashrc 中将默认浏览器设为相应浏览器.    
```bash
sudo gedit ~/.bashrc  
```
然后在 .bashrc 最后一行添加：    
```bash
export BROWSER=google-chrome  
```
最后更新一下终端配置文件:    
```bash
source ~/.bashrc
```
Done!!! 问题得到解决.    