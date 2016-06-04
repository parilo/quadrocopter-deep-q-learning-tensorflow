//
//  ApiDiscreteDeepQ.cpp
//  Quadrocopter2DBrain
//
//  Created by anton on 20/03/16.
//  Copyright © 2016 anton. All rights reserved.
//

#include "ApiDiscreteDeepQ.hpp"

#include <iostream>
#include <math.h>
#include <stdio.h>
#include <vector>
#include <string>

#include "Unity/IUnityGraphics.h"

#include "Main.h"
#include "QuadrocopterBrain.hpp"
#include "ExpLambdaFilter.hpp"

using namespace std;

const int numOfQuadrocopters = 50;

QuadrocopterBrain quadrocopterBrain;
vector<ObservationSeqLimited> currStateSeqs;
vector<ObservationSeqLimited> prevStateSeqs;
vector<ExpLambdaFilter> experienceFilters; //one filter for each quadrocopter;
vector<double> randomnessOfQuadrocopter;

extern "C" void UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API SetQuadrocopterState(
	int quadrocopterId,
	float* stateData
) {

	std::vector<float> stateV;
	stateV.resize(QuadrocopterBrain::observationSize);
	stateV.assign(stateData, stateData + QuadrocopterBrain::observationSize);
	Observation state (stateV);

	ObservationSeqLimited& currStateSeq = currStateSeqs [quadrocopterId];
	prevStateSeqs [quadrocopterId] = currStateSeq;
	currStateSeq.push(state);
}

extern "C" long UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API QuadrocopterBrainAct(
	int quadrocopterId
) {
	return quadrocopterBrain.act (
		currStateSeqs [quadrocopterId],
		randomnessOfQuadrocopter [quadrocopterId]
	);
}

extern "C" void UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API StoreQuadrocopterExperience (
	int quadrocopterId,
	double reward,
	long action
) {
	
	ExperienceItem expItem (
		prevStateSeqs [quadrocopterId],
		currStateSeqs [quadrocopterId],
		reward,
		action
	);

	experienceFilters [quadrocopterId].storeExperience(expItem);
//	quadrocopterBrain.storeExperience(expItem);
}

void initApiDiscreteDeepQ () {
	currStateSeqs.resize(numOfQuadrocopters);
	prevStateSeqs.resize(numOfQuadrocopters);
	experienceFilters.resize(numOfQuadrocopters);

	Observation ob;
	ob.setZeros(QuadrocopterBrain::observationSize);
	
	ObservationSeqLimited obs;
	obs.setLimit(QuadrocopterBrain::observationsInSeq);
	obs.initWith(ob);

	for (int i=0; i<numOfQuadrocopters; i++) {
		currStateSeqs [i] = obs;
		prevStateSeqs [i] = obs;
		experienceFilters [i].setExperienceTarget(&quadrocopterBrain);
		
//		if (i < 1) {
//			randomnessOfQuadrocopter.push_back(0.9);
//		} else if (i < 10) {
//			randomnessOfQuadrocopter.push_back(0.5);
//		} else {
			randomnessOfQuadrocopter.push_back(0.05);
//		}
	}
}



//extern "C" void UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API StoreQuadrocopterExperienceContinous (
//	float* prevStateData,
//	float* nextStateData,
//	double reward,
//	float* actionsScores
//) {
//
////	std::vector<float> prevStateV;
////	std::vector<float> nextStateV;
////	std::vector<float> actionsScoresV;
////	
////	prevStateV.resize(5);
////	nextStateV.resize(5);
////	actionsScoresV.resize(6);
////	
////	prevStateV.assign(prevStateData, prevStateData + 5);
////	nextStateV.assign(nextStateData, nextStateData + 5);
////	actionsScoresV.assign(actionsScores, actionsScores + 6);
////
////	Observation prevState (prevStateV);
////	Observation nextState (nextStateV);
////	
////	ExperienceItem expItem (
////		prevState,
////		nextState,
////		reward,
////		actionsScoresV
////	);
////
////	quadrocopterBrain.storeExperience(expItem);
//}

//extern "C" void UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API GetQuadrocopterActionsScores(float* actionsScoredVals) {
//	auto actionsScores = quadrocopterBrain.getActionsScores();
////	cerr << "--- get actions scores: " << endl;
//	for (int i=0; i<6; i++) {
//		actionsScoredVals [i] = actionsScores [i];
////		cerr << "--- " << actionsScoredVals [i] << endl;
//	}
//}
