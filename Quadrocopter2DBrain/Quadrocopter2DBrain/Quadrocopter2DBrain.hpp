//
//  Quadrocopter2DBrain.hpp
//  Quadrocopter2DBrain
//
//  Created by anton on 29/04/16.
//  Copyright © 2016 anton. All rights reserved.
//

#ifndef Quadrocopter2DBrain_hpp
#define Quadrocopter2DBrain_hpp

#include <vector>

namespace Quadrocopter2DBrain {

	void initApiDiscreteDeepQ ();

//	void setQuadrocopterState(
//		int quadrocopterId,
//		const std::vector<float>& stateV
//	);

	long quadrocopterBrainAct(
		int quadrocopterId,
		const std::vector<float>& state
	);
	
	void quadrocopterBrainTrain ();

	void storeQuadrocopterExperience (
		int quadrocopterId,
		double reward,
		long action,
		const std::vector <float>& prevState,
		const std::vector <float>& nextState
	);

}

#endif /* Quadrocopter2DBrain_hpp */
