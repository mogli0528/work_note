# tensorflow 处理输入数据的api   

1. tf.train.slice_input_producer()
module: tensorflow.python.training.input.    
```python
slice_input_producer(tensor_list, 
	num_epochs=None, 
	shuffle=True, 
	seed=None, 
	capacity=32, 
	shared_name=None, 
	name=None)

    从 `tensor_list` 的每个 `Tensor` 中产生一个切片. 
    
    使用 Queue 实现 -- 会在当前 `Graph` 的 `QUEUE_RUNNER` 集合中添加一个
    `QueueRunner`.
    
    Args:
      tensor_list: A list of `Tensor` objects. Every `Tensor` in
        `tensor_list` must have the same size in the first dimension.
      num_epochs: An integer (optional). If specified, `slice_input_producer`
        produces each slice `num_epochs` times before generating
        an `OutOfRange` error. If not specified, `slice_input_producer` can cycle
        through the slices an unlimited number of times.
      shuffle: Boolean. If true, the integers are randomly shuffled within each
        epoch.
      seed: An integer (optional). Seed used if shuffle == True.
      capacity: An integer. Sets the queue capacity.
      shared_name: (optional). If set, this queue will be shared under the given
        name across multiple sessions.
      name: A name for the operations (optional).
    
    Returns:
      `tensor_list` 的其中一对元素组成的 list. 如果 `tensor_list` 的 shape 为
      `[N, a, b, .., z]`,那么对应的输出的 shape 为 `[a, b, ..., z]`.
    
    Raises:
      ValueError: `slice_input_producer` 从 `tensor_list` 中的返回值为 None .
```
2. tf.train.string_input_producer()    
module: tensorflow.python.training.input.  
用于创建文件名,接下来会创建一个 `reader` 来读取数据.   
```python
string_input_producer(string_tensor, 
	num_epochs=None, 
	shuffle=True, 
	seed=None, 
	capacity=32, 
	shared_name=None, 
	name=None, 
	cancel_op=None)

    向一个输入管道(input pipeline)的 queue 中输出 strings. 
    
    Note: 如果 `num_epochs` 不为 `None`, 该函数会创建 local counter `epochs`. 
    使用 `local_variables_initializer()` 来初始化 local variables.

    Args:
      string_tensor: A 1-D string tensor with the strings to produce.
      num_epochs: An integer (optional). If specified, `string_input_producer`
        produces each string from `string_tensor` `num_epochs` times before
        generating an `OutOfRange` error. If not specified,
        `string_input_producer` can cycle through the strings in `string_tensor`
        an unlimited number of times.
      shuffle: Boolean. If true, the strings are randomly shuffled within each
        epoch.
      seed: An integer (optional). Seed used if shuffle == True.
      capacity: An integer. Sets the queue capacity.
      shared_name: (optional). If set, this queue will be shared under the given
        name across multiple sessions.
      name: A name for the operations (optional).
      cancel_op: Cancel op for the queue (optional).
    
    Returns:
      A queue with the output strings. 会在当前 `Graph` 的 `QUEUE_RUNNER` 集合中添加一个 `QueueRunner`.
    
    Raises:
      ValueError: If the string_tensor is a null Python list.  At runtime,
      will fail with an assertion if string_tensor becomes a null tensor.
```
3. tf.FixedLengthRecordReader()    
module: tensorflow.python.ops.io_ops.    
用于从文件中读取固定长度的数据.   
```python
class FixedLengthRecordReader(ReaderBase)
 |  A Reader that outputs fixed-length records from a file.
 |    
 |  Methods defined here:
 |  
 |  __init__(self, record_bytes, header_bytes=None, footer_bytes=None, name=None)
 |      Create a FixedLengthRecordReader.
 |      
 |      Args:
 |        record_bytes: An int.
 |        header_bytes: An optional int. Defaults to 0.
 |        footer_bytes: An optional int. Defaults to 0.
 |        name: A name for the operation (optional).
 |  
 |  ----------------------------------------------------------------------
 |  Methods inherited from ReaderBase:
 |  
 |  num_records_produced(self, name=None)
 |      Returns the number of records this reader has produced.
 |      
 |      This is the same as the number of Read executions that have
 |      succeeded.
 |      
 |      Args:
 |        name: A name for the operation (optional).
 |      
 |      Returns:
 |        An int64 Tensor.
 |  
 |  num_work_units_completed(self, name=None)
 |      Returns the number of work units this reader has finished processing.
 |      
 |      Args:
 |        name: A name for the operation (optional).
 |      
 |      Returns:
 |        An int64 Tensor.
 |  
 |  read(self, queue, name=None)
 |      Returns the next record (key, value pair) produced by a reader.
 |      
 |      Will dequeue a work unit from queue if necessary (e.g. when the
 |      Reader needs to start reading from a new file since it has
 |      finished with the previous file).
 |      
 |      Args:
 |        queue: A Queue or a mutable string Tensor representing a handle
 |          to a Queue, with string work items.
 |        name: A name for the operation (optional).
 |      
 |      Returns:
 |        A tuple of Tensors (key, value).
 |        key: A string scalar Tensor.
 |        value: A string scalar Tensor.
 |  
 |  read_up_to(self, queue, num_records, name=None)
 |      Returns up to num_records (key, value pairs) produced by a reader.
 |      
 |      Will dequeue a work unit from queue if necessary (e.g., when the
 |      Reader needs to start reading from a new file since it has
 |      finished with the previous file).
 |      It may return less than num_records even before the last batch.
 |      
 |      Args:
 |        queue: A Queue or a mutable string Tensor representing a handle
 |          to a Queue, with string work items.
 |        num_records: Number of records to read.
 |        name: A name for the operation (optional).
 |      
 |      Returns:
 |        A tuple of Tensors (keys, values).
 |        keys: A 1-D string Tensor.
 |        values: A 1-D string Tensor.
 |  
 |  reset(self, name=None)
 |      Restore a reader to its initial clean state.
 |      
 |      Args:
 |        name: A name for the operation (optional).
 |      
 |      Returns:
 |        The created Operation.
 |  
 |  restore_state(self, state, name=None)
 |      Restore a reader to a previously saved state.
 |      
 |      Not all Readers support being restored, so this can produce an
 |      Unimplemented error.
 |      
 |      Args:
 |        state: A string Tensor.
 |          Result of a SerializeState of a Reader with matching type.
 |        name: A name for the operation (optional).
 |      
 |      Returns:
 |        The created Operation.
 |  
 |  serialize_state(self, name=None)
 |      Produce a string tensor that encodes the state of a reader.
 |      
 |      Not all Readers support being serialized, so this can produce an
 |      Unimplemented error.
 |      
 |      Args:
 |        name: A name for the operation (optional).
 |      
 |      Returns:
 |        A string Tensor.
 | 
```