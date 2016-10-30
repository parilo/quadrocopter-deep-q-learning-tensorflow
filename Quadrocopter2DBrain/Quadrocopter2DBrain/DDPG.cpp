//
//  DDPG.cpp
//  Quadrocopter2DBrain
//
//  Created by anton on 13/08/16.
//  Copyright © 2016 anton. All rights reserved.
//

#include "QuadrocopterBrain.hpp"
#include "DDPG.hpp"

using namespace tensorflow;

DDPG::DDPG () : BrainDiscreteDeepQ(
//	"graph-1d-ddpg.pb"
//	"graph-2d-ddpg.pb"
	"graph-2d-ddpg-mlp-seq.pb"
) {}

void DDPG::control (const ObservationSeqLimited& obs, std::vector<float>& action, double randomness) {
	
//	double explorationP = linearAnnealing (randomness);
//std::cerr <<"--- explorationP: " << explorationP << " act: " << actionsExecutedSoFar <<  std::endl;

//	actor/lstm/state_c
//	actor/lstm/state_h

	Tensor observationSeq (DT_FLOAT, TensorShape({1, obs.getSize()}));
	
	fillTensor (obs, observationSeq, 0);
	
//		std::cerr << "--- BrainDiscreteDeepQ::control" << std::endl;
//		printTensor(observationSeq);

	std::vector<std::pair<string, tensorflow::Tensor>> inputs = {
		{ "taking_action/observation", observationSeq }
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
	
//	std::cerr << "--- action scores: " << std::endl;
//	printTensor<float>(outputs [0]);
//		auto action = outputs [0].scalar<int>();
//		actionIndex = action ();
//		std::cerr << "--- action: " << actionIndex << std::endl;

	std::normal_distribution<float> normalNoise (0, randomness * 2);
	int ai = 0;
	for (auto& actionItem : action) {
		actionItem = outputs [0].matrix<float>()(0, ai++) + normalNoise (randomGenerator);
	}

	actionsExecutedSoFar++;
	if (explorationPeriod>0) explorationPeriod--;
}

float DDPG::trainOnMinibatch (std::vector<const ExperienceItem*> minibatch) {
	std::lock_guard<std::mutex> lock (saveGraphMutex);

	int minibatchSize = (int) minibatch.size();
	int observationSize = QuadrocopterBrain::observationSize * (QuadrocopterBrain::useObsSeq?QuadrocopterBrain::mlpSeqSize:1);
	Tensor observations (DT_FLOAT, TensorShape({minibatchSize, observationSize}));
	Tensor newObservations (DT_FLOAT, TensorShape({minibatchSize, observationSize}));
	Tensor newObservationsMasks (DT_FLOAT, TensorShape({minibatchSize, 1}));
	Tensor rewards (DT_FLOAT, TensorShape({minibatchSize, 1}));
	Tensor givenAction (DT_FLOAT, TensorShape({minibatchSize, QuadrocopterBrain::contActionSize}));
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
		{ "estimating_future_reward/next_observation", newObservations }
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
