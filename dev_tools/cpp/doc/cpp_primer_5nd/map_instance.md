# map 使用实例   

《 C++ Primer 5nd 》 P391    

## map 的基本操作   
### 插入   
```cpp
map<int, int> label_hist;
label_hist.insert(std::make_pair(0, 0));
label_hist.insert(std::make_pair(label, 0));
int label = 2;
label_hist[label]++;

label_hist[0]++;
```
### 删除  
```cpp
// 删除迭代器指向位置的键值对，并返回一个指向下一元素的迭代器
iterator erase( iterator pos )

// 删除一定范围内的元素，并返回一个指向下一元素的迭代器
iterator erase( const_iterator first, const_iterator last );

// 根据 Key 来进行删除， 返回删除的元素数量，在map里结果非0即1
size_t erase( const key_type& key );

// 清空map，清空后的size为0
void clear();
```

## map 排序    

`pair` 的使用.    

```cpp
vector<std::pair<int, int> > mapping;
const Dtype* perm = bottom[1]->cpu_data();
for (int i = 0; i < bottom[1]->count(); ++i) {
    mapping.push_back(pair<int, int>(static_cast<int>(perm[i]), i));
}
std::sort(mapping.begin(), mapping.end(), pair_sort_first());

```

