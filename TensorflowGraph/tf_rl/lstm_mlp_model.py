import tensorflow as tf

from .lstm_model import LSTMModel
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
