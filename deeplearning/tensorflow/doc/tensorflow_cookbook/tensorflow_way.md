# Chapter 2. The TensorFlow Way

In this chapter, we will introduce the key components of how TensorFlow
operates. Then we will tie it together to create a simple classifier and
evaluate the outcomes. By the end of the chapter you should have learned
about the following:

- Operations in a Computational Graph  
- Layering Nested Operations  
- Working with Multiple Layers  
- Implementing Loss Functions  
- Implementing Back Propagation  
- Working with Batch and Stochastic Training  
- Combining Everything  



my_product = tf.multiply(x_data, m_const)
prod1 = tf.matmul(x_data, m1)
prod2 = tf.matmul(prod1, m2)
add1 = tf.add(prod2, a1)

# 对于提前不知道数据 shape 的   
x_data = tf.placeholder(tf.float32, shape=(3,None))

This allows us to break matrix multiplication rules and we must still obey
the fact that the multiplying constant must have the same corresponding
number of rows. We can either generate this dynamically or reshape the
x_data as we feed data in our graph.   


## Working with Multiple Layers
Now that we have covered multiple operations, we will cover how to
connect various layers that have data propagating through them.   

