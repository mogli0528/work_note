# solver 的实现  
## 目录   
- Solver Register 的实现方式;   
- Solver 的实现方式;   

## Solver Register 注册机 ----> solver_factory.cpp   
> 每个 Solver 实现之后, 要调用 `REGISTER_SOLVER_CLASS(SGD)` 注册. 其目的是将实现好的所有 `Solver` 都创建一个对应的 `g_creator_f_##type` 和 `g_creator_d_##type`类型的对象, 这样就保证每个 `creator` 中有属于自己的 g`_registry_` 了.   

1. `caffe` 中定义一个 `Solver` 的操作.     
```cpp
shared_ptr<caffe::Solver<float> > solver(caffe::SolverRegistry<float>::CreateSolver(solver_param))   

// Solver 构造函数.   
explicit Solver(const SolverParameter& param);  // 不支持隐式转换
```
其中 `CreateSolver(const SolverParameter& )`函数是一个 `static` 类型的函数. 具体实现如下:  
```cpp
// 使用一个 SolverParameter 获取一个求解器.   
// 最后还是会调用到 Solver() 的构造函数.   
static Solver<Dtype>* CreateSolver(const SolverParameter& param) 
{
    const string& type = param.type();
    CreatorRegistry& registry = Registry();
    CHECK_EQ(registry.count(type), 1) << "Unknown solver type: " << type
        << " (known types: " << SolverTypeListString() << ")";
    
    return registry[type](param);
}
```
2. `Registry()` 函数中 `static` 的作用, 功能类似于全局变量.   
```cpp
typedef Solver<Dtype>* (*Creator)(const SolverParameter&);
typedef std::map<string, Creator> CreatorRegistry;

static CreatorRegistry& Registry() 
{
    static CreatorRegistry* g_registry_ = new CreatorRegistry();
    return *g_registry_;
}
```
`static` 表示只在首次调用函数的时候分配空间(new CreatorRegistry()), 之后就不再对 `g_registry_` 变量进行赋值了.   
3. `map` 的操作.   
```cpp
// 添加一个 creator.
static void AddCreator(const string& type, Creator creator) 
{
    CreatorRegistry& registry = Registry();
    CHECK_EQ(registry.count(type), 0)
        << "Solver type " << type << " already registered.";
    registry[type] = creator;
}
```
首先判断 `map` 中是否存在这个键值, 如果不存在, 就将这个 `creator` 添加到 `g_registry_` 中, 否则什么都不做, 表示 `creator` 已经存在于这个 `map` 中.   

## Solver 的实现方式 ---> solver.cpp
> Solver 类最终会被具体的求解器类(例如 SGDSolver)继承.   

1. Solver 对应的 Action.   
 1) NONE: 不做任何处理;   
 2) STOP: 停止训练;   
 3) SNAPSHOT: 保存一个快照之后继续训练;   
2. Solver 求解器类型的默认值为 SGD; 
3. Snapshot() 要保存两部分的数据: 一部分为网络参数数据, 另一部分为已经学习到的求解器状态参数.   
```cpp
template <typename Dtype>
void SGDSolver<Dtype>::SnapshotSolverStateToBinaryProto(
    const string& model_filename) {
  SolverState state;
  state.set_iter(this->iter_);
  state.set_learned_net(model_filename);
  state.set_current_step(this->current_step_);
  state.clear_history();
  for (int i = 0; i < history_.size(); ++i) {
    // Add history
    BlobProto* history_blob = state.add_history();
    history_[i]->ToProto(history_blob);
  }
  string snapshot_filename = Solver<Dtype>::SnapshotFilename(".solverstate");
  LOG(INFO)
    << "Snapshotting solver state to binary proto file " << snapshot_filename;
  WriteProtoToBinaryFile(state, snapshot_filename.c_str());
}
```
history_ 中保存的是 historical momentum data.   
同时在 history_blob 中不保存 diff 数据.   

`---------------------------不得不去研究几个基本的模块了----------------------------`

