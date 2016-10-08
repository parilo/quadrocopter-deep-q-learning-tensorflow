import tensorflow as tf

from .utils import base_name2

class LSTMMultiModel(object):
    def __init__(self, layer_size, layers_count, input_size, output_size, nonlinearity, scope=None):

        self.layer_size = layer_size
        self.layers_count = layers_count
        self.input_size = input_size
        self.output_size = output_size
        self.nonlinearity = nonlinearity
        self.scope = scope or "LSTM"

        #creating initial states for agents
        
        
        with tf.variable_scope(self.scope) as vs:

            self.cell = tf.nn.rnn_cell.LSTMCell(self.layer_size, state_is_tuple=True)
            self.cell2 = tf.nn.rnn_cell.LSTMCell(self.layer_size, state_is_tuple=True)
#            self.multi_cell = tf.nn.rnn_cell.MultiRNNCell([cell] * self.layers_count, state_is_tuple=True)
            
            self.states  = tf.nn.rnn_cell.LSTMStateTuple (tf.placeholder(tf.float32, (None, self.layer_size), name="state_c"), tf.placeholder(tf.float32, (None, self.layer_size), name="state_h"))
            xs  = tf.placeholder(tf.float32, (None, input_size), name="xs")
            xs_prev  = tf.placeholder(tf.float32, (None, input_size), name="xs_prev")
            
            val, state = self.cell  (xs, self.states)
            val, state = self.cell2 (xs_prev, state)

            w = tf.truncated_normal([self.layer_size, self.output_size]);
            self.weight = tf.get_variable ("weight", w.get_shape(), initializer=lambda x,dtype=tf.float32: w)

            b = tf.constant(0.1, shape=[self.output_size])
            self.bias = tf.get_variable ("bias", b.get_shape(), initializer=lambda x,dtype=tf.float32: b)
            
            self.model_variables = [v for v in tf.trainable_variables()
                    if v.name.startswith(vs.name)]
            for v in self.model_variables:
                print "v: " + v.name
    
    def get_last (self, cell_val):
        val = tf.transpose(val, [1, 0, 2])
        last = tf.gather(val, int(val.get_shape()[0]) - 1)

    def __call__(self, xs):
#        val, state = self.multi_cell(xs, self.states)
#        last = self.get_last ()
        return None #self.nonlinearity(tf.matmul(last, weight) + bias)

    def variables(self):
        return self.model_variables

    def copy(self, scope=None):
        scope = scope or self.scope + "_copy"
        print "copy " + scope
        with tf.variable_scope(scope) as sc:
            for v in self.variables():
                print "bn: " + base_name2(v) + " " + v.name
                tf.get_variable(base_name2(v), v.get_shape(), initializer=lambda x,dtype=tf.float32: v.initialized_value())
            sc.reuse_variables()
            return LSTMModel(self.layer_size, self.layers_count, self.input_size, self.output_size, self.nonlinearity, scope=sc)


        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
#class LSTMModel(object):
#    def __init__(self, time_size, layer_size, layers_count, input_size, output_size, minibatch_size, nonlinearity, scope=None):
#
##        self.input_data = tf.constant(0, dtype=tf.float32, shape=[minibatch_size, time_size, output_size])
##target = tf.placeholder(tf.float32, [None, 18])
#
#        self.time_size = time_size
#        self.layer_size = layer_size
#        self.layers_count = layers_count
#        self.input_size = input_size
#        self.output_size = output_size
#        self.minibatch_size = minibatch_size
#        self.nonlinearity = nonlinearity
#        self.scope = scope or "LSTM"
#
#        with tf.variable_scope(self.scope) as vs:
#            self.fake_input_data = tf.get_variable("fake_lstm_input", (minibatch_size, time_size, input_size), dtype=tf.float32, trainable=False)
#
#        self.get_lstm (self.fake_input_data)
#
#        with tf.variable_scope(self.scope) as vs:
#            vs.reuse_variables()
#            self.model_variables = [v for v in tf.trainable_variables()
#                    if v.name.startswith(vs.name)]
#            for v in self.model_variables:
#                print "v: " + v.name
#
#    def get_lstm(self, input_data):
#        with tf.variable_scope(self.scope) as vs:
#
#            cell = tf.nn.rnn_cell.LSTMCell(self.layer_size, state_is_tuple=True)
#            multi_cell = tf.nn.rnn_cell.MultiRNNCell([cell] * self.layers_count, state_is_tuple=True)
#            val, state = tf.nn.dynamic_rnn(multi_cell, input_data, dtype=tf.float32)
#
#            val = tf.transpose(val, [1, 0, 2])
#            last = tf.gather(val, int(val.get_shape()[0]) - 1)
#
#            w = tf.truncated_normal([self.layer_size, self.output_size]);
#            weight = tf.get_variable ("weight", w.get_shape(), initializer=lambda x,dtype=tf.float32: w)
##            weight = tf.Variable(tf.truncated_normal([self.layer_size, self.output_size]), name="weight")
#            b = tf.constant(0.1, shape=[self.output_size])
#            bias = tf.get_variable ("bias", b.get_shape(), initializer=lambda x,dtype=tf.float32: b)
##            bias = tf.Variable(tf.constant(0.1, shape=[self.output_size]), name="bias")
#
#            
#            return (last, weight, bias)
#
#    def __call__(self, xs):
#        last, weight, bias = self.get_lstm (xs)
#        return self.nonlinearity(tf.matmul(last, weight) + bias)
#
#    def variables(self):
#        return self.model_variables
#
#    def copy(self, scope=None):
#        scope = scope or self.scope + "_copy"
#        print "copy " + scope
#        with tf.variable_scope(scope) as sc:
#            fake_input_data = tf.get_variable("fake_lstm_input", (self.minibatch_size, self.time_size, self.input_size), dtype=tf.float32, trainable=False)
#            for v in self.variables():
#                print "bn: " + base_name2(v) + " " + v.name
#                tf.get_variable(base_name2(v), v.get_shape(), initializer=lambda x,dtype=tf.float32: v.initialized_value())
#            sc.reuse_variables()
#            return LSTMModel(self.time_size, self.layer_size, self.layers_count, self.input_size, self.output_size, self.minibatch_size, self.nonlinearity, scope=sc)
