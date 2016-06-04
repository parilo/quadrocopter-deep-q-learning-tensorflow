//
//  BrainContinousDeepQ.cpp
//  QuadrocopterBrain
//
//  Created by anton on 13/02/16.
//  Copyright © 2016 anton. All rights reserved.
//

#include "BrainContinousDeepQ.hpp"
#include "Lib.hpp"

using namespace tensorflow;

BrainContinousDeepQ::BrainContinousDeepQ () {

	Status status = NewSession(SessionOptions(), &session);
	if (!status.ok()) {
		std::cerr << "tf error: " << status.ToString() << "\n";
	}

	// Read in the protobuf graph we exported
	GraphDef graph_def;
	status = ReadBinaryProto(Env::Default(), "/Users/anton/devel/unity/QuadrocopterHabr2D/TensorflowGraph/models/graph.pb", &graph_def);
	if (!status.ok()) {
		std::cerr << "tf error: " << status.ToString() << "\n";
	}

	// Add the graph to the session
	status = session->Create(graph_def);
	if (!status.ok()) {
		std::cerr << "tf error: " << status.ToString() << "\n";
	}
	
	Tensor observation (DT_FLOAT, TensorShape({1, 5}));
	std::vector<std::pair<string, tensorflow::Tensor>> inputs = {
		{ "taking_action/observation", observation }
	};
	std::vector<tensorflow::Tensor> outputs;
	status = session->Run(inputs, {}, {"init_all_vars_op"}, &outputs);
	if (!status.ok()) {
		std::cerr << "tf error: " << status.ToString() << "\n";
		return;
	}
	
}

/**
	Linear annealing between p_initial and p_final
	over total steps - computes value at step n

	расчет вероятности случайного действия
	с учетом уменьшения с итерациями
	(линейный отжиг)
*/
double BrainContinousDeepQ::linearAnnealing() {
	if (actionsExecutedSoFar >= explorationPeriod)
		return randomActionProbabilityFinal;
	else
		return randomActionProbabilityInitial - (actionsExecutedSoFar * (randomActionProbabilityInitial - randomActionProbabilityFinal)) / (explorationPeriod);
}

/**
Given observation returns the action that should be chosen using
DeepQ learning strategy. Does not backprop.

управление
*/
long BrainContinousDeepQ::control (const Observation& ob) {
	double explorationP = linearAnnealing();
	long actionIndex;
	if (Lib::randDouble(0.0, 1.0) < explorationP) {
		actionIndex = Lib::randInt(0, numActions-1);
	} else {
		
		Tensor observation (DT_FLOAT, TensorShape({1, ob.getSize()}));
		int i=0;
		for (auto obItem : ob.data) {
			observation.matrix<float>()(0, i++) = obItem;
		}

		std::vector<std::pair<string, tensorflow::Tensor>> inputs = {
			{ "taking_action/observation", observation }
		};

		// The session will initialize the outputs
		std::vector<tensorflow::Tensor> outputs;

		//getting index of max scored action
//		Status status = session->Run(inputs, {"taking_action/predicted_actions"}, {}, &outputs);

		//ruling basing on all actions by
		//performing actions in amplified by action scores
		Status status = session->Run(inputs, {"taking_action/action_scores"}, {}, &outputs);
		if (!status.ok()) {
			std::cout << "tf error: " << status.ToString() << "\n";
		}
		
		std::cerr << "--- action scores: " << outputs [0].DebugString() << std::endl;
		
//		auto action = outputs [0].scalar<float>();
		actionIndex = 0;//action ();
	}

	actionsExecutedSoFar++;
	
	return actionIndex;

//        assert len(observation.shape) == 1, \
//                "Action is performed based on single observation."
//
//        self.actions_executed_so_far += 1
//        # расчет вероятности случайного действия
//        exploration_p = self.linear_annealing(self.actions_executed_so_far,
//                                              self.exploration_period,
//                                              1.0,
//                                              self.random_action_probability)
//
//        if random.random() < exploration_p:
//            # случайное действие
//            return random.randint(0, self.num_actions - 1)
//        else:
//            # действие выбранное нейросетью
//            return self.s.run(self.predicted_actions, {self.observation: observation[np.newaxis,:]})[0]
}

