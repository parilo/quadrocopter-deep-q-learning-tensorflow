//
//  Quadrocopter2DCtrl.cpp
//  Quadrocopter-Visualizer
//
//  Created by anton on 24/05/16.
//
//

#include "Quadrocopter2DCtrl.hpp"
#include "Quadrocopter2DBrain.hpp"
#include "Lib.h"

static const int observationSize = 7;

Quadrocopter2DCtrl::Quadrocopter2DCtrl (int id, Quadrocopter2D& simulationModel) : id(id), simulationModel(simulationModel) {
	prevState.resize(observationSize);
	nextState.resize(observationSize);
}

void Quadrocopter2DCtrl::calcReward () {

	//награда за приближение к точке
	float prevX = fabsf(prevState [0]);
	float prevY = fabsf(prevState [1]);
	float nextX = fabsf(nextState [0]);
	float nextY = fabsf(nextState [1]);
	float prevLengthSq = sqrtf(prevX * prevX + prevY * prevY);
	float nextLengthSq = sqrtf(nextX * nextX + nextY * nextY);
	if (prevLengthSq > nextLengthSq) {
//		reward = 10/(nextX+1);
		reward = 10 * exp (-nextLengthSq/40);
	} else if (prevLengthSq == nextLengthSq) {
		reward = 0;
	} else {
//		reward = -10/(prevX+1);
		reward = -10 * exp (-prevLengthSq/40);
	}


//	sumReward += reward;
}

void Quadrocopter2DCtrl::act () {

	//getting prev state
	simulationModel.getState(prevState);
	
	action = Quadrocopter2DBrain::quadrocopterBrainAct(id, prevState);
	switch (action) {
		case 0:
			simulationModel.setMotor1Power(2);
			simulationModel.setMotor2Power(0);
		break;
		
		case 1:
			simulationModel.setMotor1Power(0);
			simulationModel.setMotor2Power(2);
		break;
		
		case 2:
			simulationModel.setMotor1Power(10);
			simulationModel.setMotor2Power(10);
		break;
		
		case 3:
			simulationModel.setMotor1Power(0);
			simulationModel.setMotor2Power(0);
		break;
		
		case 4:
		break;
	}
}

void Quadrocopter2DCtrl::storeExperience () {

	if (action == -1) {
		return;
	}
	
	simulationModel.getState(nextState);
	
	calcReward();
	
	Quadrocopter2DBrain::storeQuadrocopterExperience(id, reward, action, prevState, nextState);

//	printf("%4.2f\n", pos);
}

void Quadrocopter2DCtrl::reset () {
	action = -1;
	simulationModel.setVelocity(b2Vec2(Lib::randFloat(-10, 10), Lib::randFloat(-10, 10)));
	simulationModel.setAngularVelocity(0);
	simulationModel.setCoords(b2Vec2(Lib::randFloat(-100, 100), Lib::randFloat(-100, 100)), Lib::randFloat(0, 360));
}

double Quadrocopter2DCtrl::getReward () {
	return reward;
}

Quadrocopter2D& Quadrocopter2DCtrl::getSimulationModel () {
	return simulationModel;
}
