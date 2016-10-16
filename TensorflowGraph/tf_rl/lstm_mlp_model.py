import tensorflow as tf
import numpy as np

from .lstm_model import LSTMModel
from .lstm_model import LSTMSteppedModel
from .models import MLP

from .utils import base_name2

class LSTM_MLP(object):
    def __init__(self, input_size, lstm_hidden_size, mlp_hiddens, nonlinearities, scope=None):
        
        self.input_size = input_size
        self.lstm_hidden_size = lstm_hidden_size
        self.mlp_hiddens = mlp_hiddens
        self.nonlinearities = nonlinearities
        self.scope = scope
        
        with tf.variable_scope(scope) as sc:
            self.lstm = LSTMModel (input_size, lstm_hidden_size, "lstm")
            
            self.mlp = MLP ([lstm_hidden_size,], mlp_hiddens, nonlinearities, "mlp")
        
        self.model_variables = self.mlp.variables () + self.lstm.variables ()
        
    def __call__(self, xs):
        with tf.variable_scope(self.scope) as sc:
            return self.mlp(self.lstm(xs))

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
            return LSTM_MLP(self.input_size, self.lstm_hidden_size, self.mlp_hiddens, self.nonlinearities, scope=sc)

        
        
#class LSTMStepped_MLP(object):
#    def __init__(self, input_size, lstm_layer_size, lstm_layer_count, lstm_steps_count, mlp_hiddens, nonlinearities, scope=None):
#        
#        self.input_size = input_size
#        self.lstm_layer_size = lstm_layer_size
#        self.lstm_layer_count = lstm_layer_count
#        self.lstm_steps_count = lstm_steps_count
#        self.mlp_hiddens = mlp_hiddens
#        self.nonlinearities = nonlinearities
#        self.scope = scope
#        
#        with tf.variable_scope(scope) as sc:
#            self.lstm = LSTMSteppedModel (input_size, lstm_layer_size, lstm_layer_count, lstm_steps_count, "lstm")
#            
#            self.mlp = MLP ([lstm_layer_size,], mlp_hiddens, nonlinearities, "mlp")
#        
#        self.model_variables = self.mlp.variables () + self.lstm.variables ()
#        
#    def __call__(self, xs):
#        with tf.variable_scope(self.scope) as sc:
#            return self.mlp(self.lstm(xs))
#
#    def variables(self):
#        return self.model_variables
#
#    def copy(self, scope=None):
#        scope = scope or self.scope + "_copy"
#        print "copy " + scope
#        with tf.variable_scope(scope) as sc:
#            for v in self.variables():
#                print "bn: " + base_name2(v) + " " + v.name
#                tf.get_variable(base_name2(v), v.get_shape(), initializer=lambda x,dtype=tf.float32: v.initialized_value())
#            sc.reuse_variables()
#            return LSTMStepped_MLP(self.input_size, self.lstm_layer_size, self.lstm_layer_count, self.lstm_steps_count, self.mlp_hiddens, self.nonlinearities, scope=sc)

#class LSTM_MLP_Cell(tf.nn.rnn_cell.LSTMCell):
#    def __init__ (self, lstm_layer_size, mlp, scope=None):
#        self.mlp = mlp
#        super(LSTM_MLP_Cell, self).__init__(lstm_layer_size, state_is_tuple=True)
#
#    def __call__ (self, inputs, states):
#        val, states = super(LSTM_MLP_Cell, self).__call__(inputs, states)
#        print "got val: " + str (val)
#        print "got mlp val: " + str (self.mlp(val))
#        
#        return self.mlp(val), states
    
class LSTMStepped_MLP(object):
    def __init__(self, input_size, lstm_layer_size, lstm_layer_count, lstm_steps_count, mlp_hiddens, nonlinearities, scope=None):
        
        self.input_size = input_size
        self.lstm_layer_size = lstm_layer_size
        self.lstm_layer_count = lstm_layer_count
        self.lstm_steps_count = lstm_steps_count
        self.mlp_hiddens = mlp_hiddens
        self.nonlinearities = nonlinearities
        self.scope = scope
        
        with tf.variable_scope(scope) as sc:
            self.mlp = MLP ([lstm_layer_size,], mlp_hiddens, nonlinearities, "mlp")
            
            self.test_cell_size = 1
            self.cell = tf.nn.rnn_cell.LSTMCell(self.test_cell_size, state_is_tuple=True)
            self.mlp_cell = MLP ([self.test_cell_size,], mlp_hiddens, nonlinearities, "mlp_cell")
            
            # multi rnn
            self.layers = []
            for i in range(lstm_layer_count):
                self.layers.append (tf.nn.rnn_cell.LSTMCell(lstm_layer_size, state_is_tuple=True))
#            self.layers.append (LSTM_MLP_Cell(lstm_layer_size, self.mlp))

            fake_input = tf.placeholder(tf.float32, [None, lstm_steps_count, input_size])
            
            output = self.calc (fake_input)
            
            self.model_variables = [v for v in tf.trainable_variables()
                    if v.name.startswith(sc.name)]
            for v in self.model_variables:
                print "LSTMSteppedModel v: " + v.name

    def calc (self, xs):
#        val, state = tf.nn.dynamic_rnn(tf.nn.rnn_cell.MultiRNNCell(self.layers, state_is_tuple=True), xs, dtype=tf.float32)
#        val = tf.transpose(val, [1, 0, 2])
#        results = []
#        for i in range(self.lstm_steps_count):
#            v = self.mlp (tf.gather(val, i))
#            results.append (v)
#        r = tf.transpose(tf.pack (results), [1, 0, 2])
#        return r
        return tf.identity(np.zeros ([1, 20, 2], dtype=np.float32))
    
    def get_last (self, xs):
#        xs = np.zeros([1, 1, 50], dtype=np.float32)
#        val, state = tf.nn.dynamic_rnn(tf.nn.rnn_cell.MultiRNNCell(self.layers, state_is_tuple=True), xs, dtype=tf.float32)
#        val = tf.transpose(val, [1, 0, 2])
#        return self.mlp(tf.gather(val, int(val.get_shape()[0]) - 1))

        xs = np.zeros([1, 10, 50], dtype=np.float32)
        val, state = tf.nn.dynamic_rnn(self.cell, xs, dtype=tf.float32)
        val = tf.transpose(val, [1, 0, 2])
        return self.mlp_cell(tf.gather(val, int(val.get_shape()[0]) - 1))

#        with tf.variable_scope("lstm") as sc:
#            state = tf.nn.rnn_cell.LSTMStateTuple (np.array(np.zeros ([1, self.test_cell_size])), np.array(np.zeros([1, self.test_cell_size])))
#            xs = tf.identity(np.zeros([1, 50], dtype=np.float32))
#            cell = self.cell
#            val, newstate = cell (xs, state)
#        with tf.variable_scope("lstm", reuse=True
#                              ) as sc:
#            xs2 = tf.identity(np.zeros([1, 50], dtype=np.float32))
#            val2, newstate2 = cell (xs2, newstate)
#            xs3 = tf.identity(np.zeros([1, 50], dtype=np.float32))
#            val3, newstate3 = cell (xs3, newstate2)
#        return self.mlp_cell(val3)

    
    def __call__(self, xs):
        with tf.variable_scope(self.scope, reuse=True) as vs:
            return self.calc (xs)

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
            return LSTMStepped_MLP(self.input_size, self.lstm_layer_size, self.lstm_layer_count, self.lstm_steps_count, self.mlp_hiddens, self.nonlinearities, scope=sc)
