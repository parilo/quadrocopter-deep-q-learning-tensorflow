//
//  QuadrocopterSimulator.hpp
//  Quadrocopter-Visualizer
//
//  Created by anton on 07/05/16.
//
//

#ifndef QuadrocopterSimulator_hpp
#define QuadrocopterSimulator_hpp

#include <functional>
#include <vector>
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
//#include "unistd.h"

#include "Simulation.hpp"
#include "QuadrocopterCtrl.hpp"
#include "QuadrocopterDiscreteCtrl.hpp"
#include "main.h"
#include "Quadrocopter2DBrain.hpp"
#include "Quadrocopter2DCtrl.hpp"
#include "ConsumerProducerSyncronizer.hpp"

template <typename QuadrocopterCtrlType, typename QuadrocopterType>
class QuadrocopterSimulatorTmpl {
public:

	QuadrocopterSimulatorTmpl ();
	void init ();
//	void update ();
	void reset ();
	void startActWorkers ();
	void startTrainWorkers ();
	void stop ();
	void setSimulationUpdateCallback (std::function<void ()> callback);
	void join ();
	
	QuadrocopterCtrlType& getQuadrocopterCtrl (int index);

private:

	SimulationTmpl<QuadrocopterType> simulation;
	std::atomic<int> stepIndex;
	std::vector<QuadrocopterCtrlType> qcopterCtrls;
	
	typedef std::function<void ()> QCopterWorker;
	typedef std::function<void ()> QCopterTrainWorker;
	QCopterWorker qcopterMainWorker;
	std::vector<QCopterWorker> qcopterWorkers;
	QCopterTrainWorker trainWorker;
	
	std::atomic<bool> allowThreadsWork;
	std::thread qcopterMainWorkerThread;
	std::vector<std::thread> qcopterWorkersThreads;
	std::vector<std::thread> trainWorkerThreads;

	ConsumerProducerSyncronizer qcopterActSync;

	
	std::function<void ()> simulationUpdateCallback;

};


template <typename QuadrocopterCtrlType, typename QuadrocopterType>
QuadrocopterSimulatorTmpl<QuadrocopterCtrlType, QuadrocopterType>::QuadrocopterSimulatorTmpl () :
	simulation(quadrocoptersCount),
	stepIndex(0),
	simulationUpdateCallback([](){}),
	allowThreadsWork(true),
	qcopterActSync(quadrocoptersWorkerThreads)
	{}

template <typename QuadrocopterCtrlType, typename QuadrocopterType>
void QuadrocopterSimulatorTmpl<QuadrocopterCtrlType, QuadrocopterType>::init()
{

	Quadrocopter2DBrain::initApiDiscreteDeepQ ();
	
	for (int i=0; i<quadrocoptersCount; i++) {
		qcopterCtrls.push_back(QuadrocopterCtrlType(i, simulation.getQuadrocopter(i)));
	}

	reset ();

	int qcoptersLeft = quadrocoptersCount;
	int qcoptersPerThread = quadrocoptersCount / quadrocoptersWorkerThreads;
	int beginIndex = 0;
	for (int i=0; i<quadrocoptersWorkerThreads; i++) {

		int endIndex;
		if (i == quadrocoptersWorkerThreads-1) {
			endIndex = beginIndex + qcoptersLeft;
		} else {
			endIndex = beginIndex + qcoptersPerThread;
		}
		
		int workerIndex = i;
		qcopterWorkers.push_back([this, beginIndex, endIndex, workerIndex] () {
			while (allowThreadsWork) {

				for (int i=beginIndex; i<endIndex; i++) {
					qcopterCtrls [i].storeExperience();
					qcopterCtrls [i].act();
				}

				qcopterActSync.reportProducerDone(workerIndex);
				qcopterActSync.waitConsumer(workerIndex);
			}
		});
		
		beginIndex += qcoptersPerThread;
		qcoptersLeft -= qcoptersPerThread;
	}
	
	qcopterMainWorker = [this] () {

        while (allowThreadsWork) {
			qcopterActSync.waitProducers();

			if (stepIndex % exerciseLengthInSteps == 0) {
				reset ();
			}
		
			simulation.step();
			simulationUpdateCallback ();
			stepIndex++;

			qcopterActSync.reportConsumerDone();
        }
	};
	
	trainWorker = [this] () {
		while (allowThreadsWork) {
			Quadrocopter2DBrain::quadrocopterBrainTrain();
		}
	};
}

