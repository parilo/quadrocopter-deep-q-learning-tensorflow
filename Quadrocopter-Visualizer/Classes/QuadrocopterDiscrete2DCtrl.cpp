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

static const int observationSize = 8;

QuadrocopterDiscrete2DCtrl::QuadrocopterDiscrete2DCtrl (int id, Quadrocopter2D& simulationModel) :
	id(id)//,
//	simulationModel(simulationModel)
{
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
	
//	reward = (50 * exp (-nextLengthSq/40) + 0.5) * dLengthSq;

	if (nextR < 5) {
		reward = 5 * exp (-nextR/40);
	} else {
		reward = prevR - nextR;
//		if (prevLengthSq > nextLengthSq) {
//			reward = ;// * exp (-nextLengthSq/40);
//		} else {
//			reward = -1;//-exp (-nextLengthSq/40);
//		}
	}
	
//	sumReward += reward;
}

void QuadrocopterDiscrete2DCtrl::readState (std::vector<float>& state) {
	float r = sqrtf(model.posX * model.posX + model.posY * model.posY);
	state [0] = model.posX / r;
	state [1] = model.posY / r;
	state [2] = r;
	state [3] = sin(model.angle);
	state [4] = cos(model.angle);
	state [5] = 0;
	state [6] = 0;
	state [7] = 0;
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
			model.posX += cosf (model.angle);
			model.posY += sinf (model.angle);
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
	
//	simulationModel.setCoords(b2Vec2(posX, posY), angle * 180 / M_PI);
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

double QuadrocopterDiscrete2DCtrl::getReward () {
	return reward;
}

QuadrocopterModel2DIFace& QuadrocopterDiscrete2DCtrl::getModel () {
	return model;
}



void QuadrocopterDiscreteModel2D::setCoords (const b2Vec2& pos, float angle) {
	posX = pos.x;
	posY = pos.y;
	this->angle = angle;
}

void QuadrocopterDiscreteModel2D::setVelocity (const b2Vec2& v) {
}

void QuadrocopterDiscreteModel2D::setAngularVelocity (float w) {
}

void QuadrocopterDiscreteModel2D::getPartsCoords (
	b2Vec2& bodyPos,
	b2Vec2& motor1Pos,
	b2Vec2& motor2Pos,
	float& bodyRotation,
	float& motor1Rotation,
	float& motor2Rotation
) const {
	bodyPos.x = posX;
	bodyPos.y = posY;
	
	static const float halfBodyLength = 0.2f;
	
	b2Vec2 posAdd (- halfBodyLength * sinf (angle), halfBodyLength * cosf(angle));
	motor1Pos = bodyPos + posAdd;
	motor2Pos = bodyPos - posAdd;
	
	bodyRotation = (angle + M_PI_2) * 180 / M_PI;
	motor1Rotation = bodyRotation;
	motor2Rotation = motor1Rotation;
}

void QuadrocopterDiscreteModel2D::getMotorPower (float& p1, float& p2) const {
	p1 = motor1power;
	p2 = motor2power;
}
