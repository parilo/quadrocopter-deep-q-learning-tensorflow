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
#include <list>

#include "BrainDiscreteDeepQ.hpp"
#include "ExperienceItem.hpp"
#include "ExperienceBank.hpp"

class QuadrocopterBrain {

public:
	
	static const int observationsInSeq = 1;

	//2d
	static const int observationSize = 50;
	static const int numActions = 5;

	//1d
//	static const int observationSize = 2;
//	static const int numActions = 3;

	QuadrocopterBrain ();
	
	long act (const ObservationSeqLimited& state, double randomness);
	
	void storeExperience (const ExperienceItem& expItem);
	
	void train ();
	
	bool getMaxErrorExp (ExperienceItem& expItem);
	
private:

	BrainDiscreteDeepQ brain;
	ExperienceBank experienceLow;
//	ExperienceBank experienceMid;
//	ExperienceBank experienceHigh;

	std::atomic<long> actExecuted;
	std::atomic<long> trainExecuted;
	long storeExecuted = 0;
	double allReward = 0;
	
	double averageErr = 0;
	std::list<double> lastErrs;
	std::list<ExperienceItem> maxErrorExp;
	std::mutex mtxMaxErrorExp;
	
	static const int storeEveryNth = 5;
	static const int trainEveryNth = 1000; //act
	static const int trainCount = 1;
	static const int trainAfter = 20000; //stored exp

	//train
	static const int minibatchSize = 32;
	constexpr static float probHigh = 0.55;
	constexpr static float probMid = 0.3;
	constexpr static float probLow = 0.15;
	
	void addMaxErrorExp (const ExperienceItem& expItem);

};

#endif /* QuadrocopterBrain_hpp */
