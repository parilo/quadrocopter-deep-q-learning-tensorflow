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

class BrainDiscreteDeepQ {

public:

	BrainDiscreteDeepQ ();

	/**
		@return index of action
	*/
	long control (const ObservationSeqLimited& ob, double randomness);
	
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
	float trainOnMinibatch (std::vector<const ExperienceItem*> minibatch);
	
	void predictNextStateAndReward (const ObservationSeqLimited& state, long action);
	void trainEnvModel (const std::vector<ExperienceItem>& experience);
//	void setRandomness (double randomness);

	void saveGraphState (const std::string fileSuffix);
	
private:

	std::mutex saveGraphMutex;

	void initGraphState ();
	bool loadGraphState ();

	int actionsExecutedSoFar = 0;
	
	//linear annealing parameters
	int explorationPeriod = 100000;
	double randomActionProbabilityInitial = 1.0;
//	double randomActionProbabilityFinal = 0.1;
	
	tensorflow::GraphDef graph_def;
	tensorflow::Session* session;
	
	double linearAnnealing(double randomActionProbabilityFinal);
	
	
//	tensorflow::Tensor observations;
//	tensorflow::Tensor newObservations;
//	tensorflow::Tensor actionMasks;
//	tensorflow::Tensor newObservationsMasks;
//	tensorflow::Tensor rewards;
	

};

#endif /* BrainDiscreteDeepQ_hpp */