void BrainContinousDeepQ::controlContinuous(const Observation &ob, std::vector<float>& actionsScores) {
//	double explorationP = linearAnnealing();
//	long actionIndex;
//	if (Lib::randDouble(0.0, 1.0) < explorationP) {
//		actionIndex = Lib::randInt(0, numActions-1);
//	} else {
	
		Tensor observation (DT_FLOAT, TensorShape({1, ob.getSize()}));
		int i=0;
		for (auto obItem : ob.data) {
			observation.matrix<float>()(0, i++) = obItem;
		}

		std::vector<std::pair<string, tensorflow::Tensor>> inputs = {
			{ "taking_action/observation", observation }
		};

		// The session will initialize the outputs
		std::vector<tensorflow::Tensor> outputs;

		//getting index of max scored action
//		Status status = session->Run(inputs, {"taking_action/predicted_actions"}, {}, &outputs);

		//ruling basing on all actions by
		//performing actions in amplified by action scores
		Status status = session->Run(inputs, {"taking_action/action_scores"}, {}, &outputs);
		if (!status.ok()) {
			std::cout << "tf error: " << status.ToString() << "\n";
		}
		
//		std::cerr << "--- action scores: " << outputs [0].DebugString() << std::endl;
		auto as = outputs [0].matrix<float>();
		actionsScores.clear();
		for (int i=0; i<numActions; i++) {
			actionsScores.push_back(as (0, i));
		}
//	}

	actionsExecutedSoFar++;
}

