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
#include "DDPG_LSTM_Weak.hpp"

using namespace std;

namespace Quadrocopter2DBrain {

	const int numOfQuadrocopters = 64;

  QuadrocopterBrain quadrocopterBrain (std::shared_ptr<BrainAlgorithm> (new DDPG ()));
//	QuadrocopterBrain quadrocopterBrain (std::shared_ptr<BrainAlgorithm> (new DDPG_LSTM ()));
//	QuadrocopterBrain quadrocopterBrain (std::shared_ptr<BrainAlgorithm> (new DDPG_LSTM_Weak ()));
	
	const bool useLstmWeak = false;
//	const bool useLstmWeak = true;
	
	//lstm
	vector<ObservationSeqLimited> prevObsSeq;
	vector<ObservationSeqLimited> nextObsSeq;
	vector<ObservationSeqLimited> lstmActions;
	vector<ObservationSeqLimited> lstmRewards;
	
	vector<ExpLambdaFilter> experienceFilters; //one filter for each quadrocopter;
	vector<double> randomnessOfQuadrocopter;
	
	vector<ExperienceItem> lstmWeakExperience; // by copter
	vector<ExperienceItem*> lstmWeakExperienceMinibatch;

	vector<ObservationSeqLimited> prevObsMLPSeq;
	vector<ObservationSeqLimited> nextObsMLPSeq;


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
    if (quadrocopterId == 0) {
      std::cout << "val: " << action[0] << " : " << action[1] << std::endl;
    }
  }

	void quadrocopterBrainActContLSTMWeak(
		int quadrocopterId,
		const std::vector<float>& state,
		std::vector<float>& action
	) {
		auto& exp = lstmWeakExperience [quadrocopterId];
		
//if (quadrocopterId == 14) {
//	std::cout << "state C" << std::endl;
//	for (auto& s : exp.actorLstmStateC) {
//		std::cout << s << " ";
//	}
//	std::cout << "state H" << std::endl;
//	for (auto& s : exp.actorLstmStateH) {
//		std::cout << s << " ";
//	}
//	std::cout << std::endl << std::endl;
//}

		quadrocopterBrain.actContLSTMWeak(state, exp.actorLstmStateC, exp.actorLstmStateH, randomnessOfQuadrocopter [quadrocopterId], action);
	}
	
	void quadrocopterBrainActContMLPSeq(
		int quadrocopterId,
		const std::vector<float>& state,
		std::vector<float>& action
	) {
		auto& seq = prevObsMLPSeq [quadrocopterId];
		seq.push(state);

//if (quadrocopterId == 14) {
//	std::cout << "seq" << std::endl;
//	seq.print();
//}

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

	void storeQuadrocopterExperienceContLSTMWeak (
		int quadrocopterId,
		double reward,
		std::vector<float>& action,
		const std::vector <float>& prevState,
		const std::vector <float>& nextState
	) {
		auto& exp = lstmWeakExperience [quadrocopterId];
		
		exp.reward = reward;
		exp.rewardLambda = reward;
		exp.actionCont = action;
		exp.prevStates = ObservationSeqLimited (prevState);
		exp.nextStates = ObservationSeqLimited (nextState);
//std::cout << "--- size: " << exp.prevStates.getSize() << " / " << lstmWeakExperienceMinibatch[quadrocopterId]->prevStates.getSize() << std::endl;
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

	void storeQuadrocopterExperienceContLSTM (
		int quadrocopterId,
		double reward,
		std::vector<float>& action,
		const std::vector <float>& prevState,
		const std::vector <float>& nextState
	) {

		auto& pSeq = prevObsSeq [quadrocopterId];
		auto& nSeq = nextObsSeq [quadrocopterId];
		auto& a = lstmActions [quadrocopterId];
		auto& r = lstmRewards [quadrocopterId];
		
		nSeq.push(Observation(nextState));
		a.push(Observation(action));
		r.push(Observation(std::vector<float> (1, reward)));

		ExperienceItem expItem (
			pSeq,
			nSeq,
			r,
			a
		);
		expItem.reward = reward;
		expItem.rewardLambda = reward;
		
		quadrocopterBrain.storeExperience(expItem);
	}

	void storeQuadrocopterExperienceContMLPSeq (
		int quadrocopterId,
		double reward,
		std::vector<float>& action,
		const std::vector <float>& prevState,
		const std::vector <float>& nextState
	) {
		auto& pSeq = prevObsMLPSeq [quadrocopterId];
		auto& nSeq = nextObsMLPSeq [quadrocopterId];
		
		nSeq.push (nextState);

//if (reward == -1)
//if (quadrocopterId == 14) {
//	std::cout << "r " << reward << std::endl;
//	std::cout << "prev" << std::endl;
//	pSeq.print();
//	std::cout << "next" << std::endl;
//	nSeq.print();
//}

		quadrocopterBrain.storeExperience(ExperienceItem (
			pSeq,
			nSeq,
			reward,
			action
		));
	}

	void initApiDiscreteDeepQ () {
//		currStateSeqs.resize(numOfQuadrocopters);
//		prevStateSeqs.resize(numOfQuadrocopters);
		experienceFilters.resize(numOfQuadrocopters);
		randomnessOfQuadrocopter.clear ();
		Observation ob;
		ob.setZeros(QuadrocopterBrain::observationSize);
		Observation action;
		action.setZeros(QuadrocopterBrain::contActionSize);
		Observation reward;
		reward.setZeros(1);

//		ObservationSeqLimited obs;
//		obs.setLimit(QuadrocopterBrain::observationsInSeq);
//		obs.initWith(ob);

		prevObsSeq.resize(numOfQuadrocopters);
		nextObsSeq.resize(numOfQuadrocopters);
		lstmActions.resize(numOfQuadrocopters);
		lstmRewards.resize(numOfQuadrocopters);
		
		prevObsMLPSeq.resize(numOfQuadrocopters);
		nextObsMLPSeq.resize(numOfQuadrocopters);
		
		lstmWeakExperience.resize(numOfQuadrocopters);
		lstmWeakExperienceMinibatch.clear();
		
		for (int i=0; i<numOfQuadrocopters; i++) {
		
			lstmWeakExperience [i].actorLstmStateC.assign(QuadrocopterBrain::lstmStateSize, 0);
			lstmWeakExperience [i].actorLstmStateH.assign(QuadrocopterBrain::lstmStateSize, 0);
			lstmWeakExperience [i].criticLstmStateC.assign(QuadrocopterBrain::lstmStateSize, 0);
			lstmWeakExperience [i].criticLstmStateH.assign(QuadrocopterBrain::lstmStateSize, 0);
			lstmWeakExperience [i].targetActorLstmStateC.assign(QuadrocopterBrain::lstmStateSize, 0);
			lstmWeakExperience [i].targetActorLstmStateH.assign(QuadrocopterBrain::lstmStateSize, 0);
			lstmWeakExperience [i].targetCriticLstmStateC.assign(QuadrocopterBrain::lstmStateSize, 0);
			lstmWeakExperience [i].targetCriticLstmStateH.assign(QuadrocopterBrain::lstmStateSize, 0);

			lstmWeakExperienceMinibatch.push_back(&lstmWeakExperience [i]);
		
			prevObsSeq [i].setLimit(QuadrocopterBrain::lstmStepsCount);
			prevObsSeq [i].initWith(ob);
			nextObsSeq [i].setLimit(QuadrocopterBrain::lstmStepsCount);
			nextObsSeq [i].initWith(ob);
			lstmActions [i].setLimit(QuadrocopterBrain::lstmStepsCount);
			lstmActions [i].initWith(action);
			lstmRewards [i].setLimit(QuadrocopterBrain::lstmStepsCount);
			lstmRewards [i].initWith(reward);

			prevObsMLPSeq [i].setLimit(QuadrocopterBrain::mlpSeqSize);
			prevObsMLPSeq [i].initWith(ob);
			nextObsMLPSeq [i].setLimit(QuadrocopterBrain::mlpSeqSize);
			nextObsMLPSeq [i].initWith(ob);
		
			experienceFilters [i].setExperienceTarget(&quadrocopterBrain);
			
			if (i < 2) {
				randomnessOfQuadrocopter.push_back(0.1);
			} else
			if (i < 10) {
				randomnessOfQuadrocopter.push_back(0.05);
			}
			else {
				randomnessOfQuadrocopter.push_back(0.0);
			}
		}
	}

	bool quadrocopterBrainTrain () {
		if (useLstmWeak) {
			return quadrocopterBrain.trainOnMinibatch(lstmWeakExperienceMinibatch);
		} else {
			return quadrocopterBrain.train();
		}
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

	void resetQuadrocopterLSTMWeak (
		int quadrocopterId,
		const std::vector <float>& copterState
	) {
		auto& exp = lstmWeakExperience [quadrocopterId];
		exp.actorLstmStateC.assign(QuadrocopterBrain::lstmStateSize, 0);
		exp.actorLstmStateH.assign(QuadrocopterBrain::lstmStateSize, 0);
		exp.criticLstmStateC.assign(QuadrocopterBrain::lstmStateSize, 0);
		exp.criticLstmStateH.assign(QuadrocopterBrain::lstmStateSize, 0);
		exp.targetActorLstmStateC.assign(QuadrocopterBrain::lstmStateSize, 0);
		exp.targetActorLstmStateH.assign(QuadrocopterBrain::lstmStateSize, 0);
		exp.targetCriticLstmStateC.assign(QuadrocopterBrain::lstmStateSize, 0);
		exp.targetCriticLstmStateH.assign(QuadrocopterBrain::lstmStateSize, 0);
		
		exp.reward = 0;
		exp.rewardLambda = 0;
		exp.actionCont.assign(QuadrocopterBrain::contActionSize, 0);
		exp.prevStates = ObservationSeqLimited (copterState);
		exp.nextStates = ObservationSeqLimited (copterState);
	}
	
	void resetQuadrocopterMLPSeq (
		int quadrocopterId,
		const std::vector <float>& copterState
	) {
		Observation ob;
		ob.setZeros(QuadrocopterBrain::observationSize);
		
		prevObsMLPSeq [quadrocopterId].initWith(ob);
		nextObsMLPSeq [quadrocopterId].initWith(ob);
	}
	
	void resetQuadrocopterLSTM (
		int quadrocopterId
	) {
		Observation ob;
		ob.setZeros(QuadrocopterBrain::observationSize);
		Observation action;
		action.setZeros(QuadrocopterBrain::contActionSize);
		Observation reward;
		reward.setZeros(1);

		prevObsSeq [quadrocopterId].initWith(ob);
		nextObsSeq [quadrocopterId].initWith(ob);
		lstmActions [quadrocopterId].initWith(action);
		lstmRewards [quadrocopterId].initWith(reward);
	}

}

