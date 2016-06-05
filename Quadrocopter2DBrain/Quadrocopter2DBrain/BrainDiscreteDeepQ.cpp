//
//  BrainDiscreteDeepQ.cpp
//  QuadrocopterBrain
//
//  Created by anton on 17/01/16.
//  Copyright © 2016 anton. All rights reserved.
//

#include <fstream>

#include "BrainDiscreteDeepQ.hpp"
#include "Lib.hpp"
#include "Tensors.hpp"
#include "QuadrocopterBrain.hpp"

#define QUOTE(name) #name
#define STR(macro) QUOTE(macro)
#define GRAPHDIR STR(TF_GRAPH_DIR)

using namespace tensorflow;

BrainDiscreteDeepQ::BrainDiscreteDeepQ () {

	Status status = NewSession(SessionOptions(), &session);
	if (!status.ok()) {
		std::cerr << "tf error: " << status.ToString() << "\n";
	}

	// Read in the protobuf graph we exported
	GraphDef graph_def;
	status = ReadBinaryProto(Env::Default(), GRAPHDIR "/graph.pb", &graph_def);
	if (!status.ok()) {
		std::cerr << "tf error: " << status.ToString() << "\n";
	}

	// Add the graph to the session
	status = session->Create(graph_def);
	if (!status.ok()) {
		std::cerr << "tf error: " << status.ToString() << "\n";
	}

	initGraphState(graph_def);
//	saveGraphState(graph_def);
//	loadGraphState(graph_def);

}

void BrainDiscreteDeepQ::initGraphState (tensorflow::GraphDef& graph_def) {
	
	std::vector<tensorflow::Tensor> outputs;
	Status status = session->Run({}, {}, {"init_all_vars_op"}, &outputs);
	if (!status.ok()) {
		std::cerr << "tf error: " << status.ToString() << "\n";
		return;
	}
}

void BrainDiscreteDeepQ::saveGraphState (GraphDef& graph_def) {

	std::vector<tensorflow::Tensor> out;
	std::vector<string> vNames;

std::cerr << "--- begin saving variables" << std::endl;

	int node_count = graph_def.node_size();
	for (int i = 0; i < node_count; i++) {
		auto n = graph_def.node(i);

		if (
			n.name().find("readVariable") != std::string::npos
		) {
			vNames.push_back(n.name());
std::cerr << "--- variable: " << n.name () << std::endl;
		}
	}

	Status status = session->Run({}, vNames, {}, &out);
	if (!status.ok()) {
		std::cout << "tf error1: " << status.ToString() << "\n";
	}
	
	int variableCount = out.size ();
std::cerr << "--- got variables: " << variableCount << std::endl;
	std::fstream output("/Users/anton/devel/QuadropcopterControl/console-box2d/TensorflowGraph/states/graph-state", std::ios::out | std::ios::binary);
	output.write (reinterpret_cast<const char *>(&variableCount), sizeof(int));
	for (auto& tensor : out) {
		int tensorSize = tensor.TotalBytes();
std::cerr << "--- tensor: " << tensorSize << std::endl;
		TensorProto p;
		tensor.AsProtoField (&p);
		
		std::string pStr;
		p.SerializeToString(&pStr);
		int serializedTensorSize = pStr.size();
		output.write (reinterpret_cast<const char *>(&serializedTensorSize), sizeof(int));
		output.write (pStr.c_str(), serializedTensorSize);
	}
	output.close ();

	sync ();
}

