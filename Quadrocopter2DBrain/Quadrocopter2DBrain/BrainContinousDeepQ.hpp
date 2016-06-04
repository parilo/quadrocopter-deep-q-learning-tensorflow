//
//  BrainContinousDeepQ.hpp
//  QuadrocopterBrain
//
//  Created by anton on 13/02/16.
//  Copyright © 2016 anton. All rights reserved.
//

#ifndef BrainContinousDeepQ_hpp
#define BrainContinousDeepQ_hpp

#include "tensorflow/core/public/session.h"
//#include "tensorflow/core/public/env.h"

#include "Observation.hpp"
#include "ExperienceItem.hpp"

class BrainContinousDeepQ {

public:

	BrainContinousDeepQ ();

	/**
		Returns index of action
	*/
	long control (const Observation& ob);
	void controlContinuous (const Observation& ob, std::vector<float>& actionsScores);
	
	void train (const std::vector<ExperienceItem>& experience);
	
private:

	int actionsExecutedSoFar = 0;
	
	//linear annealing parameters
	int explorationPeriod = 30000;
	double randomActionProbabilityInitial = 1.0;
	double randomActionProbabilityFinal = 0.05;
	
	int numActions = 6;
	
	//train
	int minibatchSize = 32;
	
	tensorflow::Session* session;
	
	double linearAnnealing();
	

};

#endif /* BrainContinousDeepQ_hpp */
