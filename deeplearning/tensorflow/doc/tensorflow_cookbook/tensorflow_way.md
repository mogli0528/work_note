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


mov_avg_layer = tf.nn.conv2d(x_data, my_filter, my_strides, padding="SAME", name='Moving_AVG_WINDOW')

def custom_layer(input_matrix):
    input_matrix_squeezed = tf.squeeze(input_matrix)
    A = tf.constant([[1., 2.], [-1., 3.]])
    b = tf.constant(1., shape=[2,2])
    temp1 = tf.matmul(A, input_matrix_squeezed)
    temp = tf.add(temp1, b)


## Implementing Loss Functions 

Loss functions are very important to machine learning algorithms. They
measure the distance between the model outputs and the target (truth)
values. In this recipe, we show various loss function implementations in
TensorFlow.  

In order to optimize our machine learning algorithms, we will need to
evaluate the outcomes. Evaluating outcomes in TensorFlow depends on
specifying a loss function. A loss function tells TensorFlow how good or
bad the predictions are compared to the desired result. In most cases, we
will have a set of data and a target on which to train our algorithm. The
loss function compares the target to the prediction and gives a numerical
distance between the two.
For this recipe, we will cover the main loss functions that we can
implement in TensorFlow.
To see how the different loss functions operate, we will plot them in this
recipe. We will first start a computational graph and load matplotlib , a
python plotting library, as follows:

```python

x_vals = tf.linspace(-1.0, 1., 500)
target = tf.constant(0.)

# L2 Norm Loss: Euclidean loss function
'''
TensorFlow has a built -in form of the L2 norm, called nn.l2_loss() .
This function is actually half the L2-norm above. In other words, it is
same as previously but divided by 2.
'''
l2_y_vals = tf.square(target - x_vals)
l2_y_out = sess.run(l2_y_vals)

# L1 Norm Loss: absolute loss function
l1_y_vals = tf.abs(target - x_vals)
l1_y_out = sess.run(l1_y_vals)

# Pseudo-Huber loss: a continuous and smooth approximation to the Huber loss function   
delta1 = tf.constant(0.25)
phuber1_y_vals = tf.matmul( tf.square(delta1), tf.sqrt(1. +
tf.square((target - x_vals)/delta1))- 1.)

# Hinge Loss: mostly used for support vector machines, can be used in neural networks as well
hinge_y_vals = tf.maximum(0., 1. - tf.matmul(target, x_vals))

# Cross-entropy: Logistic loss function  
xentropy_y_vals = - tf.matmul(target, tf.log(x_vals)) - tf.matmul((1. - target), tf.log(1. - x_vals)) 

# Sigmoid cross entropy loss: transform the x-values by the sigmoid function before we put them in the cross entropy loss
xentropy_sigmoid_y_vals =                   tf.nn.sigmoid_cross_entropy_with_logits(x_vals, target)

# Weighted cross entropy loss: weighted version of the sigmoid cross entropy loss
weight = tf.constant(0.5)
xentropy_weighted_y_vals = tf.nn.weighted_cross_entropy_with_logits(x_vals, targets,
weight)

# Softmax cross-entropy loss: operates on non-normalized outputs.
unscaled_logits = tf.constant([[1., -3., 10.]])
target_dist = tf.constant([[0.1, 0.02, 0.88]])
softmax_xentropy = tf.nn.softmax_cross_entropy_with_logits(unscaled_logits, target_dist)
print(sess.run(softmax_xentropy))

# Sparse softmax cross-entropy loss: he same as previously, except instead of the target being a probability distribution, it is an index of which category is true  
unscaled_logits = tf.constant([[1., -3., 10.]])
sparse_target_dist = tf.constant([2])
sparse_xentropy = tf.nn.sparse_softmax_cross_entropy_with_logits(unscaled_logits
, sparse_target_dist)
print(sess.run(sparse_xentropy))
```


Here is a table summarizing the different loss functions that we have
described:

L2 Regression More stable Less robust
L1 Regression More robust Less stable
More robust and stable One more parameter
Psuedo-Huber RegressionHinge
Classification Creates a max margin for use in
SVM
Cross-entropy Classification More stable
Unbounded loss affected by outliers
Unbounded loss, less robust

Model metric Description
R-squared
(coefficient of
determination) For linear models, this is the proportion of variance in the dependent variable
that is explained by the independent data.
RMSE (root mean
squared error) For continuous models, measures the difference between predictions and actual
via the square root of the average squared error.
Confusion matrix For categorical models, we look at a matrix of predicted categories versus actual
categories. A perfect model has all the counts along the diagonal.
Recall For categorical models, this is the fraction of true positives over all predicted
positives.
For categorical models, this is the fraction of true positives over all actualPrecision positives.
F-score For categorical models, this is the harmonic mean of precision and recall.


## Implementing Back Propagation
One of the benefits of using TensorFlow, is that it can keep track of
operations and automatically update model variables based on back
propagation. In this recipe, we will introduce how to use this aspect to our
advantage when training machine learning models.

This is done via declaring an optimization function. Once we have an optimization function
declared, TensorFlow will go through and figure out the back propagation
terms for all of our computations in the graph. When we feed data in and
minimize the loss function, TensorFlow will modify our variables in the
graph accordingly.


x_vals = np.concatenate((np.random.normal(-1, 1, 50), np.random.normal(3, 1, 50)))
y_vals = np.concatenate((np.repeat(0., 50), np.repeat(1., 50)))
my_output_expanded = tf.expand_dims(my_output, 0)
y_target_expanded = tf.expand_dims(y_target, 0)


## Working with Batch and Stochastic Training   

While TensorFlow updates our model variables according to the prior
described back propagation, it can operate on anywhere from one datum
observation to a large group of data at once. Operating on one training
example can make for a very erratic(不稳定的) learning process, while using a too
large batch can be computationally expensive. Choosing the right type of
training is crucial to getting our machine learning algorithms to converge to
a solution.   

### Getting ready

In order for TensorFlow to compute the variable gradients for back
propagation to work, we have to measure the loss on a sample or multiple
samples. Stochastic training(随机训练) is only putting through one randomly sampled
data-target pair at a time, just like we did in the previous recipe. Another
option is to put a larger portion of the training examples in at a time and
average the loss for the gradient calculation. Batch training size can vary
up to and including the whole dataset at once. Here we will show how to
extend the prior regression example, which used stochastic training to
batch training.


Our loss function will change because we have to take the mean of all
the L2 losses of each data point in the batch. We do this by wrapping
our prior loss output in TensorFlow's reduce_mean() function:  

```python
loss = tf.reduce_mean(tf.square(my_output - y_target))
```

可以看到 batch training 的 loss 下降很平稳, 没有太大的震荡.    

Type of training Advantages Disadvantages
Stochastic Randomness may help move out of local minimums. Generally, needs more iterations to converge.
Batch  Finds minimums quicker. Takes more resources to compute.