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
#include "QuadrocopterDiscrete2DCtrl.hpp"
#include "ConsumerProducerSyncronizer.hpp"
#include "WorldDiscrete.hpp"
#include "QuadrocopterDiscrete2D.hpp"
#include "QuadrocopterContActionCtrl.hpp"
#include "Quadrocopter2DContActionCtrl.hpp"
#include "Lib.h"
#include "Quadrocopter2DContActionPIDCtrl.hpp"
#include "Quadrocopter2DContActionPIDLSTMCtrl.hpp"

template <
	typename WorldType,
	typename QuadrocopterCtrlType,
	typename QuadrocopterType,
	typename ObstacleType
	>
class QuadrocopterSimulatorTmpl {
public:

	QuadrocopterSimulatorTmpl ();
	void init ();
	void reset ();
	void startActWorkers ();
	void startTrainWorkers ();
	void stop ();
	void setSimulationUpdateCallback (std::function<void ()> callback);
	void join ();
	
	QuadrocopterCtrlType& getQuadrocopterCtrl (int index);
	ObstacleType& getObstacle (int index) {
		return simulation.getWorld().getObstacle (index);
	}
	
	WorldType& getWorld () {
		return simulation.getWorld();
	}
	
	void setCollideListener (typename WorldType::CollideListener listener) {
		collideListener = listener;
	}

private:

	int trainStepIndex = 1;
	std::mutex mtxChangeQCopterParams;
	std::mutex mtxBox2DQCopterWorkers;

	SimulationTmpl<WorldType, QuadrocopterType> simulation;
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
	typename WorldType::CollideListener collideListener;

};

template <
	typename WorldType,
	typename QuadrocopterCtrlType,
	typename QuadrocopterType,
	typename ObstacleType
	>
QuadrocopterSimulatorTmpl<WorldType, QuadrocopterCtrlType, QuadrocopterType, ObstacleType>::QuadrocopterSimulatorTmpl () :
	simulation(quadrocoptersCount, obstaclesCount),
	stepIndex(0),
	simulationUpdateCallback([](){}),
	allowThreadsWork(true),
	qcopterActSync(quadrocoptersWorkerThreads)
	{}

template <
	typename WorldType,
	typename QuadrocopterCtrlType,
	typename QuadrocopterType,
	typename ObstacleType
	>
void QuadrocopterSimulatorTmpl<WorldType, QuadrocopterCtrlType, QuadrocopterType, ObstacleType>::init()
{

	Quadrocopter2DBrain::initApiDiscreteDeepQ ();
	
	for (int i=0; i<quadrocoptersCount; i++) {
		qcopterCtrls.emplace_back(i, simulation.getQuadrocopter(i));
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
					QuadrocopterCtrlType& q = qcopterCtrls [i];
					q.storeExperience();
					q.act();
					mtxBox2DQCopterWorkers.lock();
					q.onSimulationStep (stepIndex);
					mtxBox2DQCopterWorkers.unlock();
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

			if (Lib::randFloat(0, 100) < 1) {
				qcopterCtrls [Lib::randInt(0, quadrocoptersCount-1)].reset ();
			}

//			if (stepIndex % 2 == 0) {
//				std::vector<float> state;
//				if (Quadrocopter2DBrain::getBigErrorExp(state)) {
//					qcopterCtrls [Lib::randInt(0, 10)].setState (state);
//				}
//			}

			std::lock_guard<std::mutex> lock (mtxChangeQCopterParams);
			simulation.step();
			simulationUpdateCallback ();
			stepIndex++;
			
//			for (auto& q : qcopterCtrls) {
//				q.onSimulationStep (stepIndex);
//			}

			qcopterActSync.reportConsumerDone();
        }
	};
	
	trainWorker = [this] () {
		while (allowThreadsWork) {
			if (Quadrocopter2DBrain::quadrocopterBrainTrain()) {
				std::lock_guard<std::mutex> lock (mtxChangeQCopterParams);
				for (auto& q : qcopterCtrls) {
					q.onTrainStep (trainStepIndex);
				}
				trainStepIndex++;
			}
		}
	};
	
	simulation.getWorld ().setCollideListener ([this](ObstacleType& o, QuadrocopterType& q){
		if (collideListener) collideListener (o ,q);
	});
}

template <
	typename WorldType,
	typename QuadrocopterCtrlType,
	typename QuadrocopterType,
	typename ObstacleType
	>
