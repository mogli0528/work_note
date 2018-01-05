<center><h1>Training Multi-Layer Neural Network with Caffe</h1></center>


```python
import subprocess
import platform
import sys

sys.path.append("/home/klm/work/gitwork/ssd/python")
import caffe
caffe.set_mode_gpu()
import lmdb

from sklearn.cross_validation import StratifiedShuffleSplit
import pandas as pd
import numpy as np

import math

import matplotlib.pyplot as plt
%matplotlib inline

import os

def check_if_exist(path):
    return os.path.exists(path)

def make_if_not_exist(path):
    if not os.path.exists(path):
        os.makedirs(path)
```

    /usr/local/lib/python2.7/dist-packages/sklearn/cross_validation.py:41: DeprecationWarning: This module was deprecated in version 0.18 in favor of the model_selection module into which all the refactored classes and functions are moved. Also note that the interface of the new CV iterators are different from that of this module. This module will be removed in 0.20.
      "This module will be removed in 0.20.", DeprecationWarning)


# Status Quo


```python
print "OS:      ", platform.platform()
print "Python:  ", sys.version.split("\n")[0]
print "CUDA:    ", subprocess.Popen(["nvcc", "--version"], stdout=\
                            subprocess.PIPE).communicate()[0].\
                            split("\n")[3]
print "LMDB:    ", ".".join([str(i) for i in lmdb.version()])
```

    OS:       Linux-4.4.0-105-generic-x86_64-with-Ubuntu-14.04-trusty
    Python:   2.7.6 (default, Nov 23 2017, 15:49:48) 
    CUDA:     Cuda compilation tools, release 8.0, V8.0.61
    LMDB:     0.9.18


# Load Data from CSV and Trasform

The CSV is assumed to be the training data from the ["Otto Group Product Classification Challenge"](https://www.kaggle.com/c/otto-group-product-classification-challenge) at Kaggle. It contains 95 columns:
- [0] id (discarded)
- [1..93] features (integer values)
- [94] label (9 categories - `Class_1,..,Class_9`)


```python
df = pd.read_csv("train.csv", sep = ",")
features = df.ix[:, 1:-1].as_matrix()
labels = df.ix[:,-1].as_matrix()
labels = np.reshape(labels, (np.shape(features)[0], 1))

#vec_log = np.vectorize(lambda x: log(x+1))

vec_log = np.vectorize(lambda x: long(x+1))
vec_int = np.vectorize(lambda x: int(x[-1])-1)

features = vec_log(features)
labels = vec_int(labels)
```

    /usr/local/lib/python2.7/dist-packages/ipykernel_launcher.py:2: DeprecationWarning: 
    .ix is deprecated. Please use
    .loc for label based indexing or
    .iloc for positional indexing
    
    See the documentation here:
    http://pandas.pydata.org/pandas-docs/stable/indexing.html#ix-indexer-is-deprecated
      


# Stratified Split for Training and Testing


```python
sss = StratifiedShuffleSplit(labels, 1, test_size=0.02, random_state=0)
sss = list(sss)[0]

features_training = features[sss[0], ]
labels_training = labels[sss[0], ]

features_testing = features[sss[1], ]
labels_testing = labels[sss[1], ]

```

# Load Data into LMDB


```python
# http://deepdish.io/2015/04/28/creating-lmdb-in-python/
def load_data_into_lmdb(lmdb_name, features, labels=None):
    make_if_not_exist(os.path.split(lmdb_name)[0])
    print lmdb_name
    if check_if_exist(lmdb_name):
        return
    
    
    env = lmdb.open(lmdb_name, map_size = features.nbytes * 2)
    
    features = features[:, :, None, None]
    for i in range(features.shape[0]):
        datum = caffe.proto.caffe_pb2.Datum()
        
        datum.channels = features.shape[1]
        datum.height = 1
        datum.width = 1
        
        if features.dtype == np.int:
            datum.data = features[i].tostring()
        elif features.dtype == np.float:
            datum.float_data.extend(features.flat)
        else:
            raise Exception("features.dtype unknown.")
        
        if labels is not None:
            datum.label = int(labels[i])
            
        str_id = '{:08}'.format(i)
        with env.begin(write=True) as txn:
            txn.put(str_id, datum.SerializeToString())
```


```python
# 很费时间, 第一次生成之后不必重复运行
load_data_into_lmdb("lmdb/train_data_lmdb", features_training, labels_training)
load_data_into_lmdb("lmdb/test_data_lmdb", features_testing, labels_testing)
```

    lmdb/train_data_lmdb
    lmdb/test_data_lmdb



```python
print features_training.shape[0]
```

    60640


# Check Content of LMDB


```python
# http://research.beenfrog.com/code/2015/03/28/read-leveldb-lmdb-for-caffe-with-python.html
def get_data_for_case_from_lmdb(lmdb_name, id):
    lmdb_env = lmdb.open(lmdb_name, readonly=True)
    lmdb_txn = lmdb_env.begin()
    
    raw_datum = lmdb_txn.get(id)
    datum = caffe.proto.caffe_pb2.Datum()
    datum.ParseFromString(raw_datum)
    
    feature = caffe.io.datum_to_array(datum)
    label = datum.label
    
    return (label, feature)
```


```python
get_data_for_case_from_lmdb("lmdb/train_data_lmdb", "00012345")
```


    ---------------------------------------------------------------------------

    ValueError                                Traceback (most recent call last)

    <ipython-input-13-b2bff8224acb> in <module>()
    ----> 1 get_data_for_case_from_lmdb("lmdb/train_data_lmdb", "00012345")
    

    <ipython-input-12-a25e971613ab> in get_data_for_case_from_lmdb(lmdb_name, id)
          8     datum.ParseFromString(raw_datum)
          9 
    ---> 10     feature = caffe.io.datum_to_array(datum)
         11     label = datum.label
         12 


    /home/klm/work/gitwork/ssd/python/caffe/io.pyc in datum_to_array(datum)
         88     if len(datum.data):
         89         return np.fromstring(datum.data, dtype=np.uint8).reshape(
    ---> 90             datum.channels, datum.height, datum.width)
         91     else:
         92         return np.array(datum.float_data).astype(float).reshape(


    ValueError: cannot reshape array of size 744 into shape (93,1,1)
