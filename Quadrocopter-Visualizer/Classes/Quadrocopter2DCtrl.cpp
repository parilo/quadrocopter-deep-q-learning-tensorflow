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

static const int observationSize = 50;

Quadrocopter2DCtrl::Quadrocopter2DCtrl (int id, Quadrocopter2D& simulationModel) : id(id), simulationModel(simulationModel)
{
	simulationModel.setTarget(b2Vec2(0, 0));
	prevState.resize(observationSize);
	nextState.resize(observationSize);
}

void Quadrocopter2DCtrl::calcReward () {

	//награда за приближение к точке
	float prevR = prevState [2];
	float nextR = nextState [2];
	
//	if (prevR > 5 && nextR <= 5) {
//		reward = 1;
////	} else if (prevR <= 5 && nextR > 5) {
////		reward = -1;
//	} else {
//		reward = 0;
//	}
	
	
//	reward = (50 * exp (-nextLengthSq/40) + 0.5) * dLengthSq;

//	if (nextR < 2) {
//		reward = 5 * exp (-nextR/40);
//	} else {
		reward = (prevR - nextR);
//	}
//CCLOG("--- reward: %f %f %f %f", prevR, nextR, (prevR - nextR), reward);

//	sumReward += reward;
}

void Quadrocopter2DCtrl::readState (std::vector<float>& state) {
//CCLOG ("--- readState:  %f %f %f %f", simulationModel.getPosition().x, simulationModel.getPosition().y, simulationModel.getTarget().x, simulationModel.getTarget().y);
	auto pos = simulationModel.getPosition() - simulationModel.getTarget();
	float angle = simulationModel.getRotation();
	auto v = simulationModel.getVelocity();
	float w = simulationModel.getAngularVelocity();
	float r = sqrtf(pos.x * pos.x + pos.y * pos.y);
	state [0] = 10 * pos.x / r;
	state [1] = 10 * pos.y / r;
	state [2] = r;
	state [3] = 10 * sinf(angle);
	state [4] = 10 * cosf(angle);
	state [5] = v.x;
	state [6] = v.y;
	state [7] = w * 5;
	
	state [8] = pos.x;
	state [9] = pos.y;
	state [10] = pos.x * pos.x * 0.01;
	state [11] = pos.y * pos.y * 0.01;
	state [12] = state [2] * state [2] * 0.01;
	state [13] = state [3] * state [3] * 0.1;
	state [14] = state [4] * state [4] * 0.1;
	state [15] = state [5] * state [5];
	state [16] = state [6] * state [6];
	state [17] = state [7] * state [7] / 5;

	int sensorI = 18;
	for (auto& s : simulationModel.getSensors()) {
		state [sensorI] = s;
		sensorI++;
	}

//39.271 -7.60199 46.1449
//39.9983 0.373619
//-8.29327
//1.90346
//-2.89494e-07
//10 10 10 10 10 10 10 10 10 10 10 10 10 10 10 10 10 10 10 10 10 10 10 10 10 10 10 10 10 10 10 10
}

void Quadrocopter2DCtrl::act () {

	//getting prev state
	readState(prevState);
	
	action = Quadrocopter2DBrain::quadrocopterBrainAct(id, prevState);
	switch (action) {
		case 0:
			simulationModel.setMotor1Power(0.2);
			simulationModel.setMotor2Power(0);
		break;
		
		case 1:
			simulationModel.setMotor1Power(0);
			simulationModel.setMotor2Power(0.2);
		break;
		
		case 2:
			simulationModel.setMotor1Power(7);
			simulationModel.setMotor2Power(7);
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
	
	readState(nextState);
	
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

QuadrocopterModel2DIFace& Quadrocopter2DCtrl::getModel () {
	return simulationModel;
}

void Quadrocopter2DCtrl::resetAction () {
	action = -1;
}