void BrainDiscreteDeepQ::loadGraphState (GraphDef& graph_def) {
	
	std::vector<std::pair<string, tensorflow::Tensor>> variablesValues;
	std::vector<string> restoreOps;
	
	std::fstream input("/Users/anton/devel/QuadropcopterControl/console-box2d/TensorflowGraph/states/graph-state", std::ios::in | std::ios::binary);
	
	int variableCount;
	input.read(reinterpret_cast<char *>(&variableCount), sizeof(int));
	
std::cerr << "--- stored variables: " << variableCount << std::endl;

	for (int i=0; i<variableCount; i++) {
		int serializedTensorSize;
		input.read(reinterpret_cast<char *>(&serializedTensorSize), sizeof(int));
		std::string pStr;
        pStr.resize(serializedTensorSize);
        char* begin = &*pStr.begin();
        input.read(begin, serializedTensorSize);
		
		TensorProto p;
		p.ParseFromString (pStr);
		
		std::string variableSuffix = (i==0?"":"_"+std::to_string(i));
		variablesValues.push_back ({"variableValue" + variableSuffix, Tensor ()});
		Tensor& t (variablesValues.back ().second);
		t.FromProto (p);
std::cerr << "--- read tensor: " << serializedTensorSize << " / " << t.TotalBytes() << std::endl;
		
		restoreOps.emplace_back ("resoreVariable" + variableSuffix);
	}
	
	input.close ();
	
	std::vector<tensorflow::Tensor> out;
	Status status = session->Run(variablesValues, {}, restoreOps, &out);
	if (!status.ok()) {
		std::cout << "tf error2: " << status.ToString() << "\n";
	}

};



//void BrainDiscreteDeepQ::setRandomness (double randomness) {
//	randomActionProbabilityFinal = randomness;
//}

