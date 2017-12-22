# python 字典操作

## 1. 将dict按照value进行排序
1、method 1.

items = dict.items()
items.sort()
for key,value in items:
   print key, value # print key,dict[key]
2、method 2.

print key, dict[key] for key in sorted(dict.keys())
　

python dict按照value排序：
method 1：

把dictionary中的元素分离出来放到一个list中，对list排序，从而间接实现对dictionary的排序。这个“元素”可以是key，value或者item。

method2：

#用lambda表达式来排序，更灵活：
sorted(dict.items(), lambda x, y: cmp(x[1], y[1]))
#降序
sorted(dict.items(), lambda x, y: cmp(x[1], y[1]), reverse=True)
下面给出python内置sorted函数的帮助文档：

sorted(...)
sorted(iterable, cmp=None, key=None, reverse=False) --> new sorted list

python list排序：
list有sort方法：

  如：
   >>> s=[2,1,3,0]
   >>> s.sort()
   [0, 1, 2, 3]


我们知道Python的内置dictionary数据类型是无序的，通过key来获取对应的value。可是有时我们需要对dictionary中 的item进行排序输出，可能根据key，也可能根据value来排。到底有多少种方法可以实现对dictionary的内容进行排序输出呢？下面摘取了 一些精彩的解决办法。
#最简单的方法，这个是按照key值排序：
def sortedDictValues1(adict):
items = adict.items()
items.sort()
return [value for key, value in items]

#又一个按照key值排序，貌似比上一个速度要快点
def sortedDictValues2(adict):
keys = adict.keys()
keys.sort()
return [dict[key] for key in keys]

#还是按key值排序，据说更快。。。而且当key为tuple的时候照样适用
def sortedDictValues3(adict):
keys = adict.keys()
keys.sort()
return map(adict.get, keys)
#一行语句搞定：
[(k,di[k]) for k in sorted(di.keys())]

#来一个根据value排序的，先把item的key和value交换位置放入一个list中，再根据list每个元素的第一个值，即原来的value值，排序：
def sort_by_value(d):
items=d.items()
backitems=[[v[1],v[0]] for v in items]
backitems.sort()
return [ backitems[i][1] for i in range(0,len(backitems))]

#还是一行搞定：
[ v for v in sorted(di.values())]
#用lambda表达式来排序，更灵活：
sorted(d.items(), lambda x, y: cmp(x[1], y[1])), 或反序：
sorted(d.items(), lambda x, y: cmp(x[1], y[1]), reverse=True)
#用sorted函数的key= 参数排序：
# 按照key进行排序
print sorted(dict1.items(), key=lambda d: d[0])
# 按照value进行排序 
print sorted(dict1.items(), key=lambda d: d[1])
下面给出python内置sorted函数的帮助文档：
sorted(...)
sorted(iterable, cmp=None, key=None, reverse=False) --> new sorted list
看了上面这么多种对dictionary排序的方法，其实它们的核心思想都一样，即把dictionary中的元素分离出来放到一个list中，对list排序，从而间接实现对dictionary的排序。这个“元素”可以是key，value或者item。
#################################################################################
一上转
按照value排序可以用
l = sorted(d.items(), key=lambda d:d[1])
若版本低不支持sorted
将key,value 以tuple一起放在一个list中
l = []
l.append((akey,avalue))...
用sort（）
l.sort(lambda a,b :cmp(a[1],b[1]))(cmp前加“-”表示降序排序)


博客园首页新随笔联系管理订阅
随笔- 31  文章- 1  评论- 6 
Python--关于dict
慕课网《Python 入门》学习笔记

 

1、dict 特性

dict用花括号｛｝表示，然后按照 key: value, 写出来即可。最后一个 key: value 的逗号可以省略。

①、dict 的查找速度快，无论dict有10个元素还是10万个元素，查找速度都一样。而list的查找速度随着元素增加而逐渐下降。

       dict的缺点是占用内存大，还会浪费很多内容，list正好相反，占用内存小，但是查找速度慢

