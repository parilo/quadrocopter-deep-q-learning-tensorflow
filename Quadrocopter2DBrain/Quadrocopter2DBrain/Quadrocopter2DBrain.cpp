//
//  Quadrocopter2DBrain.cpp
//  Quadrocopter2DBrain
//
//  Created by anton on 29/04/16.
//  Copyright © 2016 anton. All rights reserved.
//

#include "Quadrocopter2DBrain.hpp"

#include <iostream>
#include <math.h>
#include <stdio.h>
#include <vector>
#include <string>

#include "QuadrocopterBrain.hpp"
#include "ExpLambdaFilter.hpp"

using namespace std;

namespace Quadrocopter2DBrain {

	const int numOfQuadrocopters = 50;

	QuadrocopterBrain quadrocopterBrain;
//	vector<ObservationSeqLimited> currStateSeqs;
//	vector<ObservationSeqLimited> prevStateSeqs;
	vector<ExpLambdaFilter> experienceFilters; //one filter for each quadrocopter;
	vector<double> randomnessOfQuadrocopter;

//	void setQuadrocopterState(
//		int quadrocopterId,
//		const std::vector<float>& stateV
//	) {
//
//	//	std::vector<float> stateV;
//	//	stateV.resize(QuadrocopterBrain::observationSize);
//	//	stateV.assign(stateData, stateData + QuadrocopterBrain::observationSize);
//		Observation state (stateV);
//
////		ObservationSeqLimited& currStateSeq = currStateSeqs [quadrocopterId];
////		prevStateSeqs [quadrocopterId] = currStateSeq;
////		currStateSeq.push(state);
//	}

	long quadrocopterBrainAct(
		int quadrocopterId,
		const std::vector<float>& state
	) {
		
		return quadrocopterBrain.act (
			ObservationSeqLimited (state),
			randomnessOfQuadrocopter [quadrocopterId]
		);
	}

	void storeQuadrocopterExperience (
		int quadrocopterId,
		double reward,
		long action,
		const std::vector <float>& prevState,
		const std::vector <float>& nextState
	) {
		
		ExperienceItem expItem (
			ObservationSeqLimited (prevState),
			ObservationSeqLimited (nextState),
			reward,
			action
		);

//		experienceFilters [quadrocopterId].storeExperience(expItem);
		quadrocopterBrain.storeExperience(expItem);
	}

	void initApiDiscreteDeepQ () {
//		currStateSeqs.resize(numOfQuadrocopters);
//		prevStateSeqs.resize(numOfQuadrocopters);
		experienceFilters.resize(numOfQuadrocopters);
		randomnessOfQuadrocopter.clear ();
//		Observation ob;
//		ob.setZeros(QuadrocopterBrain::observationSize);
//		
//		ObservationSeqLimited obs;
//		obs.setLimit(QuadrocopterBrain::observationsInSeq);
//		obs.initWith(ob);

		for (int i=0; i<numOfQuadrocopters; i++) {
//			currStateSeqs [i] = obs;
//			prevStateSeqs [i] = obs;
			experienceFilters [i].setExperienceTarget(&quadrocopterBrain);
			
			if (i < 4) {
				randomnessOfQuadrocopter.push_back(0.5);
			} else
			if (i < 10) {
				randomnessOfQuadrocopter.push_back(0.2);
			}
			else {
				randomnessOfQuadrocopter.push_back(0.05);
			}
		}
	}

	void quadrocopterBrainTrain () {
		quadrocopterBrain.train();
	}

}

