# sublime-text3 Nodejs 开发环境配置   
## 1. 前言    
大家都知道，Sublime Text 安装插件一般从 Package Control 中直接安装即可，当我安装 node js 插件时候，直接通过 Package Control 安装，虽然插件安装成功了，但是找不到配置文件 Nodejs.sublime-build 来更改一些配置 。于是去 https://packagecontrol.io/packages/Nodejs 官网上查看，只提供一种安装方式。    

## 2. 安装---git 安装   
```
MacOSX
`git clone https://github.com/tanepiper/SublimeText-Nodejs.git ~/Library/Application\ Support/Sublime\ Text\ 3/Packages/Nodejs`
Windows
`git clone https://github.com/tanepiper/SublimeText-Nodejs "%APPDATA%\Sublime Text 3\Packages\Nodejs"`
Linux
`git clone https://github.com/tanepiper/SublimeText-Nodejs $HOME/.config/sublime-text-3/Packages/Nodejs`
```
**Note:**    
After the package is installed, install Node.js packages needed for working the package built-in tools. You can to install either globaly by: npm install -g commander@"~2.9.0" uglify-js@"~2.6.0" or localy by adding previously mentioned Node.js packages to your current project, either: npm install commander@"~2.9.0" uglify-js@"~2.6.0" or manully editing your project package.json file.   

## 3. 修改配置文件(两处要修改)   
3.1 Nodejs 用户设置   
Preferences -> Package Settings -> Nodejs -> Settings User, 打开 Nodejs.sublime-settings 文件.  
修改后的文件内容为:    
```cpp
{
  // save before running commands
  "save_first": true,
  // if present, use this command instead of plain "node"
  // e.g. "/usr/bin/node" or "C:\bin\node.exe"
  "node_command": "C:\\Program Files\\nodejs\\node.exe" ,
  // Same for NPM command
  "npm_command": "C:\\Program Files\\nodejs\\npm.cmd",
  // as 'NODE_PATH' environment variable for node runtime
  "node_path": false,

  "expert_mode": false,

  "ouput_to_new_tab": false
}
```
注: 修改了两个地方，分别是 `node_command` 和 `npm_command`.    
3.2 Nodejs.sublime-build 编译器设置   
在 Sublie Text 3 Packages 文件目录下， 找到 Nodejs.sublime-build 文件，更改为以下内容:     
```
{
  "cmd": ["node", "$file"],
  "file_regex": "^[ ]*File \"(...*?)\", line ([0-9]*)",
  "selector": "source.js",
  "shell":true,
  "encoding": "utf8",
  "windows":
  {
      "cmd": ["taskkill","/F", "/IM", "node.exe","&","node", "$file"]
  },
  "linux":
  {
      "cmd": ["killall node; node", "$file"]
  },
  "osx":
  {
  "cmd": ["killall node; node $file"]
  }
}
```
注: 修改了两个地方，分别是 `encoding` 和 `windows` 下的 `cmd`, cmd 命令是每次执行的时候都会 kill 掉以前启动的 nodejs 进程，这个命令有些错误，我们修改它，到达我们想要的效果.   

## 3. 测试    

新建一个 test.js 文件 输入以前内容  
```js
var http = require('http');
var os = require('os');
 
http.createServer(function (request, response) {
  response.writeHead(200, {'Content-Type': 'text/plain'});
  response.end('Hello World\n');
 
}).listen(3000);
 
console.log('Server running at http://127.0.0.1:3000/');
```
Ctrl + B 编译一下，会在Sublime Text 控制台 看到 Server running at http://127.0.0.1:3000/ ，接下来我们从浏览器打开 访问 http://127.0.0.1:3000/ .   
