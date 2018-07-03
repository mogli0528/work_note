# coding=utf-8

import tensorflow as tf

'''
每次我们调用 tf.constant 时，我们都会在图中创建一个新的节点。即使该节点的功能与现有节点相同，
即使我们将节点重新分配给同一个变量，或者即使我们根本没有将其分配给一个变量，都会在图中创建一个
新的节点。
'''

# Set the node to None
node_two = None
print(node_two)

node_one = tf.constant(1)
node_two = tf.constant(2)
print(node_two)

# Graph only contains computations, no results
node_sum = node_one + node_two   # tf.add(node_one, node_two)

'''
会话的作用是处理内存分配和优化，使我们能够实际执行由计算图指定的计算。你可以将计算图想象为我们
想要执行的计算的「模版」：它列出了所有步骤。为了使用计算图，我们需要启动一个会话，它使我们能够
实际地完成任务； 例如，遍历模版的所有节点来分配一堆用于存储计算输出的存储器。为了使用 
TensorFlow 进行各种计算，你既需要计算图也需要会话。
'''

sess = tf.Session()
print(sess.run(node_sum))

'''
一般来说，sess.run() 的调用往往是 TensorFlow 最大的瓶颈之一，因此调用它的次数越少越好。
如果可以的话，在一个 sess.run() 的调用中返回多个项目，而不是进行多个调用。
'''
print(sess.run([node_two, node_sum]))

# Place Holder
'''
更有价值的应用可能涉及构建一个计算图，它接受输入，以某种（一致）方式处理它，并返回一个输出。
最直接的方法是使用占位符。占位符是一种用于接受外部输入的节点
'''
input_holder = tf.placeholder(tf.int32)
# print(sess.run(input_holder))


'''
print(sess.run(input_holder)) 操作会引发一个异常。因为占位符预计会被赋予一个值。但我们没有提供一个值，所以 TensorFlow 崩溃了。

InvalidArgumentError (see above for traceback): You must feed a value for 
placeholder tensor 'Placeholder' with dtype int32.

为了提供一个值，我们使用 sess.run() 的 feed_dixt 属性。
'''
print(sess.run(input_holder, feed_dict={input_holder: 2}))


# Compute Path

# Init
count_var = tf.get_variable('count', [])
node_zero = tf.constant(0.0)
node_assigned = tf.assign(count_var, node_zero)

sess.run(node_assigned)
print(sess.run(count_var))

const_init = tf.constant_initializer(0.0)

# 对于想要保存在内存中的每个变量，你只需要调用一次 tf.get_variable()。 
count_var_init = tf.get_variable('count_init', [], initializer=const_init)
init = tf.global_variables_initializer()
sess.run(init)

print(sess.run(count_var_init))


# Reuse variable, 强烈建议不要使用变量共享, 可以记录变量指针   



