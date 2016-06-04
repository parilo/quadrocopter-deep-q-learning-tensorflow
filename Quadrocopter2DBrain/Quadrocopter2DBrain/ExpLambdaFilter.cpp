//
//  ExpLambdaFilter.cpp
//  Quadrocopter2DBrain
//
//  Created by anton on 03/04/16.
//  Copyright © 2016 anton. All rights reserved.
//

#include "ExpLambdaFilter.hpp"

void ExpLambdaFilter::storeExperience (const ExperienceItem& expItem) {
	expItems.push_back(expItem);
	realRewards.push_back(0);

	lambdaTheExp();

	if (expItems.size() >= filterLength) {
		ExperienceItem& expItem = expItems.front();
		expItem.rewardLambda = realRewards.front();
//std::cerr << "--- real reward: " <<  expItem.reward << std::endl;
		expTarget->storeExperience(expItem);
		expItems.pop_front();
		realRewards.pop_front();
	}
}

void ExpLambdaFilter::lambdaTheExp () {

	double backReward = expItems.back().reward;
	
	if (backReward > 0.1 || backReward < 0.1) {

		double l = 1 - lambda;
		auto realRewardBegin = realRewards.begin ();
		auto realRewardI = realRewards.end ();
		
		do {
			realRewardI--;
			double& r = *realRewardI;
			r += l * backReward;
			l *= lambda;
		} while (realRewardI != realRewardBegin);
		
	}
}

void ExpLambdaFilter::setExperienceTarget (QuadrocopterBrain* target) {
	expTarget = target;
}
