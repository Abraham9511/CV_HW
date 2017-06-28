#coding=utf-8

import tensorflow as tf
from tensorflow.examples.tutorials.mnist import input_data

def weight_variable(shape):
	initial = tf.truncated_normal(shape, stddev=0.1)
	return tf.Variable(initial)

def bias_variable(shape):
	initial = tf.constant(0.1, shape=shape)
	return tf.Variable(initial)

def conv2d(x, W):
	return tf.nn.conv2d(x, W, strides=[1,1,1,1], padding='SAME')

def max_pool_2x2(x):
	return tf.nn.max_pool(x, ksize=[1,2,2,1], 
		strides = [1,2,2,1], padding='SAME')

def deepnn(x): 
	x_image = tf.reshape(x, [-1, 28, 28, 1])

	# 第一层卷积网络，将灰度图映射到32个特征
	W_conv1 = weight_variable([5,5,1,32])
	b_conv1 = bias_variable([32])
	h_conv1 - tf.nn.relu(conv2d(x_image, w_conv1) + b_conv1)
	
	# 第一次max_pool降到14x14
	h_pool1 = max_pool_2x2(h_conv1)

	# 第二层卷积神经网络，将32特征提升到64特征
	W_conv2 = weight_variable([5,5,32,64])
	b_conv2 = bias_variable([64])
	h_conv2 = tf.nn.relu(conv2d(h_pool1, W_conv2) + b_conv2)
	
	# 第二次max_pool到7x7
	h_pool2 = max_pool_2x2(h_conv2)

	# 第一层全连接层，将7*7*64映射到 1024个特征
	W_fc1 = weight_variable([7*7*64, 1024])
	b_fc1 = bias_variable([1024])

	h_pool2_flat = tf.reshape(h_pool2, [-1, 7*7*64])
	h_fc1 = tf.nn.relu(tf.matmul(h_pool2_flat, W_fc1)+b_fc1)

	# Dropout使用，避免过拟合，测试的时候关闭
	keep_prob = tf.placeholder(tf.float32)
	h_fc1_drop = tf.nn.dropout(h_fc1, keep_prob)

	# 将1024映射都10类
	W_fc2 = weight_variable([1024, 10])
	b_fc2 = bias_variable([10])

	y_conv = tf.matmul(h_fc1_drop, W_fc2) + b_fc2

	return y_conv, keep_prob
	

def main(_):
	## ？？ one_hot是什么意思
	# 获取minist训练和测试数据
	mnist = input_data.read_data_sets('MNIST_data', one_hot=True)

	# None指的是batch_size，可以是任意大小
	x = tf.placeholder(tf.float32, shape=[None, 784])

	# loss 和 optimizer
	y_ = tf.placeholder(tf.float32, shape=[None, 10])

	# 构建深度卷积网络模型
	y_conv, keep_prob = deepnn(x)

	cross_entropy = tf.reduce_mean(
		tf.nn.softmax_cross_entropy_with_logits(labels = y_, logits = y_conv))
	train_step = tf.train.AdamOptimizer(1e-4).minimize(cross_entropy)
	correct_prediction = tf.equal(tf.argmax(y_conv, 1), tf.argmax(y_, 1))
	accuracy = tf.reduce_mean(tf.cast(correct_prediction, tf.float32))

	with tf.Session() as sess:
		sess.run(tf.global_variables_initializer())
		for i in range(20000):
			batch = mnist.train.next_batch(50)
			if i % 100 == 0:
				train_accuracy = accuracy.eval(feed_dict={
					x: batch[0], y_:batch[1], keep_prob: 1.0
					})
				print('step %d, traning accuracy %g' % (i, train_accuracy))
			train_step.run(feed_dict={x:batch[0], y_:batch[1], keep_prob:0.5})
		
		print('test accuracy %g' % accuracy.eval(feed_dict={
			x: mnist.test.images, y_: minist.test.labels, keep_prob: 1.0
			}))


if __name__ == '__main__':
	tf.app.run(main=main)












