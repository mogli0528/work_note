# C++ Primer 5nd 知识点总结

## 容器
1. `swap()`函数比赋值拷贝快的多，因为`swap()`只交换了2个容器的内部数据结构。         
2. 与其他容器不同的是，对一个string调用swap()函数会导致迭代器、引用和指针失效。        
3. 与其他容器不同，对两个array调用swap()会交换其元素，因此交换两个array的时间和array的数据个数成正比。        
4. 容器既有成员版本的swap()，也有非成员版本的swap()，统一使用非成员版本的swap()是一个好习惯。        
5. assign()允许我们从一个不同但相容的类型赋值。          
6. 不能对内置数组类型进行拷贝和赋值，但array并无此限制。       
7. 对非常量调用这些成员(begin(),cbegin(),end(),cend()等)时，得到的是iterator。只有对常量对象(const)调用这些函数时得到的是`const iterator`。     
8. 对于没有默认构造函数的类，构造函数时不能只传递给它一个元素数目。         
9. c.clear()会删除c中的所有元素。            
10. forward_list迭代器不支持递减`--`运算符。          
11. 按照约定，end()不会在begin()之前。         
12. 容器的关系运算符使用元素的关系运算符完成比较。         
13. front(),back(),下标和at返回的是对象的引用，因此要用`auto &ret`来接收其返回值。             
14. emplace()插入函数调用的是类的构造函数。     
15. insert()将元素插入到迭代器指定的位置之前，其返回值指向新加入元素的迭代器(c++11)。          
16. list, forward_list, deque支持push_front()函数，放入容器中的是对象值的拷贝，而不是对象本身。          
17. 添加元素到vector中可能引起整个对象存储空间的重新分配。         
     
