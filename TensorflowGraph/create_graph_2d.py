import numpy as np
import tempfile
import tensorflow as tf
import os

from tf_rl.controller import DiscreteDeepQ
#from tf_rl.simulation import KarpathyGame
from tf_rl import simulate
from tf_rl.models import MLP

#tf.ops.reset_default_graph()
session = tf.Session()

# This little guy will let us run tensorboard
#      tensorboard --logdir [LOG_DIR]
journalist = tf.train.SummaryWriter("/Users/anton/devel/unity/QuadrocopterHabr/TensorflowLog")

# Brain maps from observation to Q values for different actions.
# Here it is a done using a multi layer perceptron with 2 hidden
# layers

# observation
# double currentRotW,
# double currentRotX,
# double currentRotY,
# double currentRotZ,
#
# double targetX,
# double targetY,
# double targetZ,
#
# double motor1powerVal,
# double motor2powerVal,
# double motor3powerVal,
# double motor4powerVal
observation_size = 8+32;
observations_in_seq = 1;
input_size = observation_size*observations_in_seq;

# actions
num_actions = 5;

#brain = MLP([input_size,], [5, 5, 5, num_actions], 
#            [tf.tanh, tf.tanh, tf.tanh, tf.identity])
#brain = MLP([input_size,], [20, 20, 20, 20, num_actions], 
#            [tf.tanh, tf.tanh, tf.tanh, tf.tanh, tf.identity])

#brain = MLP([input_size,], [64, 64, 64, 64, 64, 64, num_actions], 
#            [tf.nn.relu, tf.nn.relu, tf.nn.relu, tf.nn.relu, tf.nn.relu, tf.nn.relu, tf.identity])
brain = MLP([input_size,], [64, 64, num_actions], 
            [tf.nn.sigmoid, tf.nn.sigmoid, tf.identity])

# The optimizer to use. Here we use RMSProp as recommended
# by the publication
optimizer = tf.train.RMSPropOptimizer(learning_rate= 0.0001, decay=0.9)
#optimizer = tf.train.FtrlOptimizer(learning_rate= 0.0001)
#optimizer = tf.train.AdamOptimizer(learning_rate= 0.0001)
#optimizer = tf.train.MomentumOptimizer(learning_rate= 0.0001, momentum=0.1)
#optimizer = tf.train.AdagradOptimizer(learning_rate= 0.0001)
#optimizer = tf.train.AdadeltaOptimizer(learning_rate= 0.0001)
#optimizer = tf.train.GradientDescentOptimizer(learning_rate= 0.0001)

# DiscreteDeepQ object
current_controller = DiscreteDeepQ(input_size, num_actions, brain, optimizer, session, discount_rate=0.9, target_network_update_rate=0.002, exploration_period=5000, max_experience=10000, store_every_nth=4, train_every_nth=4, summary_writer=journalist)



#for model based learning
#state + action
model_input_size = input_size + num_actions
#state + reward
model_output_size = input_size + 1
model_mlp = MLP([model_input_size,], [128, model_output_size], 
            [tf.tanh, tf.identity], scope="env_model_mlp")

model_input = tf.placeholder(tf.float32, [None, model_input_size], name="env_model_input")
model_prediction = tf.identity( model_mlp (model_input), name="env_model_prediction")

model_train_data = tf.placeholder(tf.float32, [None, model_output_size], name="env_model_train_data")

model_sqerror = tf.reduce_mean(tf.square(model_prediction - model_train_data), name="env_model_prediction_error")

model_optimizer = tf.train.RMSPropOptimizer(learning_rate= 0.0001, decay=0.9)
model_gradients = model_optimizer.compute_gradients(model_sqerror)
model_train_step = model_optimizer.apply_gradients(model_gradients, name="env_model_train_step")



init_all_vars_op = tf.initialize_variables(tf.all_variables(), name='init_all_vars_op')

session.run(tf.initialize_all_variables())

#for saving graph state, trainable variable values
for variable in tf.trainable_variables():
    tf.identity (variable, name="readVariable")
    tf.assign (variable, tf.placeholder(tf.float32, variable.get_shape(), name="variableValue"), name="resoreVariable")

scriptdir = os.getcwd()
tf.train.write_graph(session.graph_def, scriptdir + '/models/', 'graph2d.pb', as_text=False)
