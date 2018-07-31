# gitbook 安装 / 配置   

## gitbook 安装 

安装 gitbook 之前, 需要安装 Nodejs. Nodejs 的安装参考[这里](../system_install/nodejs_install.md).   

~~~bash
sudo npm install -g gitbook-cli
~~~

**Note**:   

You need to install "gitbook-cli" to have access to the gitbook command anywhere on your system.  
If you've installed this package globally, you need to uninstall it.  
>> Run "npm uninstall -g gitbook" then "npm install -g gitbook-cli"  

## 配置 gitbook 显示

如果 repo 中没有 book.json , 则需要配置生成电子书的参数, 例如:  

~~~json
{
    "title": "Work Note",
    "description": "problem solving ",
    "author": "zh",
    "language": "zh-hans",
    "styles": {
        "website": "style.css"
    },
    "plugins": [
        "-lunr",
        "-search",
        "-livereload",
        "-sharing",
        "book-summary-scroll-position-saver",
        "expandable-chapters",
        "search-plus",
        "splitter",
        "tbfed-pagefooter",
        "back-to-top-button",
        "yahei",
        "katex",
        "-theme-api",
        "theme-comscore"
    ],
    "pluginsConfig": {
        "expandable-chapters": {},
        "theme-default": {
            "showLevel": true
        },
        "prism": {
            "css": [
            "prismjs/themes/prism-tomorrow.css"
            ]
        },
        "tbfed-pagefooter": {
            "copyright": "&copy; magic428",
            "modify_label": "文件修订时间：",
            "modify_format": "YYYY-MM-DD HH:mm:ss"
        },
        "fontSettings": {
            "theme": "night",
            "family": "msyh",
            "size": 2
        },
        "theme-api": { "theme": "dark" }
    },
    "pdf": {
        "toc": true,
        "pageNumbers": true,
        "fontSize": 18,
        "paperSize": "a4",
        "margin": {
          "right": 30,
          "left": 30,
          "top": 30,
          "bottom": 50
        }
      }
}
~~~

在运行 `gitbook init` 时就会检测到需要安装插件, 在 repo 目录下运行 `gitbook install` 命令后就会自动安装 book.json 中的插件了.  

**解释一下**:  

* "-theme-api": "-" 表示禁用某个插件, 禁用 theme-api 插件后可以使用 night 主题;   
* "splitter": 表示目录可以折叠;  


## 遇到的问题   

1. npm install failed   

~~~
klm@klm:~$ npm install gitbook -g
npm http GET https://registry.npmjs.org/gitbook
npm http GET https://registry.npmjs.org/gitbook
^Cklm@klm:~/work/gitwork/dl_ai$ npm install -g gitbook-cli
npm http GET https://registry.npmjs.org/gitbook-cli
npm http GET https://registry.npmjs.org/gitbook-cli
npm http GET https://registry.npmjs.org/gitbook-cli
npm ERR! Error: CERT_UNTRUSTED
...
~~~

`CERT_UNTRUSTED` 说明 ssl 验证有问题， 使用下面的命令取消 ssl 验证即可:   

~~~bash
npm config set strict-ssl false
~~~

2. 设置 npm 的 registry.   

~~~bash
npm config set registry="http://registry.npmjs.org/"
npm config set registry http://registry.npm.taobao.org/
~~~
