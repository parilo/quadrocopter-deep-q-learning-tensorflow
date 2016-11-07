import numpy as np
import tempfile
import tensorflow as tf

from tf_rl.controller import ContinuousDeepQLSTMStepped
from tf_rl import simulate
from tf_rl.lstm_mlp_model import MLP_LSTM

session = tf.Session()

# This little guy will let us run tensorboard
#      tensorboard --logdir [LOG_DIR]
journalist = tf.train.SummaryWriter("/tmp")

observation_size = 50;
observations_in_seq = 1;
input_size = observation_size*observations_in_seq;
# actions
num_actions = 2;
critic_input_size = input_size + num_actions*2
steps_count = 4

minibatch_size = 32

#layer_size, layers_count, input_size, output_size, nonlinearity

critic = MLP_LSTM (critic_input_size, 1, 256, 1, steps_count, [512, 512], [tf.nn.tanh, tf.nn.tanh], scope='critic')
actor = MLP_LSTM (input_size, num_actions, 256, 1, steps_count, [512, 512], [tf.nn.tanh, tf.nn.tanh], scope='actor')

# The optimizer to use. Here we use RMSProp as recommended
# by the publication
#optimizer = tf.train.RMSPropOptimizer(learning_rate= 0.0001, decay=0.9)
#optimizer = tf.train.RMSPropOptimizer(learning_rate= 0.0005, decay=0.9)
optimizer = tf.train.AdamOptimizer(learning_rate= 0.0001)
#optimizer = tf.train.GradientDescentOptimizer(learning_rate= 0.001)

state = tf.placeholder(tf.float32, [None, steps_count, input_size], name="observation")
next_state = tf.placeholder(tf.float32, [None, steps_count, input_size], name="next_observation")
action = tf.placeholder(tf.float32, [None, steps_count, num_actions], name="given_action")

# DiscreteDeepQ object
current_controller = ContinuousDeepQLSTMStepped(state, next_state, action, input_size, num_actions, actor, critic, optimizer, session, discount_rate=0.98, target_actor_update_rate=0.01, target_critic_update_rate=0.01, exploration_period=5000, max_experience=10000, store_every_nth=4, train_every_nth=4, summary_writer=journalist)

#class ContinuousDeepQ
#                       observation_size,
#                       action_size,
#                       actor,
#                       critic,
#                       optimizer,
#                       session,
#                       exploration_sigma=0.05,
#                       exploration_period=1000,
#                       store_every_nth=5,
#                       train_every_nth=5,
#                       minibatch_size=32,
#                       discount_rate=0.95,
#                       max_experience=30000,
#                       target_actor_update_rate=0.01,
#                       target_critic_update_rate=0.01,
#                       summary_writer=None



init_all_vars_op = tf.initialize_variables(tf.all_variables(), name='init_all_vars_op')

session.run(tf.initialize_all_variables())

#for saving graph state, trainable variable values
for variable in tf.trainable_variables():
    tf.identity (variable, name="readVariable")
    tf.assign (variable, tf.placeholder(tf.float32, variable.get_shape(), name="variableValue"), name="resoreVariable")

tf.train.write_graph(session.graph_def, 'models/', 'graph-2d-ddpg-lstm-stepped-mlp.pb', as_text=False)
