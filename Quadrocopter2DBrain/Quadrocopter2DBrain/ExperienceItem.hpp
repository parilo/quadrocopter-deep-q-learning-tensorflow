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
	
	int id;
	ObservationSeqLimited prevStates;
	ObservationSeqLimited nextStates;
	double reward;
	double rewardLambda;
	long action;
	std::vector<float> actionCont;

};

#endif /* ExperienceItem_hpp */