/**
	Linear annealing between p_initial and p_final
	over total steps - computes value at step n

	расчет вероятности случайного действия
	с учетом уменьшения с итерациями
	(линейный отжиг)
*/
double BrainDiscreteDeepQ::linearAnnealing(double randomActionProbabilityFinal) {
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
long BrainDiscreteDeepQ::control (const ObservationSeqLimited& obs, double randomness) {
	double explorationP = linearAnnealing (randomness);
	long actionIndex;
	if (Lib::randDouble(0.0, 1.0) < explorationP) {
		actionIndex = Lib::randInt(0, QuadrocopterBrain::numActions-1);
	} else {
		
		Tensor observationSeq (DT_FLOAT, TensorShape({1, obs.getSize()}));
		
//		int i=0;
//		for (auto obItem : ob.data) {
//			observationSeq.matrix<float>()(0, i++) = obItem;
//		}
		fillTensor (obs, observationSeq, 0);
		
//		std::cerr << "--- BrainDiscreteDeepQ::control" << std::endl;
//		printTensor(observationSeq);

		std::vector<std::pair<string, tensorflow::Tensor>> inputs = {
			{ "taking_action/observation", observationSeq }
		};

		// The session will initialize the outputs
		std::vector<tensorflow::Tensor> outputs;

		Status status = session->Run(inputs, {"taking_action/action_scores"}, {}, &outputs);
		if (!status.ok()) {
			std::cout << "tf error: " << status.ToString() << "\n";
		}
		
//		std::cerr << "--- action scores: " << std::endl;
//		printTensor(outputs [0]);
		
//		auto as = outputs [0].matrix<float>();
//		actionsScores.clear();
//		for (int i=0; i<numActions; i++) {
//			actionsScores.push_back(as (0, i));
//		}

		//getting index of max scored action
		status = session->Run(inputs, {"taking_action/predicted_actions"}, {}, &outputs);
		if (!status.ok()) {
			std::cout << "tf error: " << status.ToString() << "\n";
		}
		
//		std::cerr << "--- predicted action: " << outputs [0].DebugString() << std::endl;
		
		auto action = outputs [0].scalar<int>();
		actionIndex = action ();
	}

	actionsExecutedSoFar++;
	
	return actionIndex;
}

//float BrainDiscreteDeepQ::train (
//	const std::vector<ExperienceItem>& experience,
//	std::vector<const ExperienceItem*>& minibatch
//) {
//	if (experience.size() < minibatchSize) return 0;
//
//	std::vector<const ExperienceItem*> minibatch;
//	Lib::getRandomSubArray(experience, minibatch, minibatchSize);
//	
//	return trainOnMinibatch(minibatch);
//}
//
//float train (
//	const std::vector<ExperienceItem>& expLow,
//	const std::vector<ExperienceItem>& expMid,
//	const std::vector<ExperienceItem>& expHigh,
//	std::vector<const ExperienceItem*>& minibatch
//) {
//	if (experience.size() < minibatchSize) return 0;
//
//	std::vector<const ExperienceItem*> minibatch;
//	Lib::getRandomSubArray(experience, minibatch, minibatchSize);
//	
//	return trainOnMinibatch(minibatch);
//}

float BrainDiscreteDeepQ::trainOnMinibatch (std::vector<const ExperienceItem*> minibatch) {

	int minibatchSize = (int) minibatch.size();
	int observationSize = QuadrocopterBrain::observationSize;
	Tensor observations (DT_FLOAT, TensorShape({minibatchSize, observationSize}));
	Tensor newObservations (DT_FLOAT, TensorShape({minibatchSize, observationSize}));
	Tensor actionMasks (DT_FLOAT, TensorShape({minibatchSize, QuadrocopterBrain::numActions}));
	Tensor newObservationsMasks (DT_FLOAT, TensorShape({minibatchSize, 1}));
	Tensor rewards (DT_FLOAT, TensorShape({minibatchSize, 1}));
	actionMasks.matrix<float>().setZero();
	int expI = 0;
	for (auto expItem : minibatch) {
	
		fillTensor (expItem->prevStates, observations, expI);
		fillTensor (expItem->nextStates, newObservations, expI);
		
		actionMasks.matrix<float>()(expI, expItem->action) = 1;
		rewards.matrix<float>()(expI, 0) = (float) expItem->rewardLambda;
		newObservationsMasks.matrix<float>()(expI, 0) = 1;
		
		expI++;
	}
	
//	std::cerr << "rewards: " << std:: endl;
//	printTensor(rewards);
	
	std::vector<std::pair<string, tensorflow::Tensor>> inputs = {
		{ "taking_action/observation", observations },
		{ "q_value_precition/action_mask", actionMasks },
		{ "estimating_future_rewards/rewards", rewards },
		{ "estimating_future_rewards/next_observation_mask", newObservationsMasks },
		{ "estimating_future_rewards/next_observation", newObservations }
	};
	std::vector<tensorflow::Tensor> outputs;

//	Status status = session->Run(inputs, {"q_value_precition/masked_action_scores"}, {}, &outputs);
//	if (!status.ok()) {
//		std::cerr << "tf error: " << status.ToString() << "\n";
//		return;
//	}
//std::cerr << "--- train masked_action_scores: " << outputs [0].DebugString() << std::endl;

	Status status = session->Run(inputs, {"q_value_precition/prediction_error"}, {}, &outputs);
	if (!status.ok()) {
		std::cerr << "tf error: " << status.ToString() << "\n";
		return 0;
	}
	auto predictionError = outputs [0].scalar<float>();
	float err = predictionError ();
	if (isnan(err)) {
std::cerr << std::endl << std::endl << std::endl << std::endl << "--- input tensors:" << std::endl;
std::cerr << "--- observations" << std::endl;
		printTensor<float>(observations);
std::cerr << "--- actionMasks" << std::endl;
		printTensor<float>(actionMasks);
std::cerr << "--- rewards" << std::endl;
		printTensor<float>(rewards);
std::cerr << "--- newObservationsMasks" << std::endl;
		printTensor<float>(newObservationsMasks);
std::cerr << "--- newObservations" << std::endl;
		printTensor<float>(newObservations);
std::cerr << "--- tensors inside graph:" << std::endl << std::endl;
		status = session->Run(inputs, {
			"taking_action/predicted_actions",
		
			"estimating_future_rewards/future_rewards",
			
			"q_value_precition/masked_action_scores",
			"q_value_precition/temp_diff"
		}, {}, &outputs);
		if (!status.ok()) {
			std::cerr << "tf error: " << status.ToString() << "\n";
			return 0;
		}
std::cerr << "--- taking_action/predicted_actions" << std::endl;
		printTensorVector<int64>(outputs[0]);

std::cerr << "--- estimating_future_rewards/future_rewards" << std::endl;
		printTensor<float>(outputs[1]);

std::cerr << "--- q_value_precition/masked_action_scores" << std::endl;
		printTensorVector<float>(outputs[2]);
std::cerr << "--- q_value_precition/temp_diff" << std::endl;
		printTensorVector<float>(outputs[3]);
		
	}

	status = session->Run(inputs, {}, {"q_value_precition/train_op"}, &outputs);
	if (!status.ok()) {
		std::cerr << "tf error: " << status.ToString() << "\n";
		return 0;
	}

	status = session->Run(inputs, {}, {"target_network_update/target_network_update"}, &outputs);
	if (!status.ok()) {
		std::cerr << "tf error: " << status.ToString() << "\n";
		return 0;
	}

	return err;
}

void BrainDiscreteDeepQ::trainEnvModel (const std::vector<ExperienceItem>& experience) {

	int minibatchSize = 64;
	if (experience.size() < minibatchSize) return;
	
	int inputSize = QuadrocopterBrain::observationSize + QuadrocopterBrain::numActions;
	int outputSize = QuadrocopterBrain::observationSize + 1; // 1 is for reward
	
	Tensor env_model_input (DT_FLOAT, TensorShape({minibatchSize, inputSize}));
	Tensor env_model_train_data (DT_FLOAT, TensorShape({minibatchSize, outputSize}));
	std::vector<const ExperienceItem*> minibatch;
	Lib::getRandomSubArray(experience, minibatch, minibatchSize);

	env_model_input.matrix<float>().setZero();
	env_model_train_data.matrix<float>().setZero();
	int expI = 0;
	for (auto expItem : minibatch) {

		fillTensor (expItem->prevStates, env_model_input, expI);
		env_model_input.matrix<float>()(expI, QuadrocopterBrain::observationSize + expItem->action) = 1.0;
	
		fillTensor (expItem->nextStates, env_model_train_data, expI);
		env_model_train_data.matrix<float>()(expI, QuadrocopterBrain::observationSize) = expItem->reward;
		
		expI++;
	}
	
//	std::cerr << "rewards: " << std:: endl;
//	printTensor(rewards);
	
	std::vector<std::pair<string, tensorflow::Tensor>> inputs = {
		{ "env_model_input", env_model_input },
		{ "env_model_train_data", env_model_train_data }
	};
	std::vector<tensorflow::Tensor> outputs;

	Status status;
//	status = session->Run(inputs, {"env_model_prediction"}, {}, &outputs);
//	if (!status.ok()) {
//		std::cerr << "tf error: " << status.ToString() << "\n";
//		return;
//	}
//std::cerr << "--- train prediction_error: " << outputs [0].DebugString() << std::endl;

	status = session->Run(inputs, {"env_model_prediction_error"}, {}, &outputs);
	if (!status.ok()) {
		std::cerr << "tf error: " << status.ToString() << "\n";
		return;
	}
	auto sqerr = outputs [0].scalar<float>();
std::cerr << "--- env_model_prediction_error: " << sqerr () << std::endl;

	status = session->Run(inputs, {}, {"env_model_train_step"}, &outputs);
	if (!status.ok()) {
		std::cerr << "tf error: " << status.ToString() << "\n";
		return;
	}
	
}

void BrainDiscreteDeepQ::predictNextStateAndReward (const ObservationSeqLimited& state, long action) {
	
	int inputSize = QuadrocopterBrain::observationSize + QuadrocopterBrain::numActions;
	Tensor env_model_input (DT_FLOAT, TensorShape({1, inputSize}));
	
	env_model_input.matrix<float>().setZero();
	fillTensor (state, env_model_input, 0);
	env_model_input.matrix<float>()(0, QuadrocopterBrain::observationSize + action) = 1.0;

	std::vector<std::pair<string, tensorflow::Tensor>> inputs = {
		{ "env_model_input", env_model_input }
	};

	std::vector<tensorflow::Tensor> outputs;

	Status status = session->Run(inputs, {"env_model_prediction"}, {}, &outputs);
	if (!status.ok()) {
		std::cout << "tf error: " << status.ToString() << "\n";
	}

	std::cerr << "------ prediction of new state, action: " << action << std::endl;
	printTensor<float>(env_model_input);
	std::cerr << "--- new state: " << action << std::endl;
	printTensor<float>(outputs [0]);
	
}