void QuadrocopterSimulatorTmpl<WorldType, QuadrocopterCtrlType, QuadrocopterType, ObstacleType>::startActWorkers () {
	qcopterMainWorkerThread = std::thread (qcopterMainWorker);
	for (auto& worker : qcopterWorkers) {
		qcopterWorkersThreads.emplace_back(worker);
	}
}

template <
	typename WorldType,
	typename QuadrocopterCtrlType,
	typename QuadrocopterType,
	typename ObstacleType
	>
void QuadrocopterSimulatorTmpl<WorldType, QuadrocopterCtrlType, QuadrocopterType, ObstacleType>::startTrainWorkers () {
	for (int i=0; i<quadrocoptersTrainWorkerThreads; i++) {
		trainWorkerThreads.emplace_back(trainWorker);
	}
}

template <
	typename WorldType,
	typename QuadrocopterCtrlType,
	typename QuadrocopterType,
	typename ObstacleType
	>
void QuadrocopterSimulatorTmpl<WorldType, QuadrocopterCtrlType, QuadrocopterType, ObstacleType>::stop () {
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

template <
	typename WorldType,
	typename QuadrocopterCtrlType,
	typename QuadrocopterType,
	typename ObstacleType
	>
void QuadrocopterSimulatorTmpl<WorldType, QuadrocopterCtrlType, QuadrocopterType, ObstacleType>::join () {
	qcopterMainWorkerThread.join();
}

template <
	typename WorldType,
	typename QuadrocopterCtrlType,
	typename QuadrocopterType,
	typename ObstacleType
	>
void QuadrocopterSimulatorTmpl<WorldType, QuadrocopterCtrlType, QuadrocopterType, ObstacleType>::reset () {
	for (auto& qCtrl : qcopterCtrls) {
		qCtrl.reset();
	}
	for (auto& o : simulation.getWorld().getObstacles()) {
		o.reset ();
	}
}

template <
	typename WorldType,
	typename QuadrocopterCtrlType,
	typename QuadrocopterType,
	typename ObstacleType
	>
QuadrocopterCtrlType& QuadrocopterSimulatorTmpl<WorldType, QuadrocopterCtrlType, QuadrocopterType, ObstacleType>::getQuadrocopterCtrl (int index) {
	return qcopterCtrls [index];
}

template <
	typename WorldType,
	typename QuadrocopterCtrlType,
	typename QuadrocopterType,
	typename ObstacleType
	>
void QuadrocopterSimulatorTmpl<WorldType, QuadrocopterCtrlType, QuadrocopterType, ObstacleType>::setSimulationUpdateCallback (std::function<void ()> callback) {
	simulationUpdateCallback = callback;
}


typedef QuadrocopterSimulatorTmpl<World1D, QuadrocopterCtrl, Quadrocopter1D, Obstacle1D> QuadrocopterSimulator;
typedef QuadrocopterSimulatorTmpl<WorldDiscrete1D, QuadrocopterDiscreteCtrl, QuadrocopterDiscrete, ObstacleDiscrete1D> QuadrocopterSimulatorDiscrete;

typedef QuadrocopterSimulatorTmpl<World2D, Quadrocopter2DCtrl, Quadrocopter2D, Obstacle2D> QuadrocopterSimulator2D;
typedef QuadrocopterSimulatorTmpl<WorldDiscrete2D, QuadrocopterDiscrete2DCtrl, QuadrocopterDiscrete2D, ObstacleDiscrete2D> QuadrocopterSimulatorDiscrete2D;

typedef QuadrocopterSimulatorTmpl<World1D, QuadrocopterContActionCtrl, Quadrocopter1D, Obstacle1D> QuadrocopterSimulatorCont1D;
typedef QuadrocopterSimulatorTmpl<World2D, Quadrocopter2DContActionCtrl, Quadrocopter2D, Obstacle2D> QuadrocopterSimulatorCont2D;
typedef QuadrocopterSimulatorTmpl<World2D, Quadrocopter2DContActionPIDCtrl, Quadrocopter2D, Obstacle2D> QuadrocopterSimulatorContPID2D;
typedef QuadrocopterSimulatorTmpl<World2D, Quadrocopter2DContActionPIDLSTMCtrl, Quadrocopter2D, Obstacle2D> QuadrocopterSimulatorContPIDLSTM2D;

#endif /* QuadrocopterSimulator_hpp */
