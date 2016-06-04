//
//  ExperienceFilter.hpp
//  Quadrocopter2DBrain
//
//  Created by anton on 26/03/16.
//  Copyright © 2016 anton. All rights reserved.
//

#ifndef ExperienceFilter_hpp
#define ExperienceFilter_hpp

#include "QuadrocopterBrain.hpp"
#include <deque>

class ExperienceFilter {

public:

	void storeExperience (const ExperienceItem& expItem);
	void setExperienceTarget (QuadrocopterBrain* target);

private:

	const static int maxExpCount = 20;
	double rewardSum = 0;
	std::deque<ExperienceItem> expItems;

	QuadrocopterBrain* expTarget;

	void checkExpIsReadyToPush ();
	void pushExpToTarget (double resultReward);

};

#endif /* ExperienceFilter_hpp */
