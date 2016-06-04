//
//  QuadrocopterCtrl.cpp
//  Quadrocopter-Visualizer
//
//  Created by anton on 29/04/16.
//
//

#include "QuadrocopterCtrl.hpp"
#include "Quadrocopter2DBrain.hpp"
#include "Lib.h"

QuadrocopterCtrl::QuadrocopterCtrl (int id, Quadrocopter1D& simulationModel) : id(id), simulationModel(simulationModel) {
	prevState.resize(2);
	nextState.resize(2);
}

const float XLimit = 2.0;

void QuadrocopterCtrl::calcReward () {
//	reward = Lib::randFloat(0, 100);

//	//рассчитывать награду за остановку
//	//рассчитывать награду за координату
//	//суммровать награды
//	//таким образом максимальная награда дается за остановку
//	//в нужном месте
//	
//	if (prevState [1] > 0.01 && nextState [1] < 0.01) {
//		if (timeReward > 0) {
//			reward = 25 * (timeReward * 1.0 / 300.0) * exp (- pow (nextState [0], 2)/150.0);
//		} else {
//			reward = 0;
//		}
//	} else {
//		reward = 0;
//	}
	
//	//награда за нахождение внутри рамок
//	float prevX = prevState [0];
//	float nextX = nextState [0];
//	if (
//		(prevX > XLimit && nextX < XLimit) ||
//		(prevX < -XLimit && nextX > -XLimit)
//	) {
//		reward = 10;
//	} else if (
//		(prevX < XLimit && nextX > XLimit) ||
//		(prevX > -XLimit && nextX < -XLimit)
//	) {
//		reward = -10;
//	} else {
//		reward = 0;
//	}


	//награда за приближение к точке
	float prevX = fabsf(prevState [0]);
	float nextX = fabsf(nextState [0]);
	if (prevX > nextX) {
//		reward = 10/(nextX+1);
		reward = 10 * exp (-nextX/40);
	} else {
//		reward = -10/(prevX+1);
		reward = -10 * exp (-prevX/40);
	}


//	sumReward += reward;
}

void QuadrocopterCtrl::act () {

	//getting prev state
	prevState [0] = simulationModel.getPosition();
	prevState [1] = simulationModel.getVelocity();
	
	action = Quadrocopter2DBrain::quadrocopterBrainAct(id, prevState);
	switch (action) {
		case 0:
			simulationModel.setMotorPower(20);
		break;
		
		case 1:
			simulationModel.setMotorPower(-20);
		break;
		
		case 2:
			simulationModel.setMotorPower(0);
		break;
	}
	
	timeReward--;
}

void QuadrocopterCtrl::storeExperience () {

	if (action == -1) {
		return;
	}
	
	nextState [0] = simulationModel.getPosition();
	nextState [1] = simulationModel.getVelocity();
	
	calcReward();
	
	Quadrocopter2DBrain::storeQuadrocopterExperience(id, reward, action, prevState, nextState);

//	printf("%4.2f\n", pos);
}

void QuadrocopterCtrl::reset () {
	action = -1;
	timeReward = 300;
	simulationModel.setPosition(Lib::randFloat(-100, 100));
	simulationModel.setVelocity(Lib::randFloat(-10, 10));
}

double QuadrocopterCtrl::getReward () {
	return reward;
}

Quadrocopter1D& QuadrocopterCtrl::getSimulationModel () {
	return simulationModel;
}
