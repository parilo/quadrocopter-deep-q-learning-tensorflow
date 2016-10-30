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

//		1 -> 0.2

		reward = rewardWeight * (prevR - nextR);// * (1.1 * exp (-nextR*nextR/100) + 0.9);
		if (reward > 0) reward *= 0.5;
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

void Quadrocopter2DCtrl::onSimulationStep (int stepIndex) {

	auto& pos = simulationModel.getPosition();
	if (fabsf(pos.x) > 200 || fabsf(pos.y) > 200) {
		reset();
		return;
	}
	
	if (simulationModel.isCollided()) {
		reset();
		return;
	}
	
}

void Quadrocopter2DCtrl::act () {

	//getting prev state
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
			simulationModel.setMotor1Power(8);
			simulationModel.setMotor2Power(6);
		break;
		
		case 3:
			simulationModel.setMotor1Power(6);
			simulationModel.setMotor2Power(8);
		break;
		
		case 4:
			simulationModel.setMotor1Power(14);
			simulationModel.setMotor2Power(12);
		break;
		
		case 5:
			simulationModel.setMotor1Power(12);
			simulationModel.setMotor2Power(14);
		break;
		
		case 6:
			simulationModel.setMotor1Power(12);
			simulationModel.setMotor2Power(12);
		break;
		
		case 7:
			simulationModel.setMotor1Power(0);
			simulationModel.setMotor2Power(0);
		break;
		
		case 8:
		break;
	}
	
//	time += 0.01;
//	if (time >= 10) time = 1;

	reseted = false;
}

void Quadrocopter2DCtrl::storeExperience () {

	if (reseted) {
		return;
	}
	
	readState(nextState);
	
	calcReward();
	
	Quadrocopter2DBrain::storeQuadrocopterExperience(id, reward, action, prevState, nextState);
}

void Quadrocopter2DCtrl::reset () {
//if (id == 0)
//std::cout << "--- reset" << std::endl;
	reseted = true;
	action = -1;
//	time = 1;

	while (true) {
		b2Vec2 p (Lib::randFloat(-100, 100), Lib::randFloat(-100, 100));
		if (!simulationModel.isPointInsideObstacles(p)) {
			simulationModel.setCoords(p, Lib::randFloat(0, 360));
			break;
		}
	}

	simulationModel.setVelocity(b2Vec2(Lib::randFloat(-10, 10), Lib::randFloat(-10, 10)));
	simulationModel.setAngularVelocity(0);
}

double Quadrocopter2DCtrl::getReward () {
	return reward;
}

QuadrocopterModel2DIFace& Quadrocopter2DCtrl::getModel () {
	return simulationModel;
}

void Quadrocopter2DCtrl::resetAction () {
//if (id == 0)
//std::cout << "--- reset action" << std::endl;
	reseted = true;
	action = -1;
}

void Quadrocopter2DCtrl::onTrainStep (int trainStepIndex) {
//	if (id != 0) return;
//	if (trainStepIndex % 50 == 0) {
//	
//		gravity += 0.0002;
//		if (gravity > 5) gravity = 5;
//		simulationModel.getWorld().world->SetGravity(b2Vec2(0, gravity));
//		std::cout << "--- gravity: " << gravity << std::endl;
//	
////		linearDamping -= 0.00002;
////		angularDamping -= 0.00002;
////		rewardWeight -= 0.00004;
////		if (linearDamping < 0) linearDamping = 0;
////		if (angularDamping < 0.1) angularDamping = 0.1;
////		if (rewardWeight < 0.2) rewardWeight = 0.2;
////		simulationModel.setLinearDamping(linearDamping);
////		simulationModel.setAngularDamping(angularDamping);
////if (id == 0) std::cout << "--- model damping: " << linearDamping << " " << angularDamping << " " << rewardWeight << std::endl;
//	}
}
