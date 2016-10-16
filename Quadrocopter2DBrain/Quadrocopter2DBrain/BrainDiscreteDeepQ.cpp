//
//  BrainDiscreteDeepQ.cpp
//  QuadrocopterBrain
//
//  Created by anton on 17/01/16.
//  Copyright © 2016 anton. All rights reserved.
//

#include <fstream>

#ifdef __gnu_linux__
#include <unistd.h>
#endif

#include "BrainDiscreteDeepQ.hpp"
#include "Lib.hpp"
#include "Tensors.hpp"
#include "QuadrocopterBrain.hpp"

#define QUOTE(name) #name
#define STR(macro) QUOTE(macro)
#define GRAPHDIR STR(TF_GRAPH_DIR)
#define GRAPHSAVEDIR STR(TF_GRAPH_SAVE_DIR)

using namespace tensorflow;

std::string BrainDiscreteDeepQ::getTFFilename () {
//	return "graph.pb";
	return "graph2d.pb";
}

BrainDiscreteDeepQ::BrainDiscreteDeepQ () :
	BrainDiscreteDeepQ (getTFFilename ())
{}

BrainDiscreteDeepQ::BrainDiscreteDeepQ (const std::string& tfGraphFilename) {

	Status status = NewSession(SessionOptions(), &session);
	if (!status.ok()) {
		std::cerr << "tf error: " << status.ToString() << "\n";
	}

	// Read in the protobuf graph we exported
//	GraphDef graph_def;
	std::string dir (GRAPHDIR);
std::cout << "--- using tf graph: " << tfGraphFilename << std::endl;
	status = ReadBinaryProto(Env::Default(), dir + "/" + tfGraphFilename, &graph_def);
	if (!status.ok()) {
		std::cerr << "tf error: " << status.ToString() << "\n";
	}

	// Add the graph to the session
	status = session->Create(graph_def);
	if (!status.ok()) {
		std::cerr << "tf error: " << status.ToString() << "\n";
	}

	initGraphState();
	loadGraphState();

//	int minibatchSize = 1;//(int) minibatch.size();
//	int observationSize = QuadrocopterBrain::observationSize;
//	observations = Tensor (DT_FLOAT, TensorShape({minibatchSize, observationSize}));
//	newObservations = Tensor (DT_FLOAT, TensorShape({minibatchSize, observationSize}));
//	actionMasks = Tensor (DT_FLOAT, TensorShape({minibatchSize, QuadrocopterBrain::numActions}));
//	newObservationsMasks = Tensor (DT_FLOAT, TensorShape({minibatchSize, 1}));
//	rewards = Tensor (DT_FLOAT, TensorShape({minibatchSize, 1}));
//	
//	observations.matrix<float>().setZero ();
//	newObservations.matrix<float>().setZero ();
//	actionMasks.matrix<float>().setZero ();
//	newObservationsMasks.matrix<float>().setZero ();
//	actionMasks.matrix<float>().setZero();

}

void BrainDiscreteDeepQ::initGraphState () {
	
	std::vector<tensorflow::Tensor> outputs;
	Status status = session->Run({}, {}, {"init_all_vars_op"}, &outputs);
	if (!status.ok()) {
		std::cerr << "tf error: " << status.ToString() << "\n";
		return;
	}
}

