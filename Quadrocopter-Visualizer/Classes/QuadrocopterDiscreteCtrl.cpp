//
//  QuadrocopterDiscreteCtrl.cpp
//  Quadrocopter-Visualizer
//
//  Created by anton on 22/05/16.
//
//

#include "QuadrocopterDiscreteCtrl.hpp"
#include "Quadrocopter2DBrain.hpp"
#include "Lib.h"

QuadrocopterDiscreteCtrl::QuadrocopterDiscreteCtrl (int id) : id(id) {
	prevState.resize(2);
	nextState.resize(2);
}

const float XLimit = 2.5;

void QuadrocopterDiscreteCtrl::calcReward () {
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
//	float prevX = getPositionFromState(prevState);
//	float nextX = getPositionFromState(nextState);
//	if (
//		(prevX > XLimit && nextX < XLimit) ||
//		(prevX < -XLimit && nextX > -XLimit)
//	) {
//		if (timeReward > 0) {
//			reward = 10 * (timeReward * 1.0 / 200.0);
//			timeReward = 0;
//		} else {
//			reward = 0;
//		}
////CCLOG ("--- x: %f %f %lf", prevX, nextX, reward);
//	} else if (
//		(prevX < XLimit && nextX > XLimit) ||
//		(prevX > -XLimit && nextX < -XLimit)
//	) {
//		reward = -10;
////CCLOG ("--- x: %f %f %lf", prevX, nextX, reward);
//	} else {
//		reward = 0;
//	}
	
	//награда за приближение к точке
//	float prevX = fabsf(getPositionFromState(prevState));
//	float nextX = fabsf(getPositionFromState(nextState));
	float prevX = fabsf(prevState [0]);
	float nextX = fabsf(nextState [0]);
	if (prevX > nextX) {
		reward = 10/(nextX+1);
	} else {
		reward = -10/(prevX+1);
	}

//	sumReward += reward;
}

float QuadrocopterDiscreteCtrl::getPositionFromState (const std::vector<float>& state) {
	if (state [0] > state [1]) {
		return state [0];
	} else {
		return -state [1];
	}
}

void QuadrocopterDiscreteCtrl::readState (std::vector<float>& state) {
	if (position > 0) {
		state [0] = position;
		state [1] = 0;
	} else {
		state [0] = 0;
		state [1] = -position;
	}
}

void QuadrocopterDiscreteCtrl::act () {

	//getting prev state
	prevState [0] = position;
	prevState [1] = 0;
//	readState(prevState);
	
	action = Quadrocopter2DBrain::quadrocopterBrainAct(id, prevState);
	switch (action) {
		case 0:
			position++;
		break;
		
		case 1:
			position--;
		break;
		
		case 2:
		break;
	}
	
	timeReward--;
}

void QuadrocopterDiscreteCtrl::storeExperience () {

	if (action == -1) {
		return;
	}
	
	nextState [0] = position;
	nextState [1] = 0;
//	readState(nextState);
	
	
	calcReward();
	
	Quadrocopter2DBrain::storeQuadrocopterExperience(id, reward, action, prevState, nextState);

//	printf("%4.2f\n", pos);
}

void QuadrocopterDiscreteCtrl::reset () {
	action = -1;
	timeReward = 200;
	position = Lib::randInt(-100, 100);
}

double QuadrocopterDiscreteCtrl::getReward () {
	return reward;
}

int QuadrocopterDiscreteCtrl::getPosition () {
	return position;
}
