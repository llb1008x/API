#!/usr/bin/env python
# coding=utf-8

#反向传播
import tensorflow as tf
import numpy as np
BATCH_SIZE = 8
seed = 23455

#基于seed产生随机数
rng = np.random.RandomState(seed)
#随机数返回32行2列的矩阵标示32组 体积和重量作为输入数据集
X = rng.rand(32,2)
#从X这个32行2列的矩阵中取出一行，判断如果和小于1 给Y赋值1，如果和不小于1，给Y赋值0
#作为输入数据集的标签
Y = [[int(x0+x1 < 1)] for (x0,x1) in X]
print("X:\n",X)
print("Y:\n",Y)

#定义神经网络的输入，参数和输出定义前向传播过程
x = tf.placeholder(tf.float32,shape=(None,2))
y_ = tf.placeholder(tf.float32,shape=(None,1))

w1 = tf.Variable(tf.random_normal([2,3],stddev=1,seed=1))
w2 = tf.Variable(tf.random_normal([3,1],stddev=1,seed=1))



