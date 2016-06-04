//
//  ExperienceFilter.cpp
//  Quadrocopter2DBrain
//
//  Created by anton on 26/03/16.
//  Copyright © 2016 anton. All rights reserved.
//

#include "ExperienceFilter.hpp"
#include <iostream>

void ExperienceFilter::storeExperience (const ExperienceItem& expItem) {
//	rewardSum += expItem.reward;
	expItems.push_back(expItem);
	checkExpIsReadyToPush();
}

void ExperienceFilter::checkExpIsReadyToPush () {
	if (expItems.size() >= maxExpCount) {
		pushExpToTarget(expItems.front().reward * 5.0);
	}
}

void ExperienceFilter::pushExpToTarget (double resultReward) {
	
	while (!expItems.empty()) {
		ExperienceItem& expItem = expItems.front();
		expItem.reward = resultReward;
		expTarget->storeExperience(expItem);
		expItems.pop_front();
	}
//	rewardSum = 0;
}

void ExperienceFilter::setExperienceTarget (QuadrocopterBrain* target) {
	expTarget = target;
}