template <typename QuadrocopterCtrlType, typename QuadrocopterType>
void QuadrocopterSimulatorTmpl<QuadrocopterCtrlType, QuadrocopterType>::startActWorkers () {
	qcopterMainWorkerThread = std::thread (qcopterMainWorker);
	for (auto& worker : qcopterWorkers) {
		qcopterWorkersThreads.emplace_back(worker);
	}
}

template <typename QuadrocopterCtrlType, typename QuadrocopterType>
void QuadrocopterSimulatorTmpl<QuadrocopterCtrlType, QuadrocopterType>::startTrainWorkers () {
	for (int i=0; i<quadrocoptersTrainWorkerThreads; i++) {
		trainWorkerThreads.emplace_back(trainWorker);
	}
}

template <typename QuadrocopterCtrlType, typename QuadrocopterType>
void QuadrocopterSimulatorTmpl<QuadrocopterCtrlType, QuadrocopterType>::stop () {
	allowThreadsWork = false;
	for (auto& t : qcopterWorkersThreads) {
		t.join ();
	}
	for (auto& t : trainWorkerThreads) {
		t.join ();
	}
	qcopterMainWorkerThread.join();
	
	qcopterWorkersThreads.clear();
	trainWorkerThreads.clear();
}

template <typename QuadrocopterCtrlType, typename QuadrocopterType>
void QuadrocopterSimulatorTmpl<QuadrocopterCtrlType, QuadrocopterType>::join () {
	qcopterMainWorkerThread.join();
}

//template <typename QuadrocopterCtrlType, typename QuadrocopterType>
//void QuadrocopterSimulatorTmpl<QuadrocopterCtrlType, QuadrocopterType>::invokeQCoptersWorkers () {
//	std::vector<std::thread> threads;
//	int qcoptersLeft = quadrocoptersCount;
//	int qcoptersPerThread = quadrocoptersCount / quadrocoptersWorkerThreads;
//	int qcoptersThreadI = 0;
//	for (int i=0; i<quadrocoptersWorkerThreads; i++) {
//		QCopterWorker& w = qcopterWorkers [i];
//		if (i == quadrocoptersWorkerThreads-1) {
//			threads.emplace_back(w, qcoptersThreadI, qcoptersLeft);
//		} else {
//			threads.emplace_back(w, qcoptersThreadI, qcoptersPerThread);
//			qcoptersThreadI += qcoptersPerThread;
//			qcoptersLeft -= qcoptersPerThread;
//		}
//	}
//	
//	for (std::thread& t : threads) {
//		t.join();
//	}
//}

//template <typename QuadrocopterCtrlType, typename QuadrocopterType>
//void QuadrocopterSimulatorTmpl<QuadrocopterCtrlType, QuadrocopterType>::update() {
////	CCLOG ("--- update: ->%f<-", delta);
//
//	if (stepIndex % exerciseLengthInSteps == 0) {
//		reset ();
//	}
//
////	if (stepIndex % 2 == 0) {
//		invokeQCoptersWorkers();
////	}
//
//	simulation.step();
//
//	stepIndex++;
//	
//}

template <typename QuadrocopterCtrlType, typename QuadrocopterType>
void QuadrocopterSimulatorTmpl<QuadrocopterCtrlType, QuadrocopterType>::reset () {
	for (QuadrocopterCtrlType& qCtrl : qcopterCtrls) {
		qCtrl.reset();
	}
}

template <typename QuadrocopterCtrlType, typename QuadrocopterType>
QuadrocopterCtrlType& QuadrocopterSimulatorTmpl<QuadrocopterCtrlType, QuadrocopterType>::getQuadrocopterCtrl (int index) {
	return qcopterCtrls [index];
}

template <typename QuadrocopterCtrlType, typename QuadrocopterType>
void QuadrocopterSimulatorTmpl<QuadrocopterCtrlType, QuadrocopterType>::setSimulationUpdateCallback (std::function<void ()> callback) {
	simulationUpdateCallback = callback;
}


typedef QuadrocopterSimulatorTmpl<QuadrocopterCtrl, Quadrocopter1D> QuadrocopterSimulator;
typedef QuadrocopterSimulatorTmpl<Quadrocopter2DCtrl, Quadrocopter2D> QuadrocopterSimulator2D;
typedef QuadrocopterSimulatorTmpl<QuadrocopterDiscreteCtrl, QuadrocopterDiscrete> QuadrocopterSimulatorDiscrete;

#endif /* QuadrocopterSimulator_hpp */
