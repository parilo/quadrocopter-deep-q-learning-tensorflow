//
//  QuadrocopterDiscrete2DCtrl.cpp
//  Quadrocopter-Visualizer
//
//  Created by anton on 18/06/16.
//
//

#include "QuadrocopterDiscrete2DCtrl.hpp"
#include "Quadrocopter2DBrain.hpp"
#include "Lib.h"

static const int observationSize = 50;

QuadrocopterDiscrete2DCtrl::QuadrocopterDiscrete2DCtrl (int id, QuadrocopterDiscrete2D& simulationModel) :
	id(id),
	model(simulationModel)
{
	model.setTarget(b2Vec2(0, 0));
	model.setId(id);
	prevState.resize(observationSize);
	nextState.resize(observationSize);
}

void QuadrocopterDiscrete2DCtrl::calcReward () {

	//награда за приближение к точке
//	float prevX = prevState [0];
//	float prevY = prevState [1];
//	float nextX = nextState [0];
//	float nextY = nextState [1];
//	float prevLengthSq = sqrtf(prevX * prevX + prevY * prevY);
//	float nextLengthSq = sqrtf(nextX * nextX + nextY * nextY);
//	float dLengthSq = prevLengthSq - nextLengthSq;
	float prevR = prevState [2];
	float nextR = nextState [2];
	
//	if (prevR > 5 && nextR <= 5) {
//		reward = 5;
////	} else if (prevR <= 10 && nextR > 10) {
////		reward = -5;
//	} else {
//		reward = 0;
//	}
	
	
//	reward = (50 * exp (-nextLengthSq/40) + 0.5) * dLengthSq;

//	if (nextR < 5) {
//		reward = 5 * exp (-nextR/40);
//	} else {
		reward = (prevR - nextR);
//		if (prevLengthSq > nextLengthSq) {
//			reward = ;// * exp (-nextLengthSq/40);
//		} else {
//			reward = -1;//-exp (-nextLengthSq/40);
//		}
//	}
//CCLOG ("--- reward: %f %f %f", prevR, nextR, reward);

//	if (model.isCollided()) {
//		reward = -0.5;
//	}
	
//	sumReward += reward;
}

void QuadrocopterDiscrete2DCtrl::readState (std::vector<float>& state) {
// sparse angles
//	float r = sqrtf(model.posX * model.posX + model.posY * model.posY);
//	float targetAngle = atan2(model.posY / r, model.posX / r) + M_PI;
//	int targetSegment = int(floorf(targetAngle * 9 / M_PI));
//	float angleLimited = fmodf(model.angle + M_PI, 2*M_PI);
//	int angleSegment = int(floorf(angleLimited * 9 / M_PI));
//	
//	for (float& s : state) { s = 0; }
//	
//	state [0] = r;
//	state [1 + targetSegment] = 1;
//	state [19 + angleSegment] = 1;

	float x = model.posX - model.targetX;
	float y = model.posY - model.targetY;

	float r = sqrtf(x * x + y * y);
	state [0] = 10 * x / r;
	state [1] = 10 * y / r;
	state [2] = r;
	state [3] = 10 * sin(model.angle);
	state [4] = 10 * cos(model.angle);
	state [5] = 0;
	state [6] = 0;
	state [7] = 0;
	
	state [8] = x;
	state [9] = y;
	state [10] = x * x * 0.01;
	state [11] = y * y * 0.01;
	state [12] = state [2] * state [2] * 0.01;
	state [13] = state [3] * state [3] * 0.1;
	state [14] = state [4] * state [4] * 0.1;
	state [15] = 0;
	state [16] = 0;
	state [17] = 0;
	
	int sensorI = 18;
	for (auto& s : model.getSensors()) {
		state [sensorI] = s;
		sensorI++;
	}
}

void QuadrocopterDiscrete2DCtrl::act () {

	//getting prev state
	readState(prevState);
	
	action = Quadrocopter2DBrain::quadrocopterBrainAct(id, prevState);
	switch (action) {
		case 0:
			model.angle += M_PI / 18;
			model.motor1power = 2;
			model.motor2power = 0;
		break;
		
		case 1:
			model.angle -= M_PI / 18;
			model.motor1power = 0;
			model.motor2power = 2;
		break;
		
		case 2:
			//move forward
			model.posX += 2 * cosf (model.angle);
			model.posY += 2 * sinf (model.angle);
			model.motor1power = 10;
			model.motor2power = 10;
		break;
		
		case 3:
			model.motor1power = 0;
			model.motor2power = 0;
		break;
		
		case 4:
			model.motor1power = 0;
			model.motor2power = 0;
		break;
	}
	
//	model.posY += 0.8;
}

void QuadrocopterDiscrete2DCtrl::storeExperience () {

	if (action == -1) {
		return;
	}

	readState(nextState);
	
	calcReward();
	
	Quadrocopter2DBrain::storeQuadrocopterExperience(id, reward, action, prevState, nextState);
}

void QuadrocopterDiscrete2DCtrl::reset () {
	action = -1;
	model.posX = Lib::randFloat(-100, 100);
	model.posY = Lib::randFloat(-100, 100);
//	model.angle = Lib::randInt(0, 5) * M_PI / 3;
	model.angle = Lib::randFloat(0, 2 * M_PI);
}

void QuadrocopterDiscrete2DCtrl::resetAction () {
	action = -1;
}

double QuadrocopterDiscrete2DCtrl::getReward () {
	return reward;
}

QuadrocopterModel2DIFace& QuadrocopterDiscrete2DCtrl::getModel () {
	return model;
}

void QuadrocopterDiscrete2DCtrl::setState (const std::vector<float> state) {
//	float x = model.posX - model.targetX;
//	float y = model.posY - model.targetY;
//
//	float r = sqrtf(x * x + y * y);
//	state [0] = 10 * x / r;
//	state [1] = 10 * y / r;
//	state [2] = r;
//	state [3] = 10 * sin(model.angle);
//	state [4] = 10 * cos(model.angle);
//	state [5] = 0;
//	state [6] = 0;
//	state [7] = 0;

	action = -1;
	model.posX = state [0] * state [2] / 10 + model.targetX;
	model.posY = state [1] * state [2] / 10 + model.targetY;
	model.angle = atan2f(state [4], state [3]);
}

void QuadrocopterDiscrete2DCtrl::onSimulationStep (int stepIndex) {

	if (fabsf(model.posX) > 200 || fabsf(model.posY) > 200) {
		reset();
		return;
	}
}
