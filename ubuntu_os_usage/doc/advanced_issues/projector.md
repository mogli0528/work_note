# ubuntu 连接投影仪失败或显示不正常

Linux连接投影仪，网上这方便的资料比较少，尤其是图文资料。最近有这方面的需求，查了很多的资料，最终实现的投影。   

直接插上VGA后，发现屏幕显示的不正确, 或不显示。这是由于投影仪的分辨率引起的。    

## 查看电脑的分辨率和投影仪的分辨率    

输入命令:   
```bash
klm@klm:~$ xrandr 

Screen 0: minimum 8 x 8, current 1366 x 768, maximum 16384 x 16384
LVDS-1 connected primary 1366x768+0+0 309mm x 174mm
   1366x768       60.0*+
   1360x768       59.8     60.0  
   1024x768       60.0     60.0  
   ...            ...      ...
   400x300        60.3     56.3  
   320x240        60.1  
VGA-1 connected 1024x768+1366+0 (normal left inverted right x axis y axis) 0mm x 0mm
   1024x768      60.00* 
   800x600       60.32    56.25  
   848x480       60.00  
   ...          ...

HDMI-1 disconnected
DP-1 disconnected
```
下面对输出的结果简单的解读一下:    

Screen 0: 是笔记本分辨率，当前的分辨率是1366 x 768。    
VGA-1: 投影仪接口, connected 说明 VGA 连接成功了, 投影仪最大支持的分辨率为 1024x768.   

## 设置
 
设置的时候注意一下 `VGA-?` 后面接着的数字可能不一样，改成你机子上的对应序号即可.       

### 1. copy 显示   
```bash
xrandr --output VGA-1 --same-as LVDS-1 --mode 1024x768
```
### 2. 修改笔记本分辨率以适应投影仪    
```bash
xrandr -s 1024x768
```
-s 选项表示设置分辨率, 1024x768 是投影仪的分辨率大小.   

注：只有电脑和投影仪分辨率一致时方正常显示.    

### 3. 切换双屏   

3.1 打开外接显示器(最高分辨率)，与笔记本液晶屏幕显示同样内容（克隆）.  
```bash
xrandr --output VGA --same-as LVDS --auto
```
3.2 打开外接显示器(分辨率为1024x768)，与笔记本液晶屏幕显示同样内容（克隆）     
```bash
xrandr --output VGA --same-as LVDS --mode 1024x768
```
3.3 打开外接显示器(最高分辨率)，设置为右侧扩展屏幕   
```bash
xrandr --output VGA --right-of LVDS --auto  
```
3.4 关闭外接显示器
```bash
xrandr --output VGA --off
```
3.5 打开外接显示器，同时关闭笔记本液晶屏幕（只用外接显示器工作）    
```bash
xrandr --output VGA --auto --output LVDS --off
```
3.6 关闭外接显示器，同时打开笔记本液晶屏幕 (只用笔记本液晶屏)   
```bash
xrandr --output VGA --off --output LVDS --auto
```
3.7 设置适应投影仪的分辨率    
```bash
xrandr - 
```
