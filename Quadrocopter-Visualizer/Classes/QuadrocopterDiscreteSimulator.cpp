//
//  QuadrocopterDiscreteSimulator.cpp
//  Quadrocopter-Visualizer
//
//  Created by anton on 22/05/16.
//
//

#include "QuadrocopterDiscreteSimulator.hpp"

#include <iostream>
#include <thread>

#include "QuadrocopterSimulator.hpp"
#include "Quadrocopter2DBrain.hpp"

QuadrocopterDiscreteSimulator::QuadrocopterDiscreteSimulator () {}

void QuadrocopterDiscreteSimulator::init()
{

	Quadrocopter2DBrain::initApiDiscreteDeepQ ();
	
	for (int i=0; i<quadrocoptersCount; i++) {
		qcopterCtrls.push_back(QuadrocopterDiscreteCtrl(i));
	}

	reset ();
	
	for (int i=0; i<quadrocoptersWorkerThreads; i++) {
		qcopterWorkers.push_back([this] (int beginIndex, int count) {
			for (int i=beginIndex; i<beginIndex+count; i++) {
				qcopterCtrls [i].storeExperience();
				qcopterCtrls [i].act();
//				double r = qcopterCtrls [i].getReward();
//				if (r > 0.001 || r < -0.001) {
//					std::cout << "--- reward: " << r << ", pos: " << qcopterCtrls [i].getPosition() << std::endl;
//				}
			}
		});
	}
}

void QuadrocopterDiscreteSimulator::invokeQCoptersWorkers () {
	std::vector<std::thread> threads;
	int qcoptersLeft = quadrocoptersCount;
	int qcoptersPerThread = quadrocoptersCount / quadrocoptersWorkerThreads;
	int qcoptersThreadI = 0;
	for (int i=0; i<quadrocoptersWorkerThreads; i++) {
		QCopterWorker& w = qcopterWorkers [i];
		if (i == quadrocoptersWorkerThreads-1) {
			threads.emplace_back(w, qcoptersThreadI, qcoptersLeft);
		} else {
			threads.emplace_back(w, qcoptersThreadI, qcoptersPerThread);
			qcoptersThreadI += qcoptersPerThread;
			qcoptersLeft -= qcoptersPerThread;
		}
	}
	
	for (std::thread& t : threads) {
		t.join();
	}
}

void QuadrocopterDiscreteSimulator::update() {
//	CCLOG ("--- update: ->%f<-", delta);

	if (stepIndex % exerciseLengthInSteps == 0) {
		reset ();
	}

//	if (stepIndex % 4 == 0) {
		invokeQCoptersWorkers();
//	}

	stepIndex++;
	
}

void QuadrocopterDiscreteSimulator::reset () {
	for (QuadrocopterDiscreteCtrl& qCtrl : qcopterCtrls) {
		qCtrl.reset();
	}
}

float QuadrocopterDiscreteSimulator::getQuadrocopterPosition (int index) {
	return qcopterCtrls [index].getPosition();
}
