# -*- coding: utf-8 -*-

# DQN taken from
# https://github.com/nivwusquorum/tensorflow-deepq
# thank you Szymon Sidor ;)

import numpy as np
import random
import tensorflow as tf

from collections import deque

class DiscreteDeepQ(object):
    # Описание параметров ниже
    def __init__(self, observation_size,
                       num_actions,
                       observation_to_actions,
                       optimizer,
                       session,
                       random_action_probability=0.05,
                       exploration_period=1000,
                       store_every_nth=5,
                       train_every_nth=5,
                       minibatch_size=32,
                       discount_rate=0.95,
                       max_experience=30000,
                       target_network_update_rate=0.01,
                       summary_writer=None):
        # Этот большой комментарий я просто переведу ниже
        """Initialized the Deepq object.

        Based on:
            https://www.cs.toronto.edu/~vmnih/docs/dqn.pdf

        Parameters
        -------
        observation_size : int
            length of the vector passed as observation
        num_actions : int
            number of actions that the model can execute
        observation_to_actions: dali model
            model that implements activate function
            that can take in observation vector or a batch
            and returns scores (of unbounded values) for each
            action for each observation.
            input shape:  [batch_size, observation_size]
            output shape: [batch_size, num_actions]
        optimizer: tf.solver.*
            optimizer for prediction error
        session: tf.Session
            session on which to execute the computation
        random_action_probability: float (0 to 1)
        exploration_period: int
            probability of choosing a random
            action (epsilon form paper) annealed linearly
            from 1 to random_action_probability over
            exploration_period
        store_every_nth: int
            to further decorrelate samples do not all
            transitions, but rather every nth transition.
            For example if store_every_nth is 5, then
            only 20% of all the transitions is stored.
        train_every_nth: int
            normally training_step is invoked every
            time action is executed. Depending on the
            setup that might be too often. When this
            variable is set set to n, then only every
            n-th time training_step is called will
            the training procedure actually be executed.
        minibatch_size: int
            number of state,action,reward,newstate
            tuples considered during experience reply
        dicount_rate: float (0 to 1)
            how much we care about future rewards.
        max_experience: int
            maximum size of the reply buffer
        target_network_update_rate: float
            how much to update target network after each
            iteration. Let's call target_network_update_rate
            alpha, target network T, and network N. Every
            time N gets updated we execute:
                T = (1-alpha)*T + alpha*N
        summary_writer: tf.train.SummaryWriter
            writer to log metrics
        """

        """Инициализация Deepq

        Основано на:
            https://www.cs.toronto.edu/~vmnih/docs/dqn.pdf

        Параметры
        -------
        observation_size : int
            длина вектора входных данных (этот вектор
            будем называть наблюдением или состоянием)
            
        num_actions : int
            количество возможных действий или же
            длина вектора выходных данных нейросети
            
        observation_to_actions: dali model
            модель (в нашем случае нейросеть),
            которая принимает наблюдение или набор наблюдений
            и возвращает оценку очками каждого действия или
            набор оценок для каждого действия каждого из наблюдений
            входной размер: матрица [batch_size, observation_size]
            выходной размер: матрица [batch_size, num_actions]
            
        optimizer: tf.solver.*
            алгоритм рассчета обратого распространения ошибки
            в нашем случае будет использоваться RMSProp
            
        session: tf.Session
            сессия TensorFlow в которой будут производится вычисления
            
        random_action_probability: float (0 to 1)
            вероятность случайного действия,
            для обогощения опыта нейросети и улучшения качесва управления
            с определенной вероятностью выполняется случайное действие, а не
            действие выданное нейросетью
            
        exploration_period: int
            период поискового поведения в итерациях,
            в течении которого вероятность выполнения случайного
            действия падает от 1 до random_action_probability
            
        store_every_nth: int
            параметр нужен чтобы сохранять не все обучающие примеры
            а только определенную часть из них.
            Сохранение происходит один раз в указаное в параметре
            количество обучающих примеров
            
        train_every_nth: int
            обычно training_step (шаг обучения)
            запускается после каждого действия.
            Иногда получается так, что это слишком часто.
            Эта переменная указывает сколько шагов
            пропустить перед тем как запускать шаг обучения
            
        minibatch_size: int
            размер набора обучающих примеров который
            используется на одном шаге обучения
            алгоритмом RMSProp.
            Обучающий пример включает в себя
            состояние, предпринятое действие, награду и
            новое состояние
            
        dicount_rate: float (0 to 1)
            параметр Q-learning
            насколько сильно влияет будущая награда при
            расчете пользы действия
            
        max_experience: int
            максимальное количество сохраненных
            обучающих примеров
            
        target_network_update_rate: float
            параметр скорости обучения нейросети,
            здесь используется 2 нейросети
            T - target_q_network
            она используется для расчета вклада будущей пользы и
            N - q_network
            она испольщуется для выбора действия,
            также эта сеть подвергается обучению
            методом обратного распространения ошибки.
            Сеть T с определенной скоростью стремится к сети N.
            Каждый раз при обучении N,
            Т модифицируется следующим образом:
                alpha = target_network_update_rate
                T = (1-alpha)*T + alpha*N
                
        summary_writer: tf.train.SummaryWriter
            запись логов
        """
        
        
        # memorize arguments
        self.observation_size          = observation_size
        self.num_actions               = num_actions

        self.q_network                 = observation_to_actions
        self.optimizer                 = optimizer
        self.s                         = session

        self.random_action_probability = random_action_probability
        self.exploration_period        = exploration_period
        self.store_every_nth           = store_every_nth
        self.train_every_nth           = train_every_nth
        self.minibatch_size            = minibatch_size
        self.discount_rate             = tf.constant(discount_rate)
        self.max_experience            = max_experience
        self.target_network_update_rate = \
                tf.constant(target_network_update_rate)

        # deepq state
        self.actions_executed_so_far = 0
        self.experience = deque()

        self.iteration = 0
        self.summary_writer = summary_writer

        self.number_of_times_store_called = 0
        self.number_of_times_train_called = 0

        self.create_variables()

    # расчет вероятности случайного действия
    # с учетом уменьшения с итерациями
    # (линейный отжиг)
    def linear_annealing(self, n, total, p_initial, p_final):
        """Linear annealing between p_initial and p_final
        over total steps - computes value at step n"""
        if n >= total:
            return p_final
        else:
            return p_initial - (n * (p_initial - p_final)) / (total)

    # создание графов TensorFlow для
    # для расчета управляющего действия
    # и реализации Q-learning
    def create_variables(self):
        # создание нейросети T копированием из исходной нейросети N
        self.target_q_network    = self.q_network.copy(scope="target_network")

        # расчет управляющего действия
        # FOR REGULAR ACTION SCORE COMPUTATION
        with tf.name_scope("taking_action"):
            # входные данные вектора состояния
            self.observation        = tf.placeholder(tf.float32, (None, self.observation_size), name="observation")
            # расчитать очки оценки полезности каждого действия
            self.action_scores      = tf.identity(self.q_network(self.observation), name="action_scores")
            tf.histogram_summary("action_scores", self.action_scores)
            # взять действие с максимальным количеством очков
            self.predicted_actions  = tf.argmax(self.action_scores, dimension=1, name="predicted_actions")

        # расчет будущей пользы
        with tf.name_scope("estimating_future_rewards"):
            # FOR PREDICTING TARGET FUTURE REWARDS
            # входной параметр - будущие состояния
            self.next_observation          = tf.placeholder(tf.float32, (None, self.observation_size), name="next_observation")
            # входной параметр - маски будущих состояний
            self.next_observation_mask     = tf.placeholder(tf.float32, (None,), name="next_observation_mask")
            # оценки полезности
            self.next_action_scores        = tf.stop_gradient(self.target_q_network(self.next_observation))
            tf.histogram_summary("target_action_scores", self.next_action_scores)
            # входной параметр - награды
            self.rewards                   = tf.placeholder(tf.float32, (None,), name="rewards")
            # взять максимальные оценки полезностей действий
            target_values                  = tf.identity(tf.reduce_max(self.next_action_scores, reduction_indices=[1,]) * self.next_observation_mask, name="target_values")
            # r + DF * MAX(Q,s) см статью о Q-learning в википедии
            #self.future_rewards            = self.rewards + self.discount_rate * target_values
            self.future_rewards            = tf.identity(self.rewards + self.discount_rate * target_values, name="future_rewards")

        # обученте сети N 
        with tf.name_scope("q_value_precition"):
            # FOR PREDICTION ERROR
            # входной параметр маски действий в наборе обучающих примеров
            self.action_mask                = tf.placeholder(tf.float32, (None, self.num_actions), name="action_mask")
            # расчет полезностей действий набора обучающих примеров
            self.masked_action_scores       = tf.reduce_sum(self.action_scores * self.action_mask, reduction_indices=[1,], name="masked_action_scores")
            # разности текущих полезностей и будущих
            # - (r + DF * MAX(Q,s) — Q[s',a'])
            #temp_diff                       = self.masked_action_scores - self.future_rewards
            temp_diff                       = tf.identity(self.masked_action_scores - self.future_rewards, name="temp_diff")
            # ключевой момент обучения сети
            # RMSProp минимизирует среднее от вышеуказанных разностей
            self.prediction_error           = tf.reduce_mean(tf.square(temp_diff), name="prediction_error")
            # работа RMSProp, первый шаг - вычисление градиентов
            gradients                       = self.optimizer.compute_gradients(self.prediction_error)
            #def get_zero(): return tf.constant(0.0)
            #def get_perror(): return self.prediction_error
            #gradients                       = self.optimizer.compute_gradients(tf.cond(tf.is_nan(self.prediction_error), get_zero, get_perror))
            for i, (grad, var) in enumerate(gradients):
                if grad is not None:
                    gradients[i] = (tf.clip_by_norm(grad, 5), var)
            # Add histograms for gradients.
            for grad, var in gradients:
                tf.histogram_summary(var.name, var)
                if grad is not None:
                    tf.histogram_summary(var.name + '/gradients', grad)
            # второй шаг - оптимизация параметров нейросети
            self.train_op                   = self.optimizer.apply_gradients(gradients, name="train_op")

        # то самое место где настраивается сеть T
        # T = (1-alpha)*T + alpha*N
        # UPDATE TARGET NETWORK
        with tf.name_scope("target_network_update"):
            self.target_network_update = []
            for v_source, v_target in zip(self.q_network.variables(), self.target_q_network.variables()):
                # this is equivalent to target = (1-alpha) * target + alpha * source
                update_op = v_target.assign_sub(self.target_network_update_rate * (v_target - v_source))
                self.target_network_update.append(update_op)
            self.target_network_update = tf.group(*self.target_network_update, name="target_network_update")

        # summaries 
        tf.scalar_summary("prediction_error", self.prediction_error)

        self.summarize = tf.merge_all_summaries()
        self.no_op1    = tf.no_op()

    # управление
    def action(self, observation):
        """Given observation returns the action that should be chosen using
        DeepQ learning strategy. Does not backprop."""
        assert len(observation.shape) == 1, \
                "Action is performed based on single observation."

        self.actions_executed_so_far += 1
        # расчет вероятности случайного действия
        exploration_p = self.linear_annealing(self.actions_executed_so_far,
                                              self.exploration_period,
                                              1.0,
                                              self.random_action_probability)

        if random.random() < exploration_p:
            # случайное действие
            return random.randint(0, self.num_actions - 1)
        else:
            # действие выбранное нейросетью
            return self.s.run(self.predicted_actions, {self.observation: observation[np.newaxis,:]})[0]

    # сохранение обучающего примера
    # обучающий примеры берутся из действий нейросети
    # во время управления
    def store(self, observation, action, reward, newobservation):
        """Store experience, where starting with observation and
        execution action, we arrived at the newobservation and got thetarget_network_update
        reward reward

        If newstate is None, the state/action pair is assumed to be terminal
        """
        if self.number_of_times_store_called % self.store_every_nth == 0:
            self.experience.append((observation, action, reward, newobservation))
            if len(self.experience) > self.max_experience:
                self.experience.popleft()
        self.number_of_times_store_called += 1

    # шаг обучения
    def training_step(self):
        """Pick a self.minibatch_size exeperiences from reply buffer
        and backpropage the value function.
        """
        if self.number_of_times_train_called % self.train_every_nth == 0:
            if len(self.experience) <  self.minibatch_size:
                return

            # из всего сохраненного опыта случайно выбираем
            # пачку из minibatch_size обучающих примеров
            # sample experience.
            samples   = random.sample(range(len(self.experience)), self.minibatch_size)
            samples   = [self.experience[i] for i in samples]

            # представляем обучающие примеры
            # в нужном виде
            # bach states
            states         = np.empty((len(samples), self.observation_size))
            newstates      = np.empty((len(samples), self.observation_size))
            action_mask    = np.zeros((len(samples), self.num_actions))

            newstates_mask = np.empty((len(samples),))
            rewards        = np.empty((len(samples),))

            for i, (state, action, reward, newstate) in enumerate(samples):
                states[i] = state
                action_mask[i] = 0
                action_mask[i][action] = 1
                rewards[i] = reward
                if newstate is not None:
                    newstates[i] = newstate
                    newstates_mask[i] = 1
                else:
                    newstates[i] = 0
                    newstates_mask[i] = 0


            calculate_summaries = self.iteration % 100 == 0 and \
                    self.summary_writer is not None

            # запускаем вычисления
            # сначала считаем ошибку сети
            # потом запускаем оптимизацию сети
            # далее собираем статистику (необязательный шаг
            # нужный для построения графиков обучения)
            cost, _, summary_str = self.s.run([
                self.prediction_error,
                self.train_op,
                self.summarize if calculate_summaries else self.no_op1,
            ], {
                self.observation:            states,
                self.next_observation:       newstates,
                self.next_observation_mask:  newstates_mask,
                self.action_mask:            action_mask,
                self.rewards:                rewards,
            })

            # подстраиваем нейросеть Т
            self.s.run(self.target_network_update)

            if calculate_summaries:
                self.summary_writer.add_summary(summary_str, self.iteration)

            self.iteration += 1

        self.number_of_times_train_called += 1
