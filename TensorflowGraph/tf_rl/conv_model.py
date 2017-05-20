import tensorflow as tf
import numpy as np

from .models import MLP

from .utils import base_name2

class CNN(object):
    def __init__(self, cnn_input_size, addition_size, cnn_weights, cnn_strides, cnn_maxpooling, cnn_nonlinearities, mlp_input, mlp_hiddens, mlp_nonlinearities, scope=None, reuse=False):
        
        self.cnn_input_size = cnn_input_size
        self.cnn_input_flatten_size = cnn_input_size[0]*cnn_input_size[1]
        self.addition_size = addition_size
        self.cnn_weights = cnn_weights
        self.cnn_strides = cnn_strides
        self.cnn_maxpooling = cnn_maxpooling
        self.cnn_nonlinearities = cnn_nonlinearities
        self.mlp_input = mlp_input
        self.mlp_hiddens = mlp_hiddens
        self.mlp_nonlinearities = mlp_nonlinearities
        self.scope = scope
        
        fake_input = tf.placeholder(tf.float32, [None, self.cnn_input_flatten_size + addition_size])
        self.gen_model (fake_input, reuse=reuse)

        with tf.variable_scope(self.scope) as sc:
            
            self.model_variables = [v for v in tf.trainable_variables() if v.name.startswith(sc.name)]
            for v in self.model_variables:
                print ("CNN v: " + v.name)
        
    def gen_model(self, model_input, reuse=True):
        
        if (isinstance(model_input, list)):
            model_input = tf.concat (1, [model_input[0], model_input[1]])

        with tf.variable_scope(self.scope, reuse=reuse) as sc:
            
            cnn_input = tf.slice (model_input, [0, 0], [-1, self.cnn_input_flatten_size])
            mlp_input_addition = tf.slice (model_input, [0, self.cnn_input_flatten_size], [-1, self.addition_size])
            
            layerI = 0
            cnn_layer_input = tf.reshape (cnn_input, (-1, self.cnn_input_size[0], self.cnn_input_size[1], 1))
#            cnn_layer_input = tf.expand_dims(tf.expand_dims(cnn_input, 1), 3)
            for W, s, n, p in zip(self.cnn_weights, self.cnn_strides, self.cnn_nonlinearities, self.cnn_maxpooling):
                
                W_initializer = tf.truncated_normal_initializer ()
                W_var = tf.get_variable("cnn_layer_"+str(layerI)+"_w", W, initializer=W_initializer)
                cnn_layer_input = tf.nn.conv2d (cnn_layer_input, W_var, strides=s, padding='VALID')
                
                b_initializer = tf.constant_initializer ()
                b_var = tf.get_variable("cnn_layer_"+str(layerI)+"_b", (W[3]), initializer=b_initializer)
                cnn_layer_input = tf.nn.bias_add(cnn_layer_input, b_var)
                
                cnn_layer_input = n (cnn_layer_input)
                
                cnn_layer_input = tf.nn.max_pool(cnn_layer_input, ksize=[1, p[0], p[1], 1], strides=[1, p[0], p[1], 1], padding='SAME')
                
                layerI += 1
            
            mlp_input_from_cnn = tf.reshape(cnn_layer_input, [-1, self.mlp_input-self.addition_size])
            mlp_input_value = tf.concat (1, [mlp_input_from_cnn, mlp_input_addition])
#            print ("mlp_input_value: " + str(mlp_input_value))
            mlp = MLP ([self.mlp_input,], self.mlp_hiddens, self.mlp_nonlinearities, "mlp")
#            print ("mlp: " + str(mlp (mlp_input_value)))
            
            return mlp (mlp_input_value)
        
    def __call__(self, xs):
        return self.gen_model (xs)

    def variables(self):
        return self.model_variables

    def copy(self, scope=None):
        scope = scope or self.scope + "_copy"
        print ("copy " + scope)
        with tf.variable_scope(scope) as sc:
            for v in self.variables():
                print ("bn: " + base_name2(v) + " " + v.name)
                tf.get_variable(base_name2(v), v.get_shape(), initializer=lambda x,dtype=tf.float32, partition_info=None: v.initialized_value())
            sc.reuse_variables()
            return CNN(self.cnn_input_size, self.addition_size, self.cnn_weights, self.cnn_strides, self.cnn_maxpooling, self.cnn_nonlinearities, self.mlp_input, self.mlp_hiddens, self.mlp_nonlinearities, scope=sc, reuse=True)
