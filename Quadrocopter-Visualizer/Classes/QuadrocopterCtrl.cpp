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

const int observationSize = 4;

QuadrocopterCtrl::QuadrocopterCtrl (int id, Quadrocopter1D& simulationModel) : id(id), simulationModel(simulationModel) {
	prevState.resize(observationSize);
	nextState.resize(observationSize);
}

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

	reward = (prevX - nextX);// * (1.1 * exp (-nextR*nextR/100) + 0.9);
	if (reward > 0) reward *= 0.5;

//	if (nextX < 5) {
//		reward = exp (-nextX/40);
//	} else {
//		if (prevX > nextX) {
//	//		reward = 10/(nextX+1);
//			reward = exp (-nextX/40);
//		} else {
//	//		reward = -10/(prevX+1);
//			reward = -exp (-nextX/40);
//		}
//	}


//	sumReward += reward;
}

void QuadrocopterCtrl::readState (std::vector<float>& state) {

	float x = simulationModel.getPosition();
	float v = simulationModel.getVelocity();
	
	state [0] = x;
	state [1] = v;
	state [2] = x * x * 0.01;
	state [3] = v * v;
}

void QuadrocopterCtrl::act () {

	//getting prev state
	readState(prevState);
	
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

	reseted = false;
}

void QuadrocopterCtrl::storeExperience () {

	if (reseted) {
		return;
	}
	
	readState(nextState);
	
	calcReward();
	
	Quadrocopter2DBrain::storeQuadrocopterExperience(id, reward, action, prevState, nextState);

//	printf("%4.2f\n", pos);
}

void QuadrocopterCtrl::reset () {
	reseted = true;
	action = -1;
	timeReward = 300;
	simulationModel.setPosition(Lib::randFloat(-100, 100));
	simulationModel.setVelocity(Lib::randFloat(-10, 10));
}

double QuadrocopterCtrl::getReward () {
	return reward;
}

Quadrocopter1D& QuadrocopterCtrl::getModel () {
	return simulationModel;
}
