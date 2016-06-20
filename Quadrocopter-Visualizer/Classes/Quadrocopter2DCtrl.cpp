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

static const int observationSize = 8;

Quadrocopter2DCtrl::Quadrocopter2DCtrl (int id, Quadrocopter2D& simulationModel) : id(id), simulationModel(simulationModel) {
	prevState.resize(observationSize);
	nextState.resize(observationSize);
}

void Quadrocopter2DCtrl::calcReward () {

	//награда за приближение к точке
	float prevR = prevState [2];
	float nextR = nextState [2];
	
//	reward = (50 * exp (-nextLengthSq/40) + 0.5) * dLengthSq;

//	if (nextR < 2) {
//		reward = 5 * exp (-nextR/40);
//	} else {
		reward = 5 * (prevR - nextR);
//	}
//CCLOG("--- reward: %f %f %f %f", prevR, nextR, (prevR - nextR), reward);

//	sumReward += reward;
}

void Quadrocopter2DCtrl::readState (std::vector<float>& state) {
	auto pos = simulationModel.getPosition();
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
	state [7] = w;

//	std::string str ("");
//	for (float& s : state) {
//		str += std::to_string(s) + " ";
//	}
//CCLOG("--- state: %s", str.c_str());
}

void Quadrocopter2DCtrl::act () {

	//getting prev state
//	simulationModel.getState(prevState);
	readState(prevState);
	
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
	
//	simulationModel.getState(nextState);
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
