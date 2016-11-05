//
//  QuadrocopterBrain.cpp
//  QuadrocopterBrain
//
//  Created by anton on 19/01/16.
//  Copyright © 2016 anton. All rights reserved.
//

#include <iostream>
#include <algorithm>

#include "QuadrocopterBrain.hpp"
#include "Lib.hpp"
#include "Tensors.hpp"

static std::mutex storeExperienceMutex;
static std::mutex countTrainErrorValuesMutex;

QuadrocopterBrain::QuadrocopterBrain () : QuadrocopterBrain::QuadrocopterBrain(std::shared_ptr<BrainAlgorithm> (new BrainDiscreteDeepQ ())) {
}

QuadrocopterBrain::QuadrocopterBrain (std::shared_ptr<BrainAlgorithm> algorithm) :
	actExecuted(1),
	trainExecuted(1)
{
	brain = algorithm;
	brain->setExplorationPeriod (1);
	experienceLow.setName("low");
//	experienceMid.setName("mid");
//	experienceHigh.setName("high");

	lastErrs.assign (100, 0);
}


//void QuadrocopterBrain::setObservation (const Observation& state) {
//	currentState.push(state);
//}

//static std::mutex actMutex;

long QuadrocopterBrain::act (const ObservationSeqLimited& currentState, double randomness) {
//	std::cerr << "--- act: " << actExecuted << std::endl;

//	std::lock_guard<std::mutex> lock(actMutex);
	
//	brain.setRandomness(randomness);
	long choosedAction = brain->control(currentState, randomness);

//	if (
//		actExecuted % trainEveryNth == 0 &&
//		experience.getExperience().size() > trainAfter
//	) {
//		brain.predictNextStateAndReward(currentState, 0);
//		brain.predictNextStateAndReward(currentState, 1);
//		brain.predictNextStateAndReward(currentState, 2);
//	}

//	train();

//	if (actExecuted % 400000 == 0) {
//		brain.setExplorationPeriod (50000);
//	}
	
	actExecuted++;
	
	return choosedAction;
}

void QuadrocopterBrain::actCont (const ObservationSeqLimited& state, std::vector<float>& action, double randomness)
{
	brain->control (state, action, randomness);
	actExecuted++;
}

void QuadrocopterBrain::actContLSTMWeak (
	const ObservationSeqLimited& state,
	const std::vector<float>& actorLstmStateC,
	const std::vector<float>& actorLstmStateH,
	double randomness,
	std::vector<float>& action//,
//	std::vector<float>& newLstmStateC,
//	std::vector<float>& newLstmStateH
) {
	brain->control (state, actorLstmStateC, actorLstmStateH, action, randomness);
	
	actExecuted++;
}

std::vector<int> counts = {
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};
std::vector<float> countsLimits = {
	500, 200, 100, 50 ,20, 10, 5, 2, 1, 0.5, 0.2, 0.1, 0.05, 0.02, 0.01
};
const int countsSize = 15;

