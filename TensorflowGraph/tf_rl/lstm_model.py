import tensorflow as tf

from .utils import base_name2

class LSTMModel(object):
    def __init__(self, input_size, layer_size, scope=None, needReuseVariables=True):

        self.layer_size = layer_size
        self.input_size = input_size
        self.scope = scope or "LSTM"
        
        with tf.variable_scope(self.scope) as vs:

            self.cell = tf.nn.rnn_cell.LSTMCell(self.layer_size, state_is_tuple=True)

            self.states  = tf.nn.rnn_cell.LSTMStateTuple (tf.placeholder(tf.float32, (None, self.layer_size), name="state_c"), tf.placeholder(tf.float32, (None, self.layer_size), name="state_h"))
            print "placeholder state c: " + str(self.states [0])
            print "placeholder state h: " + str(self.states [1])

            xs  = tf.placeholder(tf.float32, (None, input_size), name="fake_xs")
            val, states = self.cell (xs, self.states)
#            if (needReuseVariables):
#                vs.reuse_variables()
            print "LSTMModel vs: ->" + vs.name + "<- reuse: " + str(vs.reuse)
            
            self.model_variables = [v for v in tf.trainable_variables()
                    if v.name.startswith(vs.name)]
            for v in self.model_variables:
                print "LSTMModel v: " + v.name

    def __call__(self, xs, states=None):
        with tf.variable_scope(self.scope, reuse=True) as vs:
            input_states = states or self.states
            val, states = self.cell (xs, self.states)
            self.out_state_c = tf.identity (states[0], name="read_state_c")
            self.out_state_h = tf.identity (states[1], name="read_state_h")
            print "read_state_c: " + str (self.out_state_c)
            print "read_state_h: " + str (self.out_state_h)
        return val
    
    def get_output_state (self):
        return self.out_state_c, self.out_state_h

    def variables(self):
        return self.model_variables

    def copy(self, scope=None):
        scope = scope or self.scope + "_copy"
        print "copy " + scope
        with tf.variable_scope(scope) as sc:
            for v in self.variables():
                print "LSTMModel bn: " + base_name2(v) + " " + v.name
                tf.get_variable(base_name2(v), v.get_shape(), initializer=lambda x,dtype=tf.float32: v.initialized_value())
            sc.reuse_variables()
        return LSTMModel(self.input_size, self.layer_size, scope=sc, needReuseVariables=False)
    
    

class LSTMSteppedModel(object):
    def __init__(self, input_size, layer_size, layer_count, steps_count, scope=None, needReuseVariables=True):

        self.input_size = input_size
        self.layer_size = layer_size
        self.layer_count = layer_count
        self.steps_count = steps_count
        self.scope = scope or "LSTM"
        
        with tf.variable_scope(self.scope) as vs:
            
            # multi rnn
            self.layers = []
            for i in range(layer_count):
                self.layers.append (tf.nn.rnn_cell.LSTMCell(layer_size, state_is_tuple=True))

            fake_input = tf.placeholder(tf.float32, [None, steps_count, input_size])
            
            output = self.calc (fake_input)
            
            self.model_variables = [v for v in tf.trainable_variables()
                    if v.name.startswith(vs.name)]
            for v in self.model_variables:
                print "LSTMSteppedModel v: " + v.name
                
    def calc (self, xs):
        val, state = tf.nn.dynamic_rnn(tf.nn.rnn_cell.MultiRNNCell(self.layers, state_is_tuple=True), xs, dtype=tf.float32)
        
        return val
    
#        val = tf.transpose(val, [1, 0, 2])
#        return tf.gather(val, int(val.get_shape()[0]) - 1)

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
                print "LSTMSteppedModel bn: " + base_name2(v) + " " + v.name
                tf.get_variable(base_name2(v), v.get_shape(), initializer=lambda x,dtype=tf.float32: v.initialized_value())
            sc.reuse_variables()
        return LSTMSteppedModel(self.input_size, self.layer_size, self.layer_count, self.steps_count, scope=sc, needReuseVariables=False)
