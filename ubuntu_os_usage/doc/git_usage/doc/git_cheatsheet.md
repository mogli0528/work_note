# 常见的 git 命令使用

## `git cheatsheet`   

1. 添加远程仓库   

~~~bash
$ git remote add origin git@github.com:gzj2013/xxx.git
~~~

2. 删除远程仓库   

~~~bash
$ git remote rm origin 
~~~

3. 添加 .gitignore 模板   

~~~conf
# 不添加所有以 .log 结尾的文件
*.log

# 不添加所有以 . 开头的文件
.*

# 上一步把 .gitignore 排除了, 这里再加进来
!.gitignore

# 编译时产生的中间文件
*.o

# 不包含以下目录
vscode/
~~~

**注意**: 不要在这些规则的后边添加多余的空格, 否则会导致过滤规则失效.   

4. git add -u  

只是删除了本地文件, 远程仓库的并没有删除, 但是此时想要直接删除远程仓库中对应的文件。   

5. 撤销修改     

**场景1**： 当你改乱了工作区某个文件的内容, 想直接丢弃工作区的修改时, 用命令:   

~~~bash
git checkout file_xxx
~~~

**场景2**： 当你不但改乱了工作区某个文件的内容, 还添加到了暂存区时, 想丢弃修改, 分两步：   

第一步:  

~~~bash
git reset HEAD file_xxx
~~~

这样就回到了场景 1;   

第二步: 按场景 1 操作.   

**场景3**： 已经提交了不合适的修改到版本库时, 想要撤销本次提交(撤销提交后本地修改仍然会保留的)。  

~~~bash
git reset HEAD^
~~~

**场景4**： 想要恢复本地误删的文件, 包括用 git rm -f 删除的文件.   

~~~bash
git checkout xxx_file
~~~

由此可见, git checkout 不仅可以丢弃工作区修改, 而且可以恢复已经被删除的本地文件.   


**场景5**： 修改已经提交的 commit 内容(此时还未被 push 到远程仓库).  

~~~bash
$ git commit --amend
~~~

6.  Git 冲突合并   

~~~
<<<<<<<   
A的修改 
=======
B的修改
>>>>>>>
~~~

删除的文件内容需要统一, 要么所有的地方要么都保留 A 的修改, 要么都保留 B 的修改。      

7. 配置 Git 使用 SS5 代理   

~~~bash
$ git config --global http.proxy 'socks5://127.0.0.1:1080' 
$ git config --global https.proxy 'socks5://127.0.0.1:1080'

# 取消代理
$ git config --global --unset http.proxy
$ git config --global --unset https.proxy
~~~

8. 撤销  

9. 文件删除操作  

`git rm` 有两个不同的选项操作.  

* git rm --cached "file_xxx",  不删除物理文件, 仅将该文件从 Git 远程仓库中删除；   
* git rm --f "file_xxx",  不仅将该文件从  Git  远程仓库中删除, 还会将物理文件删除（且不会经过回收站）.   

10. Git 添加多个远程仓库  

比如你有 oschina 和 github.   

~~~bash
git remote add origin https://github.com/xxx(仓库地址)  #添加github
git remote add oschina https://git.oschina.net/xxxx(仓库地址)  #添加oschina

git push oschina master(分支名)  #提交到 oschina
git push origin master(分支名)   #提交到 github

git pull oschina master  # 从 oschina 更新
git pull origin master   # 从 github 更新
~~~

git remote add <name> <url>, 其中, name 表示你要给这个远程库起的名字, url 表示这个库的地址.   
git push <name> <branch>, 其中, name 表示你在上一步给它起的名字, branch 表示某一个分支.  

11. 访问 github.com 超时。  

connect to host github.com port 22: Connection timed out.   

~~~
$ ssh -T git@github.com 
ssh: connect to host github.com port 22: Connection timed out
~~~

**解决办法**： 具体操作步骤详见这篇博客 - [加速 git clone xxx_url ](accelerate_git_clone.md)   

12. https 方式每次提交代码都要输入密码   

按照如下设置即可输入一次就不用再手输入密码的困扰而且又享受 https 带来的极速。  

1) 设置记住密码（默认15分钟）   

~~~bash
git config --global credential.helper cache
~~~

2) 如果想自己设置时间, 可以这样做  

~~~bash
git config credential.helper 'cache --timeout=3600'
~~~

这样就设置一个小时之后失效。   

3) 长期存储密码   

~~~bash
git config --global credential.helper store
~~~
