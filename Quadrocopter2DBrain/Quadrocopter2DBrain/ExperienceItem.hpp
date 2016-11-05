//
//  ExperienceItem.hpp
//  QuadrocopterBrain
//
//  Created by anton on 30/01/16.
//  Copyright © 2016 anton. All rights reserved.
//

#ifndef ExperienceItem_hpp
#define ExperienceItem_hpp

#include "ObservationSeqLimited.hpp"

class ExperienceItem {

	public:

	ExperienceItem ();

	ExperienceItem (
		const ObservationSeqLimited& prevStates,
		const ObservationSeqLimited& nextStates,
		double reward,
		long action
	);

	ExperienceItem (
		const ObservationSeqLimited& prevStates,
		const ObservationSeqLimited& nextStates,
		double reward,
		const std::vector<float>& actionCont
	);

	ExperienceItem (
		const ObservationSeqLimited& prevStates,
		const ObservationSeqLimited& nextStates,
		const ObservationSeqLimited& reward,
		const ObservationSeqLimited& actionCont
	);
	
	int id;
	ObservationSeqLimited prevStates;
	ObservationSeqLimited nextStates;
	double reward;
	double rewardLambda;
	long action;
	std::vector<float> actionCont;

	// lstm
	ObservationSeqLimited lstmActions;
	ObservationSeqLimited lstmRewards;
	
	// lstm weak
	std::vector<float> actorLstmStateC;
	std::vector<float> actorLstmStateH;
	std::vector<float> criticLstmStateC;
	std::vector<float> criticLstmStateH;
	std::vector<float> targetActorLstmStateC;
	std::vector<float> targetActorLstmStateH;
	std::vector<float> targetCriticLstmStateC;
	std::vector<float> targetCriticLstmStateH;

};

#endif /* ExperienceItem_hpp */
