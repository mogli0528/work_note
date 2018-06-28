# caffe C++ 接口到 pycaffe   

> /home/klm/work/gitwork/ssd/python/caffe/_caffe.cpp
> /home/klm/work/gitwork/ssd/python/caffe/pycaffe.py

## cpp 到 Python 接口的包装   

"""
Wrap the internal caffe C++ module (_caffe.so) with a clean, Pythonic
interface.
"""

`python/caffe/pycaffe.py` 文件对 `_caffe` 进行了二次包装, 使得接口更加 Pythonic.   
 
```python
from ._caffe import Net, SGDSolver, NesterovSolver, AdaGradSolver, \
        RMSPropSolver, AdaDeltaSolver, AdamSolver
import caffe.io
```


## 包装的流程   

BOOST_PYTHON_MODULE