# string 操作   

## 构造函数    

|string 构造函数|功能|
|:--------------|:-----------|
|string s(cp)| s 是 cp 字符指针指向的数组中的字符串|
|string s(cp, n)| s 是 cp 字符指针指向的数组中前 n 个字符的拷贝|
|string s(s1, pos1)| s 是 s1 从下标 pos1 开始的所有字符拷贝, 默认从下标 0 开始拷贝|
|string s(s2, pos2, len)| s 是 s2 从下标 pos2 开始的 len 个字符拷贝|

## substr 操作   

substr 返回一个 string, 它是原始 string 的一部分或全部的拷贝.   

可以传递给 substr 一个 `可选的` 开始位置和计数值.    

```cpp
substr(pos, n)
```
返回包含 s 中从 pos 开始的 n 个字符的拷贝. pos 的默认值为 0, n 的默认值为 s.size() - pos.   

## 改变 string 的其他方法   
|修改 string 的操作|功能|
|:--------------|:-----------|
|s.insert(pos, args)| 在 pos 之前插入 args 指定的字符, pos 可以是下标或迭代器|
|s.erase(pos, len)| 删除从 pos 开始的 len 个字符, 返回一个指向 a 的引用|
|s.assign(args)| 将 s 中的字符替换为 args 中指定的字符, 返回一个指向 a 的引用 |
|s.append(args)| 将 args 中指定的字符追加到 s, 返回一个指向 a 的引用|
|s.replace(pos, len, args)| 先从 pos 位置开始删除 n 个元素, 然后将 str 插入到 pos 位置之前, 返回一个指向 a 的引用|
|s.replace(b, e, args)| 删除 b, e 范围内的元素, 然后将 args 指定的字符插入到 b 位置之前, 返回一个指向 a 的引用|

args 可以是以下 形式之一, append 和 assign 可以使用所有形式.   
str 不能和 s 相同, 迭代器 b 和 e 不能指向 s.    
str                 字符串 str   
str, pos, len       str 中从 pos 开始的(最多) len 个字符   
cp                  cp 指向空字符结尾的字符数组   
cp, len             从 cp 中指向的数组的前(最多) len 个字符   
n, c                n 个字符 c   
b, e                迭代器 b 和 e 指定的范围内的字符   
初始化列表            花括号包围的, 以逗号分隔的字符列表   
1. string 类型支持顺序容器的赋值运算符, assign, insert 和 erase 操作. 除此之外, 它还定义了额外的 insert 和 erase 版本.   
除了接受迭代器的 insert 和 erase 版本外, string 还提供了接受下标的版本. 下标指出了开始删除的位置, 或是 insert 到给定下标之前的位置.   
```cpp
a.insert(a.size(), 5, '!');
a.insert(a.size(), "@@");

const char * cp_ = "*^_^*";
a.insert(a.size()-7, cp_);

a.erase(a.size()-7, 7);

``` 
2. append 和 replace 函数   
```cpp
a.append(" Chinese Edition.");
a.replace(pos, n, str)
```
replace 操作是先从 pos 位置开始删除 n 个元素, 然后将 str 插入到 pos 位置. 返回一个指向 a 的引用.     

## string 的搜索操作   

如果搜索失败, 返回一个名为 string::npos 的 static 成员(实际为 static const string::size_type npos = -1).   

|string 的搜索操作|功能|
|:--------------|:-----------|
|s.find(args)| 查找 s 中 args 第一次出现的位置 |
|s.rfind(args)| 查找 s 中 args 最后一次出现的位置 |
|s.find_first_of(args)| 在 s 中查找 args 中任何一个字符第一次出现的位置|
|s.find_last_of(args)| 在 s 中查找 args 中任何一个字符最后一次出现的位置|
|s.find_first_not_of(args)| 在 s 中查找第一个不在 args 中的字符|
|s.find_last_not_of(args)| 在 s 中查找最后一个不在 args 中的字符|

其中 args 必须是以下形式之一:   
c, pos   
s2, pos   
cp, pos   
cp, pos, n   
pos 指定在哪里开始搜索, 前三个 pos 的默认值为 0, 最后一个的 pos 和 n 无默认值.     

## 数值转换.    

|string 和数值之间的转换操作|功能|
|:--------------|:-----------|
|to_string(val)| 一组重载函数, 返回数值 val 的 string表示 |
|stoi(s, p, b)| 返回 s 表示的整数值, s 的起始子串内容必须表示整数|
|stol(s, p, b)| 返回 s 表示的整数值, s 的起始子串内容必须表示整数|
|stoul(s, p, b)| 返回 s 表示的整数值, s 的起始子串内容必须表示整数|
|stoll(s, p, b)| 返回 s 表示的整数值, s 的起始子串内容必须表示整数|
|stoull(s, p, b)| 返回 s 表示的整数值, s 的起始子串内容必须表示整数|
|stof(s, p)| 返回 s 表示的浮点数, s 的起始子串内容必须表示浮点数|
|stod(s, p)| 返回 s 表示的浮点数, s 的起始子串内容必须表示浮点数|
|stold(s, p)| 返回 s 表示的浮点数, s 的起始子串内容必须表示浮点数|

其中 p 用来保存 s 中第一个非数值字符的下标.   

## 大小写转换   

```cpp
string word = "Hello String!";

// 全部转换为小写
transform(word.begin(), word.end(), word.begin(), (int (*)(int))tolower);

// 全部转换为大写
transform(word.begin(), word.end(), word.begin(), (int (*)(int))toupper);
```