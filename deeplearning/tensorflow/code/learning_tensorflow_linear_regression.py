import tensorflow as tf
import random

### build the graph## first set up the parameters
m = tf.get_variable("m", [], initializer=tf.constant_initializer(0.))
b = tf.get_variable("b", [], initializer=tf.constant_initializer(0.))
init = tf.global_variables_initializer()

## then set up the computations
input_placeholder = tf.placeholder(tf.float32)
output_placeholder = tf.placeholder(tf.float32)

x = input_placeholder
y = output_placeholder
y_guess = m * x + b

loss = tf.square(y - y_guess)
## finally, set up the optimizer and minimization node
optimizer = tf.train.GradientDescentOptimizer(1e-3)
train_op = optimizer.minimize(loss)

### start the session
sess = tf.Session()
sess.run(init)

### perform the training loop*import* random
## set up problem
random.seed(42)
true_m = random.random()
true_b = random.random()
for update_i in range(100000):
    ## (1) get the input and output
    input_data = random.random()
    output_data = true_m * input_data + true_b

    ## (2), (3), and (4) all take place within a single call to sess.run()!
    _loss, _ = sess.run([loss, train_op], feed_dict={input_placeholder: input_data, output_placeholder: output_data})
    print (update_i, _loss)

### finally, print out the values we learned for our two variables
print ("True parameters: m=%.4f, b=%.4f" % (true_m, true_b))
print ("Learned parameters: m=%.4f, b=%.4f" % tuple(sess.run([m, b])))
