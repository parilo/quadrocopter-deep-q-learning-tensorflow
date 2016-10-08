import tensorflow as tf
import numpy as np
import random as random

#For plotting the images
from matplotlib import pyplot as plt
import matplotlib.animation as animation

from self_organizing_map import SOM

def get_rand_colors ():
    return np.array ([[random.random(),random.random(),random.random()]])

#Training inputs for RGBcolors
colors = np.array(
     [[0., 0., 0.],
      [0., 0., 1.],
      [0., 0., 0.5],
      [0.125, 0.529, 1.0],
      [0.33, 0.4, 0.67],
      [0.6, 0.5, 1.0],
      [0., 1., 0.],
      [1., 0., 0.],
      [0., 1., 1.],
      [1., 0., 1.],
      [1., 1., 0.],
      [1., 1., 1.],
      [.33, .33, .33],
      [.5, .5, .5],
      [.66, .66, .66]])
color_names = \
    ['black', 'blue', 'darkblue', 'skyblue',
     'greyblue', 'lilac', 'green', 'red',
     'cyan', 'violet', 'yellow', 'white',
     'darkgrey', 'mediumgrey', 'lightgrey']

num_of_train_iters = 1
    
#Train a 20x30 SOM with 400 iterations
som = SOM(10, 10, 3, num_of_train_iters, sigma=1.0)
iter_no = 0;
for i in range(1):
    som.train(get_rand_colors (), iter_no)
    iter_no += num_of_train_iters
    image_grid = np.array(som.get_centroids())
fig = plt.figure()
image_grid = som.get_centroids()
im = plt.imshow(image_grid, animated=True, interpolation='none')
plt_texts = []

def updatefig(*args):
    global iter_no, colors, plt_texts
    som.train(get_rand_colors (), iter_no)
    iter_no += num_of_train_iters
    image_grid = np.array(som.get_centroids())
    print (iter_no)
    np.clip(image_grid, 0, 1, out=image_grid)
    im.set_array(image_grid.tolist())
    for t in plt_texts:
        t.remove ()
    plt_texts = []
    mapped = som.map_vects(colors)
    for i, m in enumerate(mapped):
        plt_texts.append(plt.text(m[1], m[0], color_names[i], ha='center', va='center', bbox=dict(facecolor='white', alpha=0.5, lw=0)))
    return im,

ani = animation.FuncAnimation(fig, updatefig, interval=50, blit=False)

plt.title('Color SOM')
plt.show()

#for i in range(400):
#    som.train(colors)
#
#    #Get output grid
#    image_grid = som.get_centroids()
#
#    #Map colours to their closest neurons
#    mapped = som.map_vects(colors)
#
#    #Plot
#    plt.set_array(image_grid)
#    for i, m in enumerate(mapped):
#        plt.text(m[1], m[0], color_names[i], ha='center', va='center', bbox=dict(facecolor='white', alpha=0.5, lw=0))
