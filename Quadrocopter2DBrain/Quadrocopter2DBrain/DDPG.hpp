//
//  DDPG.hpp
//  Quadrocopter2DBrain
//
//  Created by anton on 13/08/16.
//  Copyright © 2016 anton. All rights reserved.
//

#ifndef DDPG_hpp
#define DDPG_hpp

#include <random>

#include "BrainDiscreteDeepQ.hpp"

class DDPG : public BrainDiscreteDeepQ {
public:

	DDPG ();

	void control (const ObservationSeqLimited& ob, std::vector<float>& action, double randomness) override;

	/**
		@return prediction error on choosed minibatch
	*/
	float trainOnMinibatch (std::vector<const ExperienceItem*> minibatch) override;

private:

  std::default_random_engine randomGenerator;

};

#endif /* DDPG_hpp */
