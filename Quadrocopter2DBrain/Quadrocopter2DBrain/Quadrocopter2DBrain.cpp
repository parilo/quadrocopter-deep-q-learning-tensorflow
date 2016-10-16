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
#include "DDPG.hpp"
#include "DDPG_LSTM.hpp"

using namespace std;

namespace Quadrocopter2DBrain {

	const int numOfQuadrocopters = 50;

	QuadrocopterBrain quadrocopterBrain (std::shared_ptr<BrainAlgorithm> (new DDPG ()));
//	QuadrocopterBrain quadrocopterBrain (std::shared_ptr<BrainAlgorithm> (new DDPG_LSTM ()));
//	QuadrocopterBrain quadrocopterBrain;
	vector<ObservationSeqLimited> prevObsSeq;
	vector<ObservationSeqLimited> nextObsSeq;
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

	void quadrocopterBrainActCont(
		int quadrocopterId,
		const std::vector<float>& state,
		std::vector<float>& action
	) {
		quadrocopterBrain.actCont (
			ObservationSeqLimited (state),
			action,
			randomnessOfQuadrocopter [quadrocopterId]
		);
	}

	void quadrocopterBrainActContLSTM(
		int quadrocopterId,
		const std::vector<float>& state,
		std::vector<float>& action
	) {
	
//		ObservationSeqLimited obs(states.size());
//		for (auto& s : states) {
//			obs.push(Observation(*s));
//		}
//		
//		obs.print();
		
//		std::cout << "q: " << quadrocopterId << std::endl;
		auto& seq = prevObsSeq [quadrocopterId];
		seq.push(Observation(state));
//		seq.print();
		
		quadrocopterBrain.actCont (
			seq,
			action,
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

	void storeQuadrocopterExperienceCont (
		int quadrocopterId,
		double reward,
		std::vector<float>& action,
		const std::vector <float>& prevState,
		const std::vector <float>& nextState
	) {
		ExperienceItem expItem (
			ObservationSeqLimited (prevState),
			ObservationSeqLimited (nextState),
			reward,
			action
		);
		
		quadrocopterBrain.storeExperience(expItem);
	}

	void storeQuadrocopterExperienceContLSTM (
		int quadrocopterId,
		double reward,
		std::vector<float>& action,
		const std::vector <float>& prevState,
		const std::vector<float>& prevLstmStateC,
		const std::vector<float>& prevLstmStateH,
		const std::vector <float>& nextState,
		const std::vector<float>& nextLstmStateC,
		const std::vector<float>& nextLstmStateH
	) {
		ExperienceItem expItem (
			ObservationSeqLimited (prevState),
			ObservationSeqLimited (nextState),
			reward,
			action,
			prevLstmStateC,
			prevLstmStateH,
			nextLstmStateC,
			nextLstmStateH
		);
		
		quadrocopterBrain.storeExperience(expItem);
	}

	void initApiDiscreteDeepQ () {
//		currStateSeqs.resize(numOfQuadrocopters);
//		prevStateSeqs.resize(numOfQuadrocopters);
		experienceFilters.resize(numOfQuadrocopters);
		randomnessOfQuadrocopter.clear ();
		Observation ob;
		ob.setZeros(QuadrocopterBrain::observationSize);

//		ObservationSeqLimited obs;
//		obs.setLimit(QuadrocopterBrain::observationsInSeq);
//		obs.initWith(ob);

		prevObsSeq.resize(numOfQuadrocopters);
		nextObsSeq.resize(numOfQuadrocopters);

		for (int i=0; i<numOfQuadrocopters; i++) {
		
			prevObsSeq [i].setLimit(QuadrocopterBrain::lstmStepsCount);
			prevObsSeq [i].initWith(ob);
			nextObsSeq [i].setLimit(QuadrocopterBrain::lstmStepsCount);
			nextObsSeq [i].initWith(ob);
		
			experienceFilters [i].setExperienceTarget(&quadrocopterBrain);
			
			if (i < 2) {
				randomnessOfQuadrocopter.push_back(0.1);
			} else
			if (i < 10) {
				randomnessOfQuadrocopter.push_back(0.05);
			}
			else {
				randomnessOfQuadrocopter.push_back(0.001);
			}
		}
	}

	bool quadrocopterBrainTrain () {
		return quadrocopterBrain.train();
	}
	
	bool getBigErrorExp (
		std::vector <float>& state
	) {
		ExperienceItem expItem;
		bool got = quadrocopterBrain.getMaxErrorExp(expItem);
		if (got) {
			state = expItem.prevStates.getObservation(0).data;
			return true;
		}
		return false;
	}

}