void BrainDiscreteDeepQ::saveGraphState (const std::string fileSuffix) {
	std::lock_guard<std::mutex> lock1 (saveGraphMutex);

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
	std::string dir (GRAPHSAVEDIR);
	std::fstream output(dir + "/graph-state-" + fileSuffix, std::ios::out | std::ios::binary);
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

bool BrainDiscreteDeepQ::loadGraphState () {

	std::string dir (GRAPHSAVEDIR);
	std::fstream input(dir + "/graph-state", std::ios::in | std::ios::binary);
	
	if (!input.good ()) return false;
	
	std::vector<std::pair<string, tensorflow::Tensor>> variablesValues;
	std::vector<string> restoreOps;
	
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

	return true;
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
	if (explorationPeriod>0) {
		return randomActionProbabilityInitial - ((explorationPeriodInitial-explorationPeriod) * (randomActionProbabilityInitial - randomActionProbabilityFinal)) / (explorationPeriodInitial);
	} else {
		return randomActionProbabilityFinal;
	}
//	if (actionsExecutedSoFar >= explorationPeriod)
//		return randomActionProbabilityFinal;
//	else
//		return randomActionProbabilityInitial - (actionsExecutedSoFar * (randomActionProbabilityInitial - randomActionProbabilityFinal)) / (explorationPeriod);
}

/**
Given observation returns the action that should be chosen using
DeepQ learning strategy. Does not backprop.

управление
*/
long BrainDiscreteDeepQ::control (const ObservationSeqLimited& obs, double randomness) {
	
	double explorationP = linearAnnealing (randomness);
//std::cerr <<"--- explorationP: " << explorationP << " act: " << actionsExecutedSoFar <<  std::endl;
	long actionIndex;
	if (Lib::randDouble(0.0, 1.0) < explorationP) {
		actionIndex = Lib::randInt(0, QuadrocopterBrain::numActions-1);
	} else {
	
		Tensor observationSeq (DT_FLOAT, TensorShape({1, obs.getSize()}));
		
		fillTensor (obs, observationSeq, 0);
		
//		std::cerr << "--- BrainDiscreteDeepQ::control" << std::endl;
//		printTensor(observationSeq);

		std::vector<std::pair<string, tensorflow::Tensor>> inputs = {
			{ "taking_action/observation", observationSeq }
		};

		// The session will initialize the outputs
		std::vector<tensorflow::Tensor> outputs;

//		Status status = session->Run(inputs, {"taking_action/action_scores"}, {}, &outputs);
//		if (!status.ok()) {
//			std::cout << "tf error: " << status.ToString() << "\n";
//		}
		
//		std::cerr << "--- action scores: " << std::endl;
//		printTensor(outputs [0]);
		
//		auto as = outputs [0].matrix<float>();
//		actionsScores.clear();
//		for (int i=0; i<numActions; i++) {
//			actionsScores.push_back(as (0, i));
//		}

		//getting index of max scored action
		auto status = session->Run(
			inputs, {
//				"taking_action/action_scores",
				"taking_action/predicted_actions"
			}, {}, &outputs
		);
		if (!status.ok()) {
			std::cout << "tf error: " << status.ToString() << "\n";
		}
		
//		std::cerr << "--- action scores: " << std::endl;
//		printTensor<float>(outputs [0]);
		auto action = outputs [0].scalar<int>();
		actionIndex = action ();
//		std::cerr << "--- action: " << actionIndex << std::endl;
	}

	actionsExecutedSoFar++;
	if (explorationPeriod>0) explorationPeriod--;
	
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
	std::vector<tensorflow::Tensor> outputTensors;
	return trainOnMinibatch(minibatch, outputTensors);
}

float BrainDiscreteDeepQ::trainOnMinibatch (std::vector<const ExperienceItem*> minibatch, std::vector<tensorflow::Tensor>& outputTensors) {
	std::lock_guard<std::mutex> lock (saveGraphMutex);

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
	
//	std::cerr << "observations: " << std:: endl;
//	printTensor<float>(observations);
//	std::cerr << "newObservations: " << std:: endl;
//	printTensor<float>(newObservations);
//	std::cerr << "actionMasks: " << std:: endl;
//	printTensor<float>(actionMasks);
//	std::cerr << "rewards: " << std:: endl;
//	printTensor<float>(rewards);
//	std::cerr << "newObservationsMasks: " << std:: endl;
//	printTensor<float>(newObservationsMasks);
	
	std::vector<std::pair<string, tensorflow::Tensor>> inputs = {
		{ "taking_action/observation", observations },
		{ "q_value_precition/action_mask", actionMasks },
		{ "estimating_future_rewards/rewards", rewards },
		{ "estimating_future_rewards/next_observation_mask", newObservationsMasks },
		{ "estimating_future_rewards/next_observation", newObservations }
	};
	
	auto status = session->Run(inputs, {
		"q_value_precition/temp_diff",
		"q_value_precition/prediction_error"
	}, {
		"q_value_precition/train_op",
		"target_network_update/target_network_update"
	}, &outputTensors);
	if (!status.ok()) {
		std::cerr << "tf error: " << status.ToString() << "\n";
		return 0;
	}
//std::cerr << "--- train outputs: " << outputs [0].DebugString() << std::endl;
//	printTensor<float>(outputTensors [0]);
//	std::vector<float> v;
//	getTensorValues<float>(outputs [0], v);
	auto predictionError = outputTensors [1].scalar<float>();
	float err = predictionError ();

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

void BrainDiscreteDeepQ::setExplorationPeriod (int explorationPeriod) {
	this->explorationPeriod = explorationPeriod;
	this->explorationPeriodInitial = explorationPeriod;
}
