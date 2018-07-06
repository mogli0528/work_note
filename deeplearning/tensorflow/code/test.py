import tensorflow as tf
import numpy as np
from sklearn import datasets
import matplotlib.pyplot as plt
sess = tf.Session()

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






