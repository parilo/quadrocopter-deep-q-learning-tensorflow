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

QuadrocopterDiscreteCtrl::QuadrocopterDiscreteCtrl (int id, QuadrocopterDiscrete& simulationModel) : id(id), simulationModel(simulationModel) {
	prevState.resize(2);
	nextState.resize(2);
}

void QuadrocopterDiscreteCtrl::calcReward () {
	
	//награда за приближение к точке
	float prevX = fabsf(prevState [0]);
	float nextX = fabsf(nextState [0]);
	if (nextX < 5) {
		reward = exp (-nextX/40);
	} else {
		if (prevX > nextX) {
	//		reward = 10/(nextX+1);
			reward = exp (-nextX/40);
		} else {
	//		reward = -10/(prevX+1);
			reward = -exp (-nextX/40);
		}
	}


//	float prevX = fabsf(prevState [0]);
//	float nextX = fabsf(nextState [0]);
//	if (nextX < 10) {
//		reward = exp (-nextX*nextX/40);
//	} else {
//		reward = 0;
//	}


//	sumReward += reward;
}

QuadrocopterDiscrete& QuadrocopterDiscreteCtrl::getModel () {
	return simulationModel;
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
	
	simulationModel.setPosition(position);
	
	if (timeReward>0) timeReward--;
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
