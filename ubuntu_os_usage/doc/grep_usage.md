# grep 命令中支持的正则表达式    

## 1. grep正则表达式元字符集   

1.1 元字符集表    
|元字符集|描述|示例|
|-----|-----|------|
|`^` 		|锚定行的开始 |`^grep`匹配所有以grep开头的行  |
|`$` 		|锚定行的结束 |`grep$`匹配所有以grep结尾的行  |
|`.` 		|匹配一个非换行符的字符 |`gr.p`匹配gr后接一个任意字符，然后是p  |
|`*` 		|匹配零个或多个先前字符 |`*grep`匹配所有一个或多个空格后紧跟grep的行  `.*`一起用代表任意字符 |
|`[]` 		|匹配`一个`指定范围内的字符 |`[Gg]rep`匹配Grep和grep  |
|`[^]` 		|匹配一个不在指定范围内的字符 |`[^A-FH-Z]rep`匹配不包含A-R和T-Z的一个字母开头，紧跟rep的行  |
|`\(..\)` 	|标记匹配字符 |`\(love\)`，love被标记为1  |
|`\` 		|锚定单词的开始 |`\`匹配包含以grep开头的单词的行  |
|`\>` 		|锚定单词的结束 |`grep\>`匹配包含以grep结尾的单词的行  |
|`x\{m\} `	|重复字符x，m次 |`0\{5\}`匹配包含5个o的行  |
|`x\{m,\} `	|重复字符x,至少m次 |`o\{5,\}`匹配至少有5个o的行  |
|`x\{m,n\}`	|重复字符x，至少m次，不多于n次 |`o\{5,10\}`匹配5--10个o的行 |
|`\w`		|匹配文字和数字字符，也就是[A-Za-z0-9]|`G\w*p`匹配以G后跟零个或多个文字或数字字符，然后是p |
|`\b` 		|单词锁定符 | `\bgrep\b`只匹配grep |
1.2 详细描述   
1.2.1 星号 `*`    
匹配 `*` 号前面的字符串或正则表达式任意次(`包括0次`)。   
比如：`1122*`   
可能匹配的字符串有：`112、1122、1122222、1123131231 ......`.  
**疑问**: 为什么会有 112 被匹配到?   
1.2.2 句号 `.`   
匹配除换行符之外的`任意一个`字符。   
比如：`112.`，将匹配`112`+`至少一个字符`   
可能匹配的字符串有：`1121、1122、112abc、1123131231 ......`，但不匹配`112`.  
**Note:** `.*`匹配匹配任意字符串。  
1.2.3 插入符号 `^`   
匹配一行的开始，但有时依赖于上下文环境，也可能表示`否定正则表达式`中一个字符串的意思。   
比如：`^abc`只匹配`行首`为 `abc` 的字符串.   
1.2.4 美元符 `$`    
在一个正则表达式的末尾，匹配一行的结尾。   
比如：`123$`，将只能匹配到`行尾`为 `123` 的字符行.    
**Note:** `^$` 匹配一个空行。   
1.2.5 方括号 `[]`   
匹配方括号内指定的字母集中的一个字符。其实 `[]` 里面不论有几个字符，它都仅代表某`一个`字符.   
比如：   
`[abc]`，将匹配字符 `a、b、c` 中的任意一个字符；   
`[^abc]`，将匹配除 `abc` 之外的所有字符。   
1.2.6 反斜线符号 `\`    
转义一个特殊的字符，使这个字符得到字面意义的解释。   
比如：`\$`，将表示它的字面字符 `$`； `\\`，表示 `\`。   
1.2.7 转义尖括号 `\<\>`  
用于标记单词边界。尖括号必须是转义的，否则它们只是字符的字面含义。  
比如：`\<the\>` 匹配 `the`，但不匹配 `them`，`there`。   
1.2.8 问号 `?`    
匹配 `0` 个或 `1` 个前面的字符，用于匹配单个字符。  
比如：`ab?c`，将匹配"ac"或"abc"。  
1.2.9 加号 `+`   
匹配一个或多个前面的字符，它和星号 `*` 很相似，但它不匹配 `0` 个字符的情况。   
比如：`ab+c`将匹配`abc`，`abbc`，`abbb...c`等。    
1.2.10 转义波形括号 `\{\}`   
指示匹配前面正则表达式的次数。波形括号必须是转义的，否则只有字符的字面含义。   
比如：`[0-9]\{5\}`将匹配 `5` 位数字。    
1.2.11 圆括号 `()`   
包含一组正则表达式。与 `|` 或 `expr` 操作符一起使用。  
1.2.12 竖线 `|`    
正则表达式的 `或` 操作符匹配一组可选的字符。  
比如：`a(b|c)d` 将匹配 `abd` 或 `acd`。  
1.2.13 比较运算符 `=~`  
必须在 `[[   ]]` 里面(不推荐使用),可直接用支持正则的工具代替.
```bash
$ digit=132
$ [[ $digit =~ [0-9] ]]
$ echo $?
0  # 说明上边的表达式为真

