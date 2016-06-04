//
//  ExpLambdaFilter.hpp
//  Quadrocopter2DBrain
//
//  Created by anton on 03/04/16.
//  Copyright © 2016 anton. All rights reserved.
//

#ifndef ExpLambdaFilter_hpp
#define ExpLambdaFilter_hpp

#include "QuadrocopterBrain.hpp"
#include <deque>

class ExpLambdaFilter {

public:

	void storeExperience (const ExperienceItem& expItem);
	void setExperienceTarget (QuadrocopterBrain* target);

private:

	const static int filterLength = 20;
	double lambda = 0.3;
	std::deque<ExperienceItem> expItems;
	std::deque<double> realRewards;

	QuadrocopterBrain* expTarget;

	void lambdaTheExp ();

};

#endif /* ExpLambdaFilter_hpp */
