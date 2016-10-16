//
//  DDPG_LSTM.cpp
//  Quadrocopter2DBrain
//
//  Created by anton on 06/10/16.
//  Copyright © 2016 anton. All rights reserved.
//

#include "QuadrocopterBrain.hpp"
#include "DDPG_LSTM.hpp"

using namespace tensorflow;

DDPG_LSTM::DDPG_LSTM () : BrainDiscreteDeepQ(
	"graph-2d-ddpg-lstm-mlp.pb"
) {}

void DDPG_LSTM::control (
	const ObservationSeqLimited& obs,
	const std::vector<float>& lstmStateC,
	const std::vector<float>& lstmStateH,
	std::vector<float>& action,
	double randomness,
	std::vector<float>& outLstmStateC,
	std::vector<float>& outLstmStateH
) {

	Tensor observationSeq (DT_FLOAT, TensorShape({1, obs.getSize()}));
	Tensor lstmC (DT_FLOAT, TensorShape({1, QuadrocopterBrain::lstmStateSize}));
	Tensor lstmH (DT_FLOAT, TensorShape({1, QuadrocopterBrain::lstmStateSize}));
	
	fillTensor (obs, observationSeq, 0);
	fillTensor<float>(lstmC, lstmStateC, 0);
	fillTensor<float>(lstmH, lstmStateH, 0);

	std::vector<std::pair<string, tensorflow::Tensor>> inputs = {
		{ "taking_action/observation", observationSeq },
		{ "actor/lstm/state_c", lstmC },
		{ "actor/lstm/state_h", lstmH }
	};

	// The session will initialize the outputs
	std::vector<tensorflow::Tensor> outputs;

	auto status = session->Run(
		inputs, {
			"taking_action/actor_action",
			"taking_action/actor/lstm/read_state_c",
			"taking_action/actor/lstm/read_state_h"
		}, {}, &outputs
	);
	if (!status.ok()) {
		std::cout << "tf error: " << status.ToString() << "\n";
	}

	std::normal_distribution<float> normalNoise (0, randomness * 2);
	int ai = 0;
	for (auto& actionItem : action) {
		actionItem = outputs [0].matrix<float>()(0, ai++) + normalNoise (randomGenerator);
	}
	
	getTensorValues<float>(outputs [1], outLstmStateC);
	getTensorValues<float>(outputs [2], outLstmStateH);

	actionsExecutedSoFar++;
	if (explorationPeriod>0) explorationPeriod--;
}

float DDPG_LSTM::trainOnMinibatch (std::vector<const ExperienceItem*> minibatch) {
	std::lock_guard<std::mutex> lock (saveGraphMutex);

	int minibatchSize = (int) minibatch.size();
	int observationSize = QuadrocopterBrain::observationSize;
	Tensor observations (DT_FLOAT, TensorShape({minibatchSize, observationSize}));
	Tensor newObservations (DT_FLOAT, TensorShape({minibatchSize, observationSize}));
	Tensor newObservationsMasks (DT_FLOAT, TensorShape({minibatchSize, 1}));
	Tensor rewards (DT_FLOAT, TensorShape({minibatchSize, 1}));
	Tensor givenAction (DT_FLOAT, TensorShape({minibatchSize, QuadrocopterBrain::contActionSize}));
	Tensor prevLstmStateC (DT_FLOAT, TensorShape({minibatchSize, QuadrocopterBrain::lstmStateSize}));
	Tensor prevLstmStateH (DT_FLOAT, TensorShape({minibatchSize, QuadrocopterBrain::lstmStateSize}));
	Tensor nextLstmStateC (DT_FLOAT, TensorShape({minibatchSize, QuadrocopterBrain::lstmStateSize}));
	Tensor nextLstmStateH (DT_FLOAT, TensorShape({minibatchSize, QuadrocopterBrain::lstmStateSize}));
	int expI = 0;
	for (auto expItem : minibatch) {
	
		fillTensor (expItem->prevStates, observations, expI);
		fillTensor (expItem->nextStates, newObservations, expI);
		
		int ai = 0;
		for (auto& actionItem : expItem->actionCont) {
			givenAction.matrix<float>()(expI, ai) = actionItem;
			ai++;
		}
		
		rewards.matrix<float>()(expI, 0) = (float) expItem->rewardLambda;
		newObservationsMasks.matrix<float>()(expI, 0) = 1;
		
		fillTensor<float>(prevLstmStateC, expItem->prevLstmStateC, expI);
		fillTensor<float>(prevLstmStateH, expItem->prevLstmStateH, expI);
		fillTensor<float>(nextLstmStateC, expItem->nextLstmStateC, expI);
		fillTensor<float>(nextLstmStateH, expItem->nextLstmStateH, expI);
		
		expI++;
	}
	
//	std::cerr << "observations: " << std:: endl;
//	printTensor<float>(observations);
//	std::cerr << "newObservations: " << std:: endl;
//	printTensor<float>(newObservations);
//	std::cerr << "givenAction: " << std:: endl;
//	printTensor<float>(givenAction);
//	std::cerr << "rewards: " << std:: endl;
//	printTensor<float>(rewards);
//	std::cerr << "newObservationsMasks: " << std:: endl;
//	printTensor<float>(newObservationsMasks);
	
	std::vector<std::pair<string, tensorflow::Tensor>> inputs = {
		{ "taking_action/observation", observations },
		{ "critic_update/given_action", givenAction },
		{ "estimating_future_reward/rewards", rewards },
		{ "estimating_future_reward/next_observation_mask", newObservationsMasks },
		{ "estimating_future_reward/next_observation", newObservations },
		{ "critic/lstm/state_c", prevLstmStateC },
		{ "critic/lstm/state_h", prevLstmStateH },
		{ "actor/lstm/state_c", prevLstmStateC },
		{ "actor/lstm/state_h", prevLstmStateH },
		{ "target_actor/target_actor/lstm/state_c", nextLstmStateC },
		{ "target_actor/target_actor/lstm/state_h", nextLstmStateH },
		{ "target_critic/target_critic/lstm/state_c", nextLstmStateC },
		{ "target_critic/target_critic/lstm/state_h", nextLstmStateH }
	};
	
	std::vector<tensorflow::Tensor> outputTensors;
	
	auto status = session->Run(inputs, {
		"critic_update/critic_error"
	}, {
		"actor_update/actor_train_op",
		"critic_update/critic_train_op",
		"target_network_update/target_networks_update"
	}, &outputTensors);
	if (!status.ok()) {
		std::cerr << "tf error: " << status.ToString() << "\n";
		return 0;
	}
//std::cerr << "--- train outputs: " << outputs [0].DebugString() << std::endl;
//	printTensor<float>(outputTensors [0]);
//	std::vector<float> v;
//	getTensorValues<float>(outputs [0], v);
	auto predictionError = outputTensors [0].scalar<float>();
	float err = predictionError ();

	return err;
}
