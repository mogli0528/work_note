# gitbook 安装    

~~~bash
sudo npm install -g gitbook-cli
~~~

**Note**:   

You need to install "gitbook-cli" to have access to the gitbook command anywhere on your system.  
If you've installed this package globally, you need to uninstall it.  
>> Run "npm uninstall -g gitbook" then "npm install -g gitbook-cli"  


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
