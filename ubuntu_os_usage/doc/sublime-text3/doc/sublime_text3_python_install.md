# ubuntu sublime text3 python 配置   

## 1. 介绍    
利用 `sublimeCodeIntel` 插件可以实现自动提示 python 代码、跳转追踪自定义函数、查看系统函数等.功能还是相当强大的.   
Anaconda 插件有以下功能:   
- Goto Definitions 能够在你的整个工程中查找并且显示任意一个变量，函数，或者类的定义。   
- Find Usage 能够快速的查找某个变量，函数或者类在某个特定文件中的什么地方被使用了。   
- Show Documentation： 能够显示一个函数或者类的说明性字符串(当然，是在定义了字符串的情况下).   

## 2.安装 Anaconda 和 sublimeCodeIntel 插件   
现在你可以通过快捷键 `Ctrl+Shift+P` 打开 `Package Control` 来安装其他的插件了。输入 Install 然后你就能看见屏幕上出现了 Package Control: Install Package，点击回车然后搜索你想要的插件(比如 Anaconda 和 sublimeCodeIntel). 安装完成后会显示 messages 文档。    

## 3. 配置
选择 Preferences-Package Settings-Anacoda-Settings-Users 选项，键入以下 json 数据。保存，重启 sublime-text3 即可。    
```
{
  "python_interpreter": "/usr/bin/python2.7",
  "suppress_word_completions": true,
  "suppress_explicit_completions": true,
  "complete_parameters": true,
  "anaconda_linting": false, 
}
```
**Note:**   
(1) Code linting 使用支持 pep8 标准的 PyLint 或者 PyFlakes。所以需要在 Anaconda 的配置文件 Anaconda.sublime-settings 中将 linting 完全禁用。   
"anaconda_linting": false    
(2) 由于 Anaconda 插件本身无法知道 Python 安装的路径，所以需要设置 Python 主程序的实际位置。   
"python_interpreter": "/usr/bin/python2.7"
注意：python 所在路径的查找方式是在 terminal 中输入:   
```bash
whereis python
```