②、dict 通过key 来查找 value ,因此key 不能重复，而value可重复

③、dict储存的"key:value"是无序的，即不可用索引号切片等。

Python的基本类型如字符串、整数、浮点数都是不可变的，都可以作为 key。但是list是可变的，就不能作为 key。

用 dict 表示“名字”-“成绩”的查找表如下：

 d = {
 'Adam': 95, #key : value
 'Lisa': 85,
 'Bart': 59
 }
我们把名字称为key，对应的成绩称为value，dict就是通过 key 来查找 value。

2、访问 dict

创建一个dict，用于表示名字和成绩的对应关系：

 d = {
 'Adam': 95,
 'Lisa': 85,
 'Bart': 59
 }
使用 d[key] 的形式来查找对应的 value，这和 list 很像，不同之处是，list 必须使用索引返回对应的元素，而dict使用key：

 23:30:12
注意: 通过 key 访问 dict 的value，只要 key 存在，dict就返回对应的value。如果key不存在，会直接报错：KeyError。

要避免 KeyError 发生，有两个办法：

一是先判断一下 key 是否存在，用 in 操作符：

 if 'Paul' in d:
 print d['Paul']
如果 'Paul' 不存在，if语句判断为False，自然不会执行 print d['Paul'] ，从而避免了错误。

二是使用dict本身提供的一个 get 方法 dict.get(key, default=None)，在Key不存在的时候，返回默认值None：

 >>> print d.get('Bart')
 59
 >>> print d.get('Paul')
 None
 3、更新 dict 
 