void BrainContinousDeepQ::train (const std::vector<ExperienceItem>& experience) {

//	if (experience.size() < minibatchSize) return;
//	
//	int observationSize = experience.front().prevState.getSize ();
//	
//	Tensor observations (DT_FLOAT, TensorShape({minibatchSize, observationSize}));
//	Tensor newObservations (DT_FLOAT, TensorShape({minibatchSize, observationSize}));
//	Tensor actionMasks (DT_FLOAT, TensorShape({minibatchSize, numActions}));
//
//	Tensor newObservationsMasks (DT_FLOAT, TensorShape({minibatchSize, 1}));
//	Tensor rewards (DT_FLOAT, TensorShape({minibatchSize, 1}));
//	
//	std::vector<const ExperienceItem*> minibatch;
//	Lib::getRandomSubArray(experience, minibatch, minibatchSize);
//	
//	std::cerr << "--- minibatch experience " << minibatch.size() << std::endl;
//	
//	actionMasks.matrix<float>().setZero();
//	
//	int expI = 0;
//	for (auto expItem : minibatch) {
//	
//		const std::vector<float>& prevState = expItem->prevState.data;
//		const std::vector<float>& nextState = expItem->nextState.data;
//	
//		for (int i=0; i<observationSize; i++) {
//			observations.matrix<float>()(expI, i) = prevState [i];
//			newObservations.matrix<float>()(expI, i) = nextState [i];
//		}
//
//		for (int i=0; i<numActions; i++) {
//			actionMasks.matrix<float>()(expI, i) = 1;
//		}
//	
////		observations.matrix<float>()(expI, 0) = expItem->prevState.currentRotW;
////		observations.matrix<float>()(expI, 1) = expItem->prevState.currentRotX;
////		observations.matrix<float>()(expI, 2) = expItem->prevState.currentRotY;
////		observations.matrix<float>()(expI, 3) = expItem->prevState.currentRotZ;
////
////		observations.matrix<float>()(expI, 4) = expItem->prevState.targetX;
////		observations.matrix<float>()(expI, 5) = expItem->prevState.targetY;
////		observations.matrix<float>()(expI, 6) = expItem->prevState.targetZ;
////
////		observations.matrix<float>()(expI, 7) = expItem->prevState.motor1power;
////		observations.matrix<float>()(expI, 8) = expItem->prevState.motor2power;
////		observations.matrix<float>()(expI, 9) = expItem->prevState.motor3power;
////		observations.matrix<float>()(expI, 10) = expItem->prevState.motor4power;
//
//
////		newObservations.matrix<float>()(expI, 0) = expItem->nextState.currentRotW;
////		newObservations.matrix<float>()(expI, 1) = expItem->nextState.currentRotX;
////		newObservations.matrix<float>()(expI, 2) = expItem->nextState.currentRotY;
////		newObservations.matrix<float>()(expI, 3) = expItem->nextState.currentRotZ;
////
////		newObservations.matrix<float>()(expI, 4) = expItem->nextState.targetX;
////		newObservations.matrix<float>()(expI, 5) = expItem->nextState.targetY;
////		newObservations.matrix<float>()(expI, 6) = expItem->nextState.targetZ;
////
////		newObservations.matrix<float>()(expI, 7) = expItem->nextState.motor1power;
////		newObservations.matrix<float>()(expI, 8) = expItem->nextState.motor2power;
////		newObservations.matrix<float>()(expI, 9) = expItem->nextState.motor3power;
////		newObservations.matrix<float>()(expI, 10) = expItem->nextState.motor4power;
//		
////		actionMasks.matrix<float>()(expI, expItem->action) = 1;
//		rewards.matrix<float>()(expI, 0) = (float) expItem->reward;
//		newObservationsMasks.matrix<float>()(expI, 0) = 1;
//		
//		expI++;
//	}
//	
////	std::cerr << "--- observations" << std::endl;
////	printTensor(observations);
////	std::cerr << "--- newObservations" << std::endl;
////	printTensor(newObservations);
////	std::cerr << "--- newObservationsMasks" << std::endl;
////	printTensor(newObservationsMasks);
////	std::cerr << "--- actionMasks" << std::endl;
////	printTensor(actionMasks);
////	std::cerr << "--- rewards" << std::endl;
////	printTensor(rewards);
//	
//	std::vector<std::pair<string, tensorflow::Tensor>> inputs = {
//		{ "taking_action/observation", observations },
//		{ "q_value_precition/action_mask", actionMasks },
//		{ "estimating_future_rewards/rewards", rewards },
//		{ "estimating_future_rewards/next_observation_mask", newObservationsMasks },
//		{ "estimating_future_rewards/next_observation", newObservations }
//	};
//
//	std::vector<tensorflow::Tensor> outputs;
//
////	Status status = session->Run(inputs, {"q_value_precition/masked_action_scores"}, {}, &outputs);
////	if (!status.ok()) {
////		std::cerr << "tf error: " << status.ToString() << "\n";
////		return;
////	}
////std::cerr << "--- train masked_action_scores: " << outputs [0].DebugString() << std::endl;
//
//	Status status = session->Run(inputs, {"q_value_precition/prediction_error"}, {}, &outputs);
//	if (!status.ok()) {
//		std::cerr << "tf error: " << status.ToString() << "\n";
//		return;
//	}
//std::cerr << "--- train prediction_error: " << outputs [0].DebugString() << std::endl;
//
//	status = session->Run(inputs, {}, {"q_value_precition/train_op"}, &outputs);
//	if (!status.ok()) {
//		std::cerr << "tf error: " << status.ToString() << "\n";
//		return;
//	}
//
//	status = session->Run(inputs, {}, {"target_network_update/target_network_update"}, &outputs);
//	if (!status.ok()) {
//		std::cerr << "tf error: " << status.ToString() << "\n";
//		return;
//	}
//
////            # из всего сохраненного опыта случайно выбираем
////            # пачку из minibatch_size обучающих примеров
////            # sample experience.
////            samples   = random.sample(range(len(self.experience)), self.minibatch_size)
////            samples   = [self.experience[i] for i in samples]
////
////            # представляем обучающие примеры
////            # в нужном виде
////            # bach states
////            states         = np.empty((len(samples), self.observation_size))
////            newstates      = np.empty((len(samples), self.observation_size))
////            action_mask    = np.zeros((len(samples), self.num_actions))
////
////            newstates_mask = np.empty((len(samples),))
////            rewards        = np.empty((len(samples),))
////
////            for i, (state, action, reward, newstate) in enumerate(samples):
////                states[i] = state
////                action_mask[i] = 0
////                action_mask[i][action] = 1
////                rewards[i] = reward
////                if newstate is not None:
////                    newstates[i] = newstate
////                    newstates_mask[i] = 1
////                else:
////                    newstates[i] = 0
////                    newstates_mask[i] = 0
////
////
////            calculate_summaries = self.iteration % 100 == 0 and \
////                    self.summary_writer is not None
////
////            # запускаем вычисления
////            # сначала считаем ошибку сети
////            # потом запускаем оптимизацию сети
////            # далее собираем статистику (необязательный шаг
////            # нужный для построения графиков обучения)
////            cost, _, summary_str = self.s.run([
////                self.prediction_error,
////                self.train_op,
////                self.summarize if calculate_summaries else self.no_op1,
////            ], {
////                self.observation:            states,
////                self.next_observation:       newstates,
////                self.next_observation_mask:  newstates_mask,
////                self.action_mask:            action_mask,
////                self.rewards:                rewards,
////            })
////
////            # подстраиваем нейросеть Т
////            self.s.run(self.target_network_update)

}