bool QuadrocopterBrain::train () {

	int expSize =
//		experienceHigh.getSize() +
//		experienceMid.getSize() +
		experienceLow.getSize();

	if (expSize % 10 == 0 && expSize < trainAfter) {
		std::cerr << "--- exp: " << " ( "
//			<< experienceHigh.getSize() << " , "
//			<< experienceMid.getSize() << " , "
			<< experienceLow.getCurrentIndex () << " / "
			<< experienceLow.getSize() << " )" << std::endl;
	}

	if (
//		actExecuted % trainEveryNth != 0 ||
		expSize < trainAfter
	) {
		return false;
	}

	std::vector<const ExperienceItem*> minibatch;
	storeExperienceMutex.lock();
	getMinibatch(experienceLow, minibatch, minibatchSize);
	storeExperienceMutex.unlock();

//	getMinibatchWithProbabilities (
//		experienceHigh,
//		experienceLow,
//		0.5,
//		0.5,
//		minibatch,
//		minibatchSize
//	);

	float err = brain->trainOnMinibatch(minibatch);

trainExecuted++; return true;

//	std::vector<tensorflow::Tensor> outputTensors;
//	float err = brain.trainOnMinibatch(minibatch, outputTensors);
//	lastErrs.push_back (err * 0.01);
//	averageErr += lastErrs.back();
//	averageErr -= lastErrs.front();
//	lastErrs.pop_front();
//	if (err > 5 * averageErr) {
//		std::vector<float> itemsErrors;
//		getTensorVectorValues<float>(outputTensors [0], itemsErrors);
//		int maxIndex = Lib::argmaxabs<float>(itemsErrors);
//		std::cout << "--- err: " << err << " / " << itemsErrors[maxIndex] << " / " << averageErr << std::endl;
//		addMaxErrorExp(*minibatch[maxIndex]);
//	}

//	for (int i=0; i<10; i++) {
//		const ExperienceItem* minibatchRndItem = minibatch [ Lib::randInt(0, minibatchSize-1) ];
//		if (err > 2) {
//			experienceHigh.store(*minibatchRndItem);
//	//		experienceMid.remove(*minibatchRndItem);
//			experienceLow.remove(*minibatchRndItem);
//		}
//	//	else if (err > 1) {
//	//		experienceHigh.remove(*minibatchRndItem);
//	//		experienceMid.store(*minibatchRndItem);
//	//		experienceLow.remove(*minibatchRndItem);
//	//	}
//		else {
//			experienceHigh.remove(*minibatchRndItem);
//	//		experienceMid.remove(*minibatchRndItem);
//			experienceLow.store(*minibatchRndItem);
//		}
//	}

	countTrainErrorValuesMutex.lock();
	for (int i=0; i<countsSize; i++) {
		if (err > countsLimits [i]) {
			counts [i] ++;
			break;
		}
	}

	if (trainExecuted % 50 == 0) {
		std::cerr << "--- train: " << trainExecuted << " ( "
		//	<< experienceHigh.getSize() << " , "
		//	<< experienceMid.getSize() << " , "
			<< experienceLow.getCurrentIndex () << " / "
			<< experienceLow.getSize() << " ) :";
			
		for (int i=0; i<countsSize; i++) {
			std::cerr << " " << counts [i];
		}
		std::cerr << " err: " << err << " reward: " << allReward << std::endl;
		allReward = 0;
	}
	countTrainErrorValuesMutex.unlock();
	
	if (trainExecuted % 5000 == 0) {
		brain->saveGraphState (to_string(trainExecuted));
	}

	trainExecuted++;
	
//std::cerr << "--- train: " << trainExecuted++ << " : " << err << std::endl;
//		brain.trainEnvModel(exp);

	return true;
}

bool QuadrocopterBrain::trainOnMinibatch (std::vector<ExperienceItem*> minibatch) {

	float err = brain->trainOnMinibatch(minibatch);
	
	for (auto expItem : minibatch) {
		allReward += expItem->reward;
	}
	
	countTrainErrorValuesMutex.lock();
	for (int i=0; i<countsSize; i++) {
		if (err > countsLimits [i]) {
			counts [i] ++;
			break;
		}
	}

	if (trainExecuted % 50 == 0) {
		std::cerr << "--- train: " << trainExecuted << " ( "
			<< experienceLow.getCurrentIndex () << " / "
			<< experienceLow.getSize() << " ) :";
			
		for (int i=0; i<countsSize; i++) {
			std::cerr << " " << counts [i];
		}
		std::cerr << " err: " << err << " reward: " << allReward << std::endl;
		allReward = 0;
	}
	countTrainErrorValuesMutex.unlock();

	if (trainExecuted % 5000 == 0) {
		brain->saveGraphState (to_string(trainExecuted));
	}

	trainExecuted++;

	return true;
}

void QuadrocopterBrain::storeExperience (const ExperienceItem& expItem) {
	std::lock_guard<std::mutex> lock(storeExperienceMutex);
	
	allReward += expItem.reward;
	
//	if (expItem.rewardLambda > 0.1 || expItem.rewardLambda < -0.1) {
//		experienceHigh.store(expItem);
//		return;
//	} else
	if (storeExecuted++ % storeEveryNth != 0) {
		return;
	}
	
	experienceLow.store(expItem);
}

void QuadrocopterBrain::addMaxErrorExp (const ExperienceItem& expItem) {
	if (trainExecuted<100) return;
	std::lock_guard<std::mutex> lock (mtxMaxErrorExp);
	if (maxErrorExp.size () < 100) {
		maxErrorExp.push_back (expItem);
	}
}

bool QuadrocopterBrain::getMaxErrorExp (ExperienceItem& expItem) {
	std::lock_guard<std::mutex> lock (mtxMaxErrorExp);
	if (maxErrorExp.size () > 0) {
		expItem = maxErrorExp.front();
		maxErrorExp.pop_front ();
		if (maxErrorExp.size () < 80) {
			maxErrorExp.push_back (expItem);
		}
		return true;
	}
	return false;
}
