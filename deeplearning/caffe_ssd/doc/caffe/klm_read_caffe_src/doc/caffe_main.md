# caffe 的入口    

## main() 函数入口    
我们在使用 caffe 训练时, 一般是使用例如以下的命令:    
```
/path/to/caffe.bin train --solver=solver.prototxt --weights=ResNet-50-model.caffemodel --gpu 0,1,2,3
```
其实除了训练模型或微调模型之外, caffe 还包含其他的命令行参数:   

- train           train or finetune a model    
- test            score a model   
- device_query    show GPU diagnostic information   
- time            benchmark model execution time   
这些都是在 caffe.cpp 的 main() 函数中实现的.   

```cpp
int main(int argc, char** argv) {
  // logging 时将标准输出信息同时定位到 stderr.
  FLAGS_alsologtostderr = 1;
  // 设置 caffe 的版本号
  gflags::SetVersionString(AS_STRING(CAFFE_VERSION));
  // 本程序的帮助信息.
  gflags::SetUsageMessage("command line brew\n"
      "usage: caffe <command> <args>\n\n"
      "commands:\n"
      "  train           train or finetune a model\n"
      "  test            score a model\n"
      "  device_query    show GPU diagnostic information\n"
      "  time            benchmark model execution time");
  // Run tool or show usage.
  caffe::GlobalInit(&argc, &argv);
  if (argc == 2) {
#ifdef WITH_PYTHON_LAYER
    try {
#endif
      return GetBrewFunction(caffe::string(argv[1]))();
#ifdef WITH_PYTHON_LAYER
    } catch (bp::error_already_set) {
      PyErr_Print();
      return 1;
    }
#endif
  } else {
    gflags::ShowUsageWithFlagsRestrict(argv[0], "tools/caffe");
  }
}
```
1. 开始是 gflags 的使用. 作用是定制你自己的 help 信息与软件 version 信息. 因为 gflags 已经定义了 -h 和 --version，你可以通过以下方式定制它们的内容:   
- version信息：使用 `google::SetVersionString()` 设定，使用 `google::VersionString()`访问
- help信息：使用 `google::SetUsageMessage()` 设定，使用 `google::ProgramUsage()`访问
注意： `google::SetUsageMessage()` 和 `google::SetVersionString()` 必须在 `google::ParseCommandLineFlags()` 之前执行.   
2. 接下来  `GlobalInit(int*, char*** )`函数初始化 gflags 和 glog.    
3. 最后 `GetBrewFunction()` 根据参数 `argv[1]` 调用相应的回调函数, 如果命令行参数传递错误, 则会提示相应的帮助信息. 例如:   
```
$ ./caffe
caffe: command line brew
usage: caffe <command> <args>

commands:
  train           train or finetune a model
  test            score a model
  device_query    show GPU diagnostic information
  time            benchmark model execution time

  Flags from tools/caffe.cpp:
    -gpu (Optional; run in GPU mode on given device IDs separated by ','.Use
      '-gpu all' to run on all available GPUs. The effective training batch
      size is multiplied by the number of devices.) type: string default: ""
    ...
    -weights (Optional; the pretrained weights to initialize finetuning,
      separated by ','. Cannot be set simultaneously with snapshot.)
      type: string default: ""

```

## caffe 支持的四个命令行参数的实现方式     
```cpp
// 一个简单的 caffe 命令注册函数.   
typedef int (*BrewFunction)();
typedef std::map<caffe::string, BrewFunction> BrewMap;
BrewMap g_brew_map;

#define RegisterBrewFunction(func) \
namespace { \
class __Registerer_##func { \
 public: /* NOLINT */ \
  __Registerer_##func() { \
    g_brew_map[#func] = &func; \
  } \
}; \
__Registerer_##func g_registerer_##func; \
}

static BrewFunction GetBrewFunction(const caffe::string& name) {
  if (g_brew_map.count(name)) {  // 有效命令行参数  
    return g_brew_map[name];
  } else {
    LOG(ERROR) << "Available caffe actions:";
    for (BrewMap::iterator it = g_brew_map.begin();
         it != g_brew_map.end(); ++it) {
      LOG(ERROR) << "\t" << it->first;
    }
    LOG(FATAL) << "Unknown action: " << name;
    return NULL;  // not reachable, just to suppress old compiler warnings.
  }
}
```
这是一个简单的 caffe 命令注册函数.   
1. 每个命令("train", "test", "device_query", "time")对应的是一个实现好的函数("int train()", "int test()", "int device_query()", "int time()"), 因此最好的保存方式就是用关联容器 map.  
2. `RegisterBrewFunction(func)` 中使用到的是 C 语言中的宏的高级用法. `__Registerer_##func()` 中 `##` 起字符串拼接作用, 而`g_brew_map[#func] = &func` 中 `#` 起参数当做字符串使用. 因此,对于 `RegisterBrewFunction(test)`, 上述两行的展开就是 `__Registerer_test()` 和 `g_brew_map[test] = &test`.   
3. `GetBrewFunction(const caffe::string& )` 在 map 中按照键值查找函数入口后返回.  

