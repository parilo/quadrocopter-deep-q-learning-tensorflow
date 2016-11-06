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
	"graph-2d-ddpg-lstm-stepped-mlp.pb"
) {}

void DDPG_LSTM::control (
	const ObservationSeqLimited& obs,
	std::vector<float>& action,
	double randomness
) {
	Tensor observationSeq (DT_FLOAT, TensorShape({1, QuadrocopterBrain::lstmStepsCount, QuadrocopterBrain::observationSize}));
	
	fillTensor3D (obs, observationSeq, 0);

	std::vector<std::pair<string, tensorflow::Tensor>> inputs = {
		{ "observation", observationSeq }
	};

	// The session will initialize the outputs
	std::vector<tensorflow::Tensor> outputs;

	auto status = session->Run(
		inputs, {
			"taking_action/actor_action"
		}, {}, &outputs
	);
	if (!status.ok()) {
		std::cout << "tf error: " << status.ToString() << "\n";
	}
	std::normal_distribution<float> normalNoise (0, randomness * 2);
	int ai = 0;
//std::cout << "action";
	for (auto& actionItem : action) {
//std::cout << " " << outputs [0].matrix<float>()(0, ai);
		actionItem = outputs [0].matrix<float>()(0, ai++) + normalNoise (randomGenerator);
	}
//std::cout << std::endl;

	actionsExecutedSoFar++;
	if (explorationPeriod>0) explorationPeriod--;
}

float DDPG_LSTM::trainOnMinibatch (std::vector<const ExperienceItem*> minibatch) {
	std::lock_guard<std::mutex> lock (saveGraphMutex);

	int minibatchSize = (int) minibatch.size();
	int observationSize = QuadrocopterBrain::observationSize;
	int lstmStepSize = QuadrocopterBrain::lstmStepsCount;
	Tensor observations (DT_FLOAT, TensorShape({minibatchSize, lstmStepSize, observationSize}));
	Tensor newObservations (DT_FLOAT, TensorShape({minibatchSize, lstmStepSize, observationSize}));
	Tensor newObservationsMasks (DT_FLOAT, TensorShape({minibatchSize, lstmStepSize, 1}));
	Tensor rewards (DT_FLOAT, TensorShape({minibatchSize, lstmStepSize, 1}));
	Tensor givenAction (DT_FLOAT, TensorShape({minibatchSize, lstmStepSize, QuadrocopterBrain::contActionSize}));

	int expI = 0;
	for (auto expItem : minibatch) {
	
		fillTensor3D (expItem->prevStates, observations, expI);
		fillTensor3D (expItem->nextStates, newObservations, expI);
		fillTensor3D (expItem->lstmActions, givenAction, expI);
		fillTensor3D (expItem->lstmRewards, rewards, expI);
		
//		for (int lstmI=0; lstmI < lstmStepSize; lstmI++) {
//			int ai = 0;
//			for (auto& actionItem : expItem->actionCont) {
//				givenAction.tensor<float, 3>()(expI, lstmI, ai) = actionItem;
//				ai++;
//			}
//		}
		
//		rewards.matrix<float>()(expI, 0) = (float) expItem->rewardLambda;
		for (int lstmI=0; lstmI < lstmStepSize; lstmI++)
			newObservationsMasks.tensor<float, 3>()(expI, lstmI, 0) = 1;
		
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
		{ "observation", observations },
		{ "given_action", givenAction },
		{ "estimating_future_reward/rewards", rewards },
		{ "estimating_future_reward/next_observation_mask", newObservationsMasks },
		{ "next_observation", newObservations },
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
//std::cerr << "--- train outputs: " << outputTensors [0].DebugString() << std::endl;
//	printTensor<float>(outputTensors [0]);
//	std::vector<float> v;
//	getTensorValues<float>(outputs [0], v);
	auto predictionError = outputTensors [0].scalar<float>();
	float err = predictionError ();
//std::cerr << "--- train err: " << err << std::endl;

	return err;
}
