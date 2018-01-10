# 为什么你的 Git 仓库变得如此臃肿
2016.02.13 01:05
你已经掌握了 Git 的基本用法，只消熟练使用几个常用命令，足以应付开发过程中的绝大多数场景。在 Git 的帮助下，你过上了快乐的生活。然而，某天早上你一觉醒来之后，发现了一件令人纳闷的事情：“为什么我的 Git 仓库变得如此臃肿？”

## 情形一：仓库自身的增长

大多数版本控制系统存储的是一组初始文件，以及每个文件随着时间的演进而逐步积累起来的差异；而 Git 则会把文件的每一个差异化版本都记录在案（关于 Git 是如何存储数据的，请参阅这篇文章）。这意味着，即使你只改动了某个文件的一行内容，Git 也会生成一个全新的对象来存储新的文件内容。

如果你改动了一个很大的文件，问题就来了。

对象碎片

现在，让我们生成一个包含 1000000 行随机字符串的大文本文件，并把它添加到版本库中：

$ perl -le 'for (1..1000000) { print map { (0..9, "a".."z")[rand 36] } 1..80 }' > bigfile
$ git add bigfile
我们看到，Git 已经为这个文件生成了一个 Blob 对象，大小是 54M。

$ find .git/objects -type f
.git/objects/7f/a055b2d22855b67287e4e30d9a91584c8b27c1
$
$ du -ah    # 此处略去了无关输出
 54M    ./.git/objects/7f/a055b2d22855b67287e4e30d9a91584c8b27c1
 77M    ./bigfile
132M    .
如果往文件末尾添加一行，会怎么样呢？

$ perl -le 'print map { (0..9, "a".."z")[rand 36] } 1..80' >> bigfile
$ git add bigfile
可以看到，Git 生成了一个全新的 Blog 对象来存储新的文件内容，这个对象的大小同样是 54M。仓库瞬间患上了肥胖症。

$ find .git/objects -type f
.git/objects/7f/a055b2d22855b67287e4e30d9a91584c8b27c1
.git/objects/f3/79c10af3f3e497d37558ac2497fe3c69d2de89
$
$ du -ah    # 此处略去了无关输出
 54M    ./.git/objects/7f/a055b2d22855b67287e4e30d9a91584c8b27c1
 54M    ./.git/objects/f3/79c10af3f3e497d37558ac2497fe3c69d2de89
 77M    ./bigfile
186M    .
你的仓库里面现在有两个内容几乎完全相同，大小均为 54M 的庞大对象。如果 Git 可以只保存其中一个对象，再保存另一个对象与这个对象的差异内容，岂不妙哉？

gc 命令

“垃圾回收”是一个很亲切的功能。让我们开始吧：

$ git gc --prune=now
现在，重新检视一下仓库的大小，发现确实有效啊：

$ find .git/objects -type f
.git/objects/info/packs
.git/objects/pack/pack-9d75315485cb7bfbf51ce5c94a4535da99b58dbb.idx
.git/objects/pack/pack-9d75315485cb7bfbf51ce5c94a4535da99b58dbb.pack
$
$ du -ah    # 此处略去了无关输出
4.0K    ./.git/objects/pack/pack-9d75315485cb7bfbf51ce5c94a4535da99b58dbb.idx
 52M    ./.git/objects/pack/pack-9d75315485cb7bfbf51ce5c94a4535da99b58dbb.pack
 77M    ./bigfile
130M    .
运行 gc 命令之后，两个 Blob 对象不见了。Git 创建了一个包文件和一个索引文件。包文件中包含了之前的两个 Blob 对象，索引文件中包含了每个对象在包文件中的偏移信息。Git 在打包的过程中使用了增量编码方案（delta encoding），只保存对象的不同版本之间的差异，这使得仓库瘦身成功。

不过...

实际上，你并不需要手动调用 gc 命令。每当碎片对象过多，或者你向远端服务器发起推送的时候，Git 就会自动执行一次打包过程。

## 情形二：错误的大文件

让我们添加一个名为 new.txt 的新文件，并且执行两次提交：

$ git commit -m "first commit"
$ echo 'new file' > new.txt
$ git commit -a "second commit"
当执行第二次提交的时候，你突然发现，其实 bigfile 这个文件在项目中并没什么卵用。然而它很大。即使你把这个文件从项目中移除了，它还是会顽固地永远存在于你的提交历史中。有没有办法把这个文件从历次提交中彻底地移除呢？

办法是有的，不过务必要谨慎哦。

能够胜任这个任务的命令叫做 filter-branch：

$ git filter-branch --force --index-filter \
  'git rm --cached --ignore-unmatch bigfile' \
  --prune-empty --tag-name-filter cat -- --all
Rewrite 1d92bc51b15c80582cef9cfb27ee056f000590bc (1/2)rm 'bigfile'
Rewrite 4ef010df40a1e81b1f9a11391d63879b649e9690 (2/2)rm 'bigfile'

Ref 'refs/heads/master' was rewritten
然后，删除缓存的对象。这一步可以暂时跳过，等到确认完全不会出现问题之后再执行（可以说，这些缓存对象给你提供了撤销操作的最后一次机会）。

$ git for-each-ref --format='delete %(refname)' refs/original | git update-ref --stdin
$ git reflog expire --expire=now --all
$ git gc --prune=now
现在仓库的总大小只有 88K 了，是不是很棒：

$ du -ah    # 此处略去了无关输出
4.0K    ./.git/objects/pack/pack-846da79290b3ef2f6617aa8aab03e4f54439a40a.idx
4.0K    ./.git/objects/pack/pack-846da79290b3ef2f6617aa8aab03e4f54439a40a.pack
4.0K    ./new.txt
 88K    .
当然，你可能还需要把这一次的改动提交到远端仓库：

$ git push --force --verbose --dry-run
$ git push --force