## int train() 函数的实现    
train() 函数用于训练或者微调一个模型.   
```cpp
// 训练 / 微调 一个模型.
int train() {
  // 表示 --solver 参数的字符个数 
  CHECK_GT(FLAGS_solver.size(), 0) << "Need a solver definition to train.";  
  CHECK(!FLAGS_snapshot.size() || !FLAGS_weights.size())
      << "Give a snapshot to resume training or weights to finetune "
      "but not both.";
  vector<string> stages = get_stages_from_flags();

  caffe::SolverParameter solver_param;
  caffe::ReadSolverParamsFromTextFileOrDie(FLAGS_solver, &solver_param);

  solver_param.mutable_train_state()->set_level(FLAGS_level);
  for (int i = 0; i < stages.size(); i++) {
    solver_param.mutable_train_state()->add_stage(stages[i]);
  }

  // If the gpus flag is not provided, allow the mode and device to be set
  // in the solver prototxt.
  if (FLAGS_gpu.size() == 0
      && solver_param.has_solver_mode()
      && solver_param.solver_mode() == caffe::SolverParameter_SolverMode_GPU) {
      if (solver_param.has_device_id()) {
          FLAGS_gpu = "" +
              boost::lexical_cast<string>(solver_param.device_id());
      } else {  // Set default GPU if unspecified
          FLAGS_gpu = "" + boost::lexical_cast<string>(0);
      }
  }

  vector<int> gpus;
  get_gpus(&gpus);
  if (gpus.size() == 0) {
    LOG(INFO) << "Use CPU.";
    Caffe::set_mode(Caffe::CPU);
  } else {
    ostringstream s;
    for (int i = 0; i < gpus.size(); ++i) {
      s << (i ? ", " : "") << gpus[i];
    }
    LOG(INFO) << "Using GPUs " << s.str();
#ifndef CPU_ONLY
    cudaDeviceProp device_prop;
    for (int i = 0; i < gpus.size(); ++i) {
      cudaGetDeviceProperties(&device_prop, gpus[i]);
      LOG(INFO) << "GPU " << gpus[i] << ": " << device_prop.name;
    }
#endif
    solver_param.set_device_id(gpus[0]);
    Caffe::SetDevice(gpus[0]);
    Caffe::set_mode(Caffe::GPU);
    Caffe::set_solver_count(gpus.size());
  }

  //-/ 安装 caffe 信号捕捉处理函数(Ctrl+C 中断和 Ctrl+Z 挂起).  
  caffe::SignalHandler signal_handler(
        GetRequestedAction(FLAGS_sigint_effect),
        GetRequestedAction(FLAGS_sighup_effect));

  shared_ptr<caffe::Solver<float> >
      solver(caffe::SolverRegistry<float>::CreateSolver(solver_param));

  solver->SetActionFunction(signal_handler.GetActionFunction());

  //-/ FLAGS_snapshot 指示是否需要继续训练  
  //-/ FLAGS_weights 指示是否在预训练模型上微调  
  if (FLAGS_snapshot.size()) {
    LOG(INFO) << "Resuming from " << FLAGS_snapshot;
    solver->Restore(FLAGS_snapshot.c_str());
  } else if (FLAGS_weights.size()) {
    CopyLayers(solver.get(), FLAGS_weights);
  }

  LOG(INFO) << "Starting Optimization";
  if (gpus.size() > 1) {
#ifdef USE_NCCL
    caffe::NCCL<float> nccl(solver);
    nccl.Run(gpus, FLAGS_snapshot.size() > 0 ? FLAGS_snapshot.c_str() : NULL);
#else
    LOG(FATAL) << "Multi-GPU execution not available - rebuild with USE_NCCL";
#endif
  } else {
    solver->Solve();
  }
  LOG(INFO) << "Optimization Done.";
  return 0;
}

RegisterBrewFunction(train);
```
> 一些参数的含义?   
DEFINE_string(phase, "",
    "Optional; network phase (TRAIN or TEST). Only used for 'time'.");
DEFINE_int32(level, 0,
    "Optional; network level.");
DEFINE_string(stage, "",
    "Optional; network stages (not to be confused with phase), "
    "separated by ','.");
上述3个参数一起构成了solver的NetState.   
message NetState {
  optional Phase phase = 1 [default = TEST];
  optional int32 level = 2 [default = 0];
  repeated string stage = 3;
}

1. FLAGS_solver.size() 表示 --solver 参数的字符个数.   
2. stages 是什么意思?   
3. `ReadSolverParamsFromTextFileOrDie()` 函数从 `solver.prototxt`文件中读取求解器参数, 该函数最终会调用 `google::protobuf::TextFormat::Parse(input, proto)` 函数执行解析操作. 之后检查旧的参数, 如果需要的话进行升级.    
4. train/test 网络的 state. 每个网络最多只能指定一个 state.   
  默认情况下, train_state.phase = TRAIN, test_state.phase = TEST.
  其他域则被初始化为 NetState 的默认值.   
```proto
optional NetState train_state = 26;   
repeated NetState test_state = 27;   
```
5. 设置 GPU 模式 及其 id 号, 求解器的个数等于 gpu 的个数.    
6. 注册两个信号: 中断信号和挂起信号.  
7. 如果是模型微调, 则将预训练模型 `.caffemodel` 指定的参数复制一份; 如果是继续训练, 则将模型参数恢复为 `.solverstate` 指定的状态.      
8. caffe 中对于多 GPU 的实现和 ssd 中是不同的. caffe 中使用的是 `USE_NCCL`宏控制, 而 ssd 中使用的是 `P2PSync`.   
9. 调用 `solver->solve()` 开始求解.   

## TODO 其他的三个命令行参数   


## 结语   
按理说接下来应该是分析求解器 `Solver.cpp`, 但是在 `Solver.cpp` 中就会涉及到 `Net` 和 `Blob` 了, 因此还是暂且去分析一下 `Caffe` 中基本的组件(Blob, Net, Layer), 之后就可以回来继续分析 `Solver` 了;

