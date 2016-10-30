//
//  DDPG_LSTM.cpp
//  Quadrocopter2DBrain
//
//  Created by anton on 06/10/16.
//  Copyright © 2016 anton. All rights reserved.
//

#include "QuadrocopterBrain.hpp"
#include "DDPG_LSTM_Weak.hpp"

using namespace tensorflow;

DDPG_LSTM_Weak::DDPG_LSTM_Weak () : BrainDiscreteDeepQ(
	"graph-2d-ddpg-lstm-mlp-weak.pb"
) {}

void DDPG_LSTM_Weak::control (
	const ObservationSeqLimited& obs,
	const std::vector<float>& lstmStateC,
	const std::vector<float>& lstmStateH,
	std::vector<float>& action,
	double randomness//,
//	std::vector<float>& outLstmStateC,
//	std::vector<float>& outLstmStateH
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
			"taking_action/actor_action"//,
//			"taking_action/actor/lstm/read_state_c",
//			"taking_action/actor/lstm/read_state_h"
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
	
//	getTensorValues<float>(outputs [1], outLstmStateC);
//	getTensorValues<float>(outputs [2], outLstmStateH);

	actionsExecutedSoFar++;
	if (explorationPeriod>0) explorationPeriod--;
}

float DDPG_LSTM_Weak::trainOnMinibatch (std::vector<ExperienceItem*> minibatch) {
//std::cout << "--- DDPG_LSTM_Weak::trainOnMinibatch" << std::endl;
	std::lock_guard<std::mutex> lock (saveGraphMutex);

	int minibatchSize = (int) minibatch.size();
	int observationSize = QuadrocopterBrain::observationSize;
	Tensor observations (DT_FLOAT, TensorShape({minibatchSize, observationSize}));
	Tensor newObservations (DT_FLOAT, TensorShape({minibatchSize, observationSize}));
	Tensor newObservationsMasks (DT_FLOAT, TensorShape({minibatchSize, 1}));
	Tensor rewards (DT_FLOAT, TensorShape({minibatchSize, 1}));
	Tensor givenAction (DT_FLOAT, TensorShape({minibatchSize, QuadrocopterBrain::contActionSize}));
	Tensor actorLstmStateC (DT_FLOAT, TensorShape({minibatchSize, QuadrocopterBrain::lstmStateSize}));
	Tensor actorLstmStateH (DT_FLOAT, TensorShape({minibatchSize, QuadrocopterBrain::lstmStateSize}));
	Tensor criticLstmStateC (DT_FLOAT, TensorShape({minibatchSize, QuadrocopterBrain::lstmStateSize}));
	Tensor criticLstmStateH (DT_FLOAT, TensorShape({minibatchSize, QuadrocopterBrain::lstmStateSize}));
	Tensor targetActorLstmStateC (DT_FLOAT, TensorShape({minibatchSize, QuadrocopterBrain::lstmStateSize}));
	Tensor targetActorLstmStateH (DT_FLOAT, TensorShape({minibatchSize, QuadrocopterBrain::lstmStateSize}));
	Tensor targetCriticLstmStateC (DT_FLOAT, TensorShape({minibatchSize, QuadrocopterBrain::lstmStateSize}));
	Tensor targetCriticLstmStateH (DT_FLOAT, TensorShape({minibatchSize, QuadrocopterBrain::lstmStateSize}));

	int expI = 0;
	for (auto expItem : minibatch) {
if (expItem->prevStates.getSize () == 0)
std::cout << "--- zero expI " << expI << std::endl;
		fillTensor (expItem->prevStates, observations, expI);
		fillTensor (expItem->nextStates, newObservations, expI);
		
		int ai = 0;
		for (auto& actionItem : expItem->actionCont) {
			givenAction.matrix<float>()(expI, ai) = actionItem;
			ai++;
		}
		
		rewards.matrix<float>()(expI, 0) = (float) expItem->rewardLambda;
		newObservationsMasks.matrix<float>()(expI, 0) = 1;
		
		fillTensor<float>(actorLstmStateC, expItem->actorLstmStateC, expI);
		fillTensor<float>(actorLstmStateH, expItem->actorLstmStateH, expI);
		fillTensor<float>(criticLstmStateC, expItem->criticLstmStateC, expI);
		fillTensor<float>(criticLstmStateH, expItem->criticLstmStateH, expI);
		fillTensor<float>(targetActorLstmStateC, expItem->targetActorLstmStateC, expI);
		fillTensor<float>(targetActorLstmStateH, expItem->targetActorLstmStateH, expI);
		fillTensor<float>(targetCriticLstmStateC, expItem->targetCriticLstmStateC, expI);
		fillTensor<float>(targetCriticLstmStateH, expItem->targetCriticLstmStateH, expI);
		
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
		{ "critic/lstm/state_c", criticLstmStateC },
		{ "critic/lstm/state_h", criticLstmStateH },
		{ "actor/lstm/state_c", actorLstmStateC },
		{ "actor/lstm/state_h", actorLstmStateH },
		{ "target_actor/target_actor/lstm/state_c", targetActorLstmStateC },
		{ "target_actor/target_actor/lstm/state_h", targetActorLstmStateH },
		{ "target_critic/target_critic/lstm/state_c", targetCriticLstmStateC },
		{ "target_critic/target_critic/lstm/state_h", targetCriticLstmStateH }
	};
	
	std::vector<tensorflow::Tensor> outputTensors;
	
	auto status = session->Run(inputs, {
		"critic_update/critic_error",
		"taking_action/actor/lstm/read_state_c",
		"taking_action/actor/lstm/read_state_h",
		"critic_update/critic/lstm/read_state_c",
		"critic_update/critic/lstm/read_state_h",
		"estimating_future_reward/read_target_actor_state_c",
		"estimating_future_reward/read_target_actor_state_h",
		"estimating_future_reward/read_target_critic_state_c",
		"estimating_future_reward/read_target_critic_state_h"
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

//std::cerr << "--- train outputs 1: " << outputTensors [1].shape().dim_size(0) << std::endl;
//std::cerr << "--- train outputs 2: " << outputTensors [2].DebugString() << std::endl;

	auto& outActorC = outputTensors [1];
	auto& outActorH = outputTensors [2];
	auto& outCriticC = outputTensors [3];
	auto& outCriticH = outputTensors [4];
	auto& outTargetActorC = outputTensors [5];
	auto& outTargetActorH = outputTensors [6];
	auto& outTargetCriticC = outputTensors [7];
	auto& outTargetCriticH = outputTensors [8];

	expI = 0;
	for (auto expItem : minibatch) {
		getTensorValues<float>(outActorC, expItem->actorLstmStateC, expI);
		getTensorValues<float>(outActorH, expItem->actorLstmStateH, expI);
		getTensorValues<float>(outCriticC, expItem->criticLstmStateC, expI);
		getTensorValues<float>(outCriticH, expItem->criticLstmStateH, expI);
		getTensorValues<float>(outTargetActorC, expItem->targetActorLstmStateC, expI);
		getTensorValues<float>(outTargetActorH, expItem->targetActorLstmStateH, expI);
		getTensorValues<float>(outTargetCriticC, expItem->targetCriticLstmStateC, expI);
		getTensorValues<float>(outTargetCriticH, expItem->targetCriticLstmStateH, expI);
		expI++;
	}

	return err;
}
