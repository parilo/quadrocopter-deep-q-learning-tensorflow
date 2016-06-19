import numpy as np
import tempfile
import tensorflow as tf

from tf_rl.controller import DiscreteDeepQ
#from tf_rl.simulation import KarpathyGame
from tf_rl import simulate
from tf_rl.models import MLP
from tf_rl.models import SeparatedMLP

#tf.ops.reset_default_graph()
session = tf.Session()

# This little guy will let us run tensorboard
#      tensorboard --logdir [LOG_DIR]
journalist = tf.train.SummaryWriter("/Users/anton/devel/unity/QuadrocopterHabr/TensorflowLog")

observation_size = 7;
observations_in_seq = 1;
input_size = observation_size*observations_in_seq;

# actions
num_actions = 5;

#brain = MLP([input_size,], [5, 5, 5, num_actions], 
#            [tf.tanh, tf.tanh, tf.tanh, tf.identity])
#brain = MLP([input_size,], [20, 20, 20, 20, num_actions], 
#            [tf.tanh, tf.tanh, tf.tanh, tf.tanh, tf.identity])

brain = SeparatedMLP ([
        MLP([input_size,], [8, 8, 1], [tf.nn.relu, tf.nn.relu, tf.identity], scope="mlp_action1"),
        MLP([input_size,], [8, 8, 1], [tf.nn.relu, tf.nn.relu, tf.identity], scope="mlp_action2"),
        MLP([input_size,], [8, 8, 1], [tf.nn.relu, tf.nn.relu, tf.identity], scope="mlp_action3"),
        MLP([input_size,], [8, 8, 1], [tf.nn.relu, tf.nn.relu, tf.identity], scope="mlp_action4"),
        MLP([input_size,], [8, 8, 1], [tf.nn.relu, tf.nn.relu, tf.identity], scope="mlp_action5"),
        ])

# The optimizer to use. Here we use RMSProp as recommended
# by the publication
#optimizer = tf.train.RMSPropOptimizer(learning_rate= 0.0001, decay=0.9)
optimizer = tf.train.RMSPropOptimizer(learning_rate= 0.0001, decay=0.9)

# DiscreteDeepQ object
current_controller = DiscreteDeepQ(input_size, num_actions, brain, optimizer, session, discount_rate=0.95, exploration_period=5000, max_experience=10000, store_every_nth=4, train_every_nth=4, summary_writer=journalist)



init_all_vars_op = tf.initialize_variables(tf.all_variables(), name='init_all_vars_op')

session.run(tf.initialize_all_variables())

#for saving graph state, trainable variable values
for variable in tf.trainable_variables():
    tf.identity (variable, name="readVariable")
    tf.assign (variable, tf.placeholder(tf.float32, variable.get_shape(), name="variableValue"), name="resoreVariable")

tf.train.write_graph(session.graph_def, 'models/', 'graph-separated-2d.pb', as_text=False)
