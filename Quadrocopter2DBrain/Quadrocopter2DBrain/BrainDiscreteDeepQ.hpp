//
//  BrainDiscreteDeepQ.hpp
//  QuadrocopterBrain
//
//  Created by anton on 17/01/16.
//  Copyright © 2016 anton. All rights reserved.
//

#ifndef BrainDiscreteDeepQ_hpp
#define BrainDiscreteDeepQ_hpp

#include "tensorflow/core/public/session.h"
//#include "tensorflow/core/public/env.h"

#include "Observation.hpp"
#include "ExperienceItem.hpp"
#include "BrainAlgorithm.hpp"

class BrainDiscreteDeepQ : public BrainAlgorithm {

public:

	BrainDiscreteDeepQ ();
	BrainDiscreteDeepQ (const std::string& tfGraphFilename);

	std::string getTFFilename ();

	/**
		@return index of action
	*/
	long control (const ObservationSeqLimited& ob, double randomness) override;
	
	void control (const ObservationSeqLimited& ob, std::vector<float>& action, double randomness) override {}
	
	void control (
		const ObservationSeqLimited& ob,
		const std::vector<float>& lstmStateC,
		const std::vector<float>& lstmStateH,
		std::vector<float>& action,
		double randomness//,
//		std::vector<float>& outLstmStateC,
//		std::vector<float>& outLstmStateH
	) override {}
	
//	/**
//		@return prediction error on choosed minibatch
//	*/
//	float train (
//		const std::vector<ExperienceItem>& experience,
//		std::vector<const ExperienceItem*>& minibatch
//	);
//	
//	float train (
//		const std::vector<ExperienceItem>& expLow,
//		const std::vector<ExperienceItem>& expMid,
//		const std::vector<ExperienceItem>& expHigh,
//		std::vector<const ExperienceItem*>& minibatch
//	);

	/**
		@return prediction error on choosed minibatch
	*/
	float trainOnMinibatch (std::vector<const ExperienceItem*> minibatch) override;
	float trainOnMinibatch (std::vector<const ExperienceItem*> minibatch, std::vector<tensorflow::Tensor>& outputTensors);

	float trainOnMinibatch (std::vector<ExperienceItem*> minibatch) override { return -1; }
	
	void predictNextStateAndReward (const ObservationSeqLimited& state, long action);
	void trainEnvModel (const std::vector<ExperienceItem>& experience);
//	void setRandomness (double randomness);
	void setExplorationPeriod (int explorationPeriod) override;

	void saveGraphState (const std::string fileSuffix) override;
	
protected:

	std::mutex saveGraphMutex;

	void initGraphState ();
	bool loadGraphState ();

	int actionsExecutedSoFar = 0;
	
	//linear annealing parameters
	std::atomic<int> explorationPeriod;
	int explorationPeriodInitial;
	double randomActionProbabilityInitial = 0.6;
	
	tensorflow::GraphDef graph_def;
	tensorflow::Session* session;
	
	double linearAnnealing(double randomActionProbabilityFinal);
	

};

#endif /* BrainDiscreteDeepQ_hpp */