dict是可变的，可以随时往dict中添加新的 key-value。比如已有dict：
 d = {
 'Adam': 95,
 'Lisa': 85,
 'Bart': 59
 }

 要把新同学'Paul'的成绩 72 加进去，用赋值语句：
 >>> d['Paul'] = 72 

 再看看dict的内容：
 >>> print d
 {'Lisa': 85, 'Paul': 72, 'Adam': 95, 'Bart': 59}

 如果 key 已经存在，则赋值会用新的 value 替换掉原来的 value：
 >>> d['Bart'] = 60
 >>> print d

 {'Lisa': 85, 'Paul': 72, 'Adam': 95, 'Bart': 60｝

4、删除dict元素或清空dict
　　可使用pop方法 dict.pop(key[,default])，通过key值删除dict内元素，并返回被删除key对应的value。若key不存在，且default值未设置，则返回KeyError异常
复制代码
>>> a
{1: 'abc', 2: 'efg', 3: 'hij'}
>>> a.pop(1)
'abc'
>>> a
{2: 'efg', 3: 'hij'}
>>> 
>>> a
{2: 'efg', 3: 'hij'}
>>> a.pop(1,False)
False
>>>
复制代码
　　可使用clear方法dict.clear()清空dict

5、遍历/迭代 dict 
 Ⅰ、for循环遍历：由于dict也是一个集合，所以，遍历dict和遍历list类似，都可以通过 for 循环实现。 
 >>> d = { 'Adam': 95, 'Lisa': 85, 'Bart': 59 }4
>>> for key in d:
 print key,'-',d[key]
 Lisa - 85
Adam - 95
Bart - 59
Ⅱ、values() / itervalues() 方法：返回dict 的value值 
 values()方法：把 dict 转换成了包含 value 的list 
 >>> d = { 'Adam': 95, 'Lisa': 85, 'Bart': 59 }
>>> print d.values()
[85, 95, 59]
>>> for v in d.values():
 print v
 85
95
59
 用 itervalues() 方法替代 values() 方法，迭代效果完全一样。而 itervalues() 方法不会转换，它会在迭代过程中依次从 dict 中取出 value，所以 itervalues() 方法比 values() 方法节省了生成 list 所需的内存。 
 Ⅲ、items() / iteritems() 方法：返回dict 的key和value 
 
dict 对象的 items() 方法返回的值：
 >>> d = { 'Adam': 95, 'Lisa': 85, 'Bart': 59 } >>> print d.items() [('Lisa', 85), ('Adam', 95), ('Bart', 59)]

 可以看到，items() 方法把dict对象转换成了包含tuple的list，对这个list进行迭代，可以同时获得key和value：
 >>> for key, value in d.items(): ... print key, ':', value ... Lisa : 85 Adam : 95 Bart : 59

 和 values() 有一个 itervalues() 类似， items() 也有一个对应的 iteritems()，iteritems() 不把dict转换成list，而是在迭代过程中不断给出 tuple，所以， iteritems() 不占用额外的内存。


 dict

Python内置了字典：dict的支持，dict全称dictionary，在其他语言中也称为map，使用键-值（key-value）存储，具有极快的查找速度。

举个例子，假设要根据同学的名字查找对应的成绩，如果用list实现，需要两个list：

names = ['Michael', 'Bob', 'Tracy']
scores = [95, 75, 85]
给定一个名字，要查找对应的成绩，就先要在names中找到对应的位置，再从scores取出对应的成绩，list越长，耗时越长。

如果用dict实现，只需要一个“名字”-“成绩”的对照表，直接根据名字查找成绩，无论这个表有多大，查找速度都不会变慢。用Python写一个dict如下：

>>> d = {'Michael': 95, 'Bob': 75, 'Tracy': 85}
>>> d['Michael']
95
为什么dict查找速度这么快？因为dict的实现原理和查字典是一样的。假设字典包含了1万个汉字，我们要查某一个字，一个办法是把字典从第一页往后翻，直到找到我们想要的字为止，这种方法就是在list中查找元素的方法，list越大，查找越慢。

第二种方法是先在字典的索引表里（比如部首表）查这个字对应的页码，然后直接翻到该页，找到这个字。无论找哪个字，这种查找速度都非常快，不会随着字典大小的增加而变慢。

dict就是第二种实现方式，给定一个名字，比如'Michael'，dict在内部就可以直接计算出Michael对应的存放成绩的“页码”，也就是95这个数字存放的内存地址，直接取出来，所以速度非常快。

你可以猜到，这种key-value存储方式，在放进去的时候，必须根据key算出value的存放位置，这样，取的时候才能根据key直接拿到value。

把数据放入dict的方法，除了初始化时指定外，还可以通过key放入：

>>> d['Adam'] = 67
>>> d['Adam']
67
由于一个key只能对应一个value，所以，多次对一个key放入value，后面的值会把前面的值冲掉：

>>> d['Jack'] = 90
>>> d['Jack']
90
>>> d['Jack'] = 88
>>> d['Jack']
88
如果key不存在，dict就会报错：

>>> d['Thomas']
Traceback (most recent call last):
  File "<stdin>", line 1, in <module>
KeyError: 'Thomas'
要避免key不存在的错误，有两种办法，一是通过in判断key是否存在：

>>> 'Thomas' in d
False
二是通过dict提供的get()方法，如果key不存在，可以返回None，或者自己指定的value：

>>> d.get('Thomas')
>>> d.get('Thomas', -1)
-1
注意：返回None的时候Python的交互环境不显示结果。

要删除一个key，用pop(key)方法，对应的value也会从dict中删除：

>>> d.pop('Bob')
75
>>> d
{'Michael': 95, 'Tracy': 85}
请务必注意，dict内部存放的顺序和key放入的顺序是没有关系的。

和list比较，dict有以下几个特点：

查找和插入的速度极快，不会随着key的增加而变慢；
需要占用大量的内存，内存浪费多。
而list相反：

查找和插入的时间随着元素的增加而增加；
占用空间小，浪费内存很少。
所以，dict是用空间来换取时间的一种方法。

dict可以用在需要高速查找的很多地方，在Python代码中几乎无处不在，正确使用dict非常重要，需要牢记的第一条就是dict的key必须是不可变对象。

这是因为dict根据key来计算value的存储位置，如果每次计算相同的key得出的结果不同，那dict内部就完全混乱了。这个通过key计算位置的算法称为哈希算法（Hash）。

要保证hash的正确性，作为key的对象就不能变。在Python中，字符串、整数等都是不可变的，因此，可以放心地作为key。而list是可变的，就不能作为key：

>>> key = [1, 2, 3]
>>> d[key] = 'a list'
Traceback (most recent call last):
  File "<stdin>", line 1, in <module>
TypeError: unhashable type: 'list'
set

set和dict类似，也是一组key的集合，但不存储value。由于key不能重复，所以，在set中，没有重复的key。

要创建一个set，需要提供一个list作为输入集合：

>>> s = set([1, 2, 3])
>>> s
{1, 2, 3}
注意，传入的参数[1, 2, 3]是一个list，而显示的{1, 2, 3}只是告诉你这个set内部有1，2，3这3个元素，显示的顺序也不表示set是有序的。。

重复元素在set中自动被过滤：

>>> s = set([1, 1, 2, 2, 3, 3])
>>> s
{1, 2, 3}
通过add(key)方法可以添加元素到set中，可以重复添加，但不会有效果：

>>> s.add(4)
>>> s
{1, 2, 3, 4}
>>> s.add(4)
>>> s
{1, 2, 3, 4}
通过remove(key)方法可以删除元素：

>>> s.remove(4)
>>> s
{1, 2, 3}
set可以看成数学意义上的无序和无重复元素的集合，因此，两个set可以做数学意义上的交集、并集等操作：

>>> s1 = set([1, 2, 3])
>>> s2 = set([2, 3, 4])
>>> s1 & s2
{2, 3}
>>> s1 | s2
{1, 2, 3, 4}
set和dict的唯一区别仅在于没有存储对应的value，但是，set的原理和dict一样，所以，同样不可以放入可变对象，因为无法判断两个可变对象是否相等，也就无法保证set内部“不会有重复元素”。试试把list放入set，看看是否会报错。

再议不可变对象

上面我们讲了，str是不变对象，而list是可变对象。

对于可变对象，比如list，对list进行操作，list内部的内容是会变化的，比如：

>>> a = ['c', 'b', 'a']
>>> a.sort()
>>> a
['a', 'b', 'c']
而对于不可变对象，比如str，对str进行操作呢：

>>> a = 'abc'
>>> a.replace('a', 'A')
'Abc'
>>> a
'abc'
虽然字符串有个replace()方法，也确实变出了'Abc'，但变量a最后仍是'abc'，应该怎么理解呢？

我们先把代码改成下面这样：

>>> a = 'abc'
>>> b = a.replace('a', 'A')
>>> b
'Abc'
>>> a
'abc'
要始终牢记的是，a是变量，而'abc'才是字符串对象！有些时候，我们经常说，对象a的内容是'abc'，但其实是指，a本身是一个变量，它指向的对象的内容才是'abc'：

┌───┐                  ┌───────┐
│ a │─────────────────>│ 'abc' │
└───┘                  └───────┘
当我们调用a.replace('a', 'A')时，实际上调用方法replace是作用在字符串对象'abc'上的，而这个方法虽然名字叫replace，但却没有改变字符串'abc'的内容。相反，replace方法创建了一个新字符串'Abc'并返回，如果我们用变量b指向该新字符串，就容易理解了，变量a仍指向原有的字符串'abc'，但变量b却指向新字符串'Abc'了：

┌───┐                  ┌───────┐
│ a │─────────────────>│ 'abc' │
└───┘                  └───────┘
┌───┐                  ┌───────┐
│ b │─────────────────>│ 'Abc' │
└───┘                  └───────┘
所以，对于不变对象来说，调用对象自身的任意方法，也不会改变该对象自身的内容。相反，这些方法会创建新的对象并返回，这样，就保证了不可变对象本身永远是不可变的。

小结

使用key-value存储结构的dict在Python中非常有用，选择不可变对象作为key很重要，最常用的key是字符串。

tuple虽然是不变对象，但试试把(1, 2, 3)和(1, [2, 3])放入dict或set中，并解释结果。