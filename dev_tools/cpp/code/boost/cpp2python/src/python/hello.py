#coding-utf-8
# import sys
# sys.path.insert(0, "/home/klm/work/gitwork/work_note/dev_tools/cpp/code/boost/cpp2python/src/python")
import hello_ext

print (hello_ext.greet())

# Three different initialization
planet = hello_ext.World() # 调用默认构造函数，产生类对象
planet = hello_ext.World(5, 6)
planet2 = hello_ext.World("constructor")

print (planet.sum_s())
print (planet2.greet())
planet.set("howdy")   # 调用对象的方法
print (planet.greet()) # 调用对象的方法