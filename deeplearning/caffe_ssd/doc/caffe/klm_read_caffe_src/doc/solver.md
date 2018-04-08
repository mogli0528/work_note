# solver 的实现   
> 每个 Solver 实现之后, 要调用 `REGISTER_SOLVER_CLASS(SGD)` 注册.   

```cpp
shared_ptr<caffe::Solver<float> > solver(caffe::SolverRegistry<float>::CreateSolver(solver_param))   

// Solver 构造函数.   
explicit Solver(const SolverParameter& param);  // 不支持隐式转换

```


## Solver.cpp
```cpp
g_registry_

```

1. Solver 对应的 Action.   
- NONE: 不做任何处理;   
- STOP: 停止训练;   
- SNAPSHOT: 保存一个快照之后继续训练;   
2. Solver 求解器类型的默认值为 SGD;   
3. static 的作用, 功能类似于全局变量.     
```cpp
static CreatorRegistry& Registry() 
{
    static CreatorRegistry* g_registry_ = new CreatorRegistry();
    return *g_registry_;
}
```
只是在第一次调用这个函数的时候分配空间(new CreatorRegistry()), 之后就不对 `g_registry_` 变量进行赋值了.   