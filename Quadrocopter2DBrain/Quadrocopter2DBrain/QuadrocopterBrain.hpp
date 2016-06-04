//
//  QuadrocopterBrain.hpp
//  QuadrocopterBrain
//
//  Created by anton on 19/01/16.
//  Copyright © 2016 anton. All rights reserved.
//

#ifndef QuadrocopterBrain_hpp
#define QuadrocopterBrain_hpp

#include <deque>

#include "BrainDiscreteDeepQ.hpp"
#include "ExperienceItem.hpp"
#include "ExperienceBank.hpp"

class QuadrocopterBrain {

public:
	
	static const int observationsInSeq = 1;
	static const int observationSize = 7;
	static const int numActions = 5;

	QuadrocopterBrain ();
	
	long act (const ObservationSeqLimited& state, double randomness);
	
	void storeExperience (const ExperienceItem& expItem);
	
	void train ();
	
private:

	BrainDiscreteDeepQ brain;
	ExperienceBank experienceLow;
//	ExperienceBank experienceMid;
//	ExperienceBank experienceHigh;

	std::atomic<long> actExecuted;
	std::atomic<long> trainExecuted;
	long storeExecuted = 0;
	static const int storeEveryNth = 5;
	static const int trainEveryNth = 15; //act
	static const int trainCount = 1;
	static const int trainAfter = 5000; //stored exp

	//train
	static const int minibatchSize = 64;
	constexpr static float probHigh = 0.55;
	constexpr static float probMid = 0.3;
	constexpr static float probLow = 0.15;

};

#endif /* QuadrocopterBrain_hpp */