$ digit="sfsfs"
$ [[ $digit =~ [0-9] ]]
$ echo $?
1  # 说明上边的表达式为假
```
## 2. 常用的 grep 选项    

|选项|描述|
|-----|-----|
|-c |只输出匹配行的个数 |
|-i |不区分大小写（只适用于单字符） |
|-h |查询多文件时不显示文件名 |
|-l |查询多文件时只输出包含匹配字符的文件名 |
|-n |显示匹配行及行号 |
|-s |不显示不存在或无匹配文本的错误信息 |
|-v |显示不包含匹配文本的所有行 |
|-V |显示软件版本信息|
|-a |将 binary 文件以 text 文件的方式搜寻数据|

## 3. 匹配的实例     
使用`grep`匹配时最好用双引号引起来，防止被系统误认为参数或者特殊命令，也可以匹配多个单词。   

|使用示例|作用|
|-----|-----|
|grep -c "48" test.txt 		|统计所有以“48”字符开头的行有多少|
|grep -i "May" test.txt 	|不区分大小写查找“May”所有的行）|
|grep -n "48" test.txt 		|显示行号；显示匹配字符“48”的行及行号，相同于 nl test.txt |grep 48）|
|grep -v "48" test.txt 		|显示输出没有字符“48”所有的行）|
|grep "471" test.txt 		|显示输出字符“471”所在的行）|
|grep "48;" test.txt 		|显示输出以字符“48”开头，并在字符“48”后是一个tab键所在的行|
|grep "48[34]" test.txt 	|显示输出以字符“48”开头，第三个字符是“3”或是“4”的所有的行）|
|grep "^[^48]" test.txt 	|显示输出行首不是字符“48”的行）|
|grep "[Mm]ay" test.txt 	|设置大小写查找：显示输出第一个字符以“M”或“m”开头，以字符“ay”结束的行）|
|grep "K…D" test.txt 		|显示输出第一个字符是“K”，第二、三、四是任意字符，第五个字符是“D”所在的行）|
|grep "[A-Z][9]D" test.txt  |显示输出第一个字符的范围是“A-D”，第二个字符是“9”，第三个字符的是“D”的所有的行|
|grep "[35]..1998" test.txt |显示第一个字符是3或5，第二三个字符是任意，以1998结尾的所有行|
|grep "4\{2,\}" test.txt 	|模式出现几率查找：显示输出字符“4”至少重复出现两次的所有行|
|grep "9\{3,\}" test.txt 	|模式出现几率查找：显示输出字符“9”至少重复出现三次的所有行|
|grep "9\{2,3\}" test.txt   |模式出现几率查找：显示输出字符“9”重复出现的次数在一定范围内，重复出现2次或3次所有行|
|grep -n "^$" test.txt 		|显示输出空行的行号|
|ls -l |grep "^d" 			|如果要查询目录列表中的目录 同：`ls -d *`|
|ls -l |grep "^d[d]" 		|在一个目录中查询不包含目录的所有文件|
|ls -l |grpe "^d…..x..x" 	|查询其他用户和用户组成员有可执行权限的目录集合|


grep与正规表达式
 字符类

1. 字符类的搜索：如果我想要搜寻 test 或 taste 这两个单字时，可以发现到，其实她们有共通的 't?st' 存在～这个时候，我可以这样来搜寻：

grep -n 't[ae]st' regular_express.txt

8:I can't finish the test.
9:Oh! The soup taste good.

其实 [] 里面不论有几个字节，他都谨代表某『一个』字节， 所以，上面的例子说明了，我需要的字串是『tast』或『test』两个字串而已！

 

2. 字符类的反向选择 [^] ：如果想要搜索到有 oo 的行，但不想要 oo 前面有 g，如下

```
grep -n '[^g]oo' regular_express.txt
2:apple is my favorite food.
3:Football game is not use feet only.
18:google is the best tools for search keyword.
19:goooooogle yes!
```
第 2,3 行没有疑问，因为 foo 与 Foo 均可被接受！

但是第 18 行明明有 google 的 goo 啊～别忘记了，因为该行后面出现了 tool 的 too 啊！所以该行也被列出来～ 也就是说， 18 行里面虽然出现了我们所不要的项目 (goo) 但是由於有需要的项目 (too) ， 因此，是符合字串搜寻的喔！

至於第 19 行，同样的，因为 goooooogle 里面的 oo 前面可能是 o ，例如： go(ooo)oogle ，所以，这一行也是符合需求的！

 

3. 字符类的连续：再来，假设我 oo 前面不想要有小写字节，所以，我可以这样写 [^abcd....z]oo ， 但是这样似乎不怎么方便，由於小写字节的 ASCII 上编码的顺序是连续的， 因此，我们可以将之简化为底下这样：

[root@www ~]# grep -n '[^a-z]oo' regular_express.txt
3:Football game is not use feet only.
也就是说，当我们在一组集合字节中，如果该字节组是连续的，例如大写英文/小写英文/数字等等， 就可以使用[a-z],[A-Z],[0-9]等方式来书写，那么如果我们的要求字串是数字与英文呢？ 呵呵！就将他全部写在一起，变成：[a-zA-Z0-9]。

4. 我们要取得有数字的那一行，就这样：

[root@www ~]# grep -n '[0-9]' regular_express.txt
5:However, this dress is about $ 3183 dollars.
15:You are the best is mean you are the no. 1.
 

5. 行首与行尾字节 ^ $
行首字符：如果我想要让 the 只在行首列出呢？ 这个时候就得要使用定位字节了！我们可以这样做：

[root@www ~]# grep -n '^the' regular_express.txt
12:the symbol '*' is represented as start.
 


此时，就只剩下第 12 行，因为只有第 12 行的行首是 the 开头啊～

6. 如果我想要开头是小写字节的那一行就列出呢？可以这样：

复制代码
[root@www ~]# grep -n '^[a-z]' regular_express.txt
2:apple is my favorite food.
4:this dress doesn't fit me.
10:motorcycle is cheap than car.
12:the symbol '*' is represented as start.
18:google is the best tools for search keyword.
19:goooooogle yes!
20:go! go! Let's go.
复制代码
 

7. 如果我不想要开头是英文字母，则可以是这样：

[root@www ~]# grep -n '^[^a-zA-Z]' regular_express.txt
1:"Open Source" is a good mechanism to develop programs.
21:# I am VBird
^ 符号，在字符类符号(括号[])之内与之外是不同的！ 在 [] 内代表『反向选择』，在 [] 之外则代表定位在行首的意义！

 

7. 那如果我想要找出来，行尾结束为小数点 (.) 的那一行：

复制代码
[root@www ~]# grep -n '\.$' regular_express.txt
1:"Open Source" is a good mechanism to develop programs.
2:apple is my favorite food.
3:Football game is not use feet only.
4:this dress doesn't fit me.
10:motorcycle is cheap than car.
11:This window is clear.
12:the symbol '*' is represented as start.
15:You are the best is mean you are the no. 1.
16:The world <Happy> is the same with "glad".
17:I like dog.
18:google is the best tools for search keyword.
20:go! go! Let's go.
复制代码

特别注意到，因为小数点具有其他意义(底下会介绍)，所以必须要使用转义字符(\)来加以解除其特殊意义！

 

8. 找出空白行：

[root@www ~]# grep -n '^$' regular_express.txt
22:
因为只有行首跟行尾 (^$)，所以，这样就可以找出空白行啦！

 

9. 任意一个字节 . 与重复字节 *
这两个符号在正则表达式的意义如下：

. (小数点)：代表『一定有一个任意字节』的意思；
* (星号)：代表『重复前一个字符， 0 到无穷多次』的意思，为组合形态
假设我需要找出 g??d 的字串，亦即共有四个字节， 起头是 g 而结束是 d ，我可以这样做：

[root@www ~]# grep -n 'g..d' regular_express.txt
1:"Open Source" is a good mechanism to develop programs.
9:Oh! The soup taste good.
16:The world <Happy> is the same with "glad".
因为强调 g 与 d 之间一定要存在两个字节，因此，第 13 行的 god 与第 14 行的 gd 就不会被列出来啦！

 

如果我想要列出有 oo, ooo, oooo 等等的数据， 也就是说，至少要有两个(含) o 以上，该如何是好？

因为 * 代表的是『重复 0 个或多个前面的 RE 字符』的意义， 因此，『o*』代表的是：『拥有空字节或一个 o 以上的字节』，因此，『 grep -n 'o*' regular_express.txt 』将会把所有的数据都列印出来终端上！

当我们需要『至少两个 o 以上的字串』时，就需要 ooo* ，亦即是：

复制代码
[root@www ~]# grep -n 'ooo*' regular_express.txt
1:"Open Source" is a good mechanism to develop programs.
2:apple is my favorite food.
3:Football game is not use feet only.
9:Oh! The soup taste good.
18:google is the best tools for search keyword.
19:goooooogle yes!
复制代码
 

如果我想要字串开头与结尾都是 g，但是两个 g 之间仅能存在至少一个 o ，亦即是 gog, goog, gooog.... 等等，那该如何？

[root@www ~]# grep -n 'goo*g' regular_express.txt
18:google is the best tools for search keyword.
19:goooooogle yes!
 

如果我想要找出 g 开头与 g 结尾的行，当中的字符可有可无

[root@www ~]# grep -n 'g.*g' regular_express.txt
1:"Open Source" is a good mechanism to develop programs.
14:The gd software is a library for drafting programs.
18:google is the best tools for search keyword.
19:goooooogle yes!
20:go! go! Let's go.

因为是代表 g 开头与 g 结尾，中间任意字节均可接受，所以，第 1, 14, 20 行是可接受的喔！ 这个 .* 的 RE 表示任意字符是很常见的.

 

如果我想要找出『任意数字』的行？因为仅有数字，所以就成为：

[root@www ~]# grep -n '[0-9][0-9]*' regular_express.txt
5:However, this dress is about $ 3183 dollars.
15:You are the best is mean you are the no. 1.
 

 

限定连续 RE 字符范围 {}
我们可以利用 . 与 RE 字符及 * 来配置 0 个到无限多个重复字节， 那如果我想要限制一个范围区间内的重复字节数呢？

举例来说，我想要找出两个到五个 o 的连续字串，该如何作？这时候就得要使用到限定范围的字符 {} 了。 但因为 { 与 } 的符号在 shell 是有特殊意义的，因此， 我们必须要使用字符   \ 来让他失去特殊意义才行。 至於 {} 的语法是这样的，假设我要找到两个 o 的字串，可以是：

复制代码
[root@www ~]# grep -n 'o\{2\}' regular_express.txt
1:"Open Source" is a good mechanism to develop programs.
2:apple is my favorite food.
3:Football game is not use feet only.
9:Oh! The soup taste good.
18:google is the best tools for search ke
19:goooooogle yes!
复制代码
 

假设我们要找出 g 后面接 2 到 5 个 o ，然后再接一个 g 的字串，他会是这样：

[root@www ~]# grep -n 'go\{2,5\}g' regular_express.txt
18:google is the best tools for search keyword.
 

如果我想要的是 2 个 o 以上的 goooo....g 呢？除了可以是 gooo*g ，也可以是：

[root@www ~]# grep -n 'go\{2,\}g' regular_express.txt
18:google is the best tools for search keyword.
19:goooooogle yes!
 

扩展grep(grep -E 或者 egrep)：
使用扩展grep的主要好处是增加了额外的正则表达式元字符集。

 

打印所有包含NW或EA的行。如果不是使用egrep，而是grep，将不会有结果查出。

    # egrep 'NW|EA' testfile     
    northwest       NW      Charles Main        3.0     .98     3       34
    eastern         EA      TB Savage           4.4     .84     5       20
 

对于标准grep，如果在扩展元字符前面加\，grep会自动启用扩展选项-E。

#grep 'NW\|EA' testfile
northwest       NW      Charles Main        3.0     .98     3       34
eastern         EA      TB Savage           4.4     .84     5       20
 

搜索所有包含一个或多个3的行。

复制代码
# egrep '3+' testfile
# grep -E '3+' testfile
# grep '3\+' testfile        
#这3条命令将会
northwest       NW      Charles Main          3.0     .98     3       34
western         WE      Sharon Gray           5.3     .97     5       23
northeast       NE      AM Main Jr.           5.1     .94     3       13
central         CT      Ann Stephens          5.7     .94     5       13
复制代码
 

搜索所有包含0个或1个小数点字符的行。
    

复制代码
# egrep '2\.?[0-9]' testfile 
# grep -E '2\.?[0-9]' testfile
# grep '2\.\?[0-9]' testfile 
#首先含有2字符，其后紧跟着0个或1个点，后面再是0和9之间的数字。
western         WE       Sharon Gray          5.3     .97     5       23
southwest       SW      Lewis Dalsass         2.7     .8      2       18
eastern         EA       TB Savage             4.4     .84     5       20
复制代码
 

搜索一个或者多个连续的no的行。
    

# egrep '(no)+' testfile
# grep -E '(no)+' testfile
# grep '\(no\)\+' testfile   #3个命令返回相同结果，
northwest       NW      Charles Main        3.0     .98     3       34
northeast       NE       AM Main Jr.        5.1     .94     3       13
north           NO      Margot Weber        4.5     .89     5       9
 

不使用正则表达式

fgrep 查询速度比grep命令快，但是不够灵活：它只能找固定的文本，而不是规则表达式。

如果你想在一个文件或者输出中找到包含星号字符的行

fgrep  '*' /etc/profile
for i in /etc/profile.d/*.sh ; do

或
grep -F '*' /etc/profile
for i in /etc/profile.d/*.sh ; do





