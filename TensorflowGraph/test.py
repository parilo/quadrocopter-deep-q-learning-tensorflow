import numpy as np
import tempfile
import tensorflow as tf

from tf_rl.controller import ContinuousDeepQ
#from tf_rl.simulation import KarpathyGame
from tf_rl import simulate
from tf_rl.lstm_model import LSTMModel
from tf_rl.lstm_mlp_model import LSTMStepped_MLP

#tf.ops.reset_default_graph()
session = tf.Session()

# This little guy will let us run tensorboard
#      tensorboard --logdir [LOG_DIR]
journalist = tf.train.SummaryWriter("/Users/anton/devel/unity/QuadrocopterHabr/TensorflowLog")

observation_size = 50;
observations_in_seq = 1;
input_size = observation_size*observations_in_seq;

# actions
num_actions = 2;

minibatch_size = 32

#layer_size, layers_count, input_size, output_size, nonlinearity

m = LSTMStepped_MLP (input_size, 128, 2, 10, [128, 1], [tf.nn.sigmoid, tf.identity], scope='test')

c = m.copy ()
