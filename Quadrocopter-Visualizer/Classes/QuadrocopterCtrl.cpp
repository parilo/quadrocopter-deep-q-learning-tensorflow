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


//	reward = (exp (-nextX/40) + 1) * (prevX - nextX);
//	if (fabsf(nextX) > 5) {
//		float v = prevX - nextX;
////		reward = 0;
//		reward = fminf(1, v);
//	} else {
//		reward = 5 * exp (-nextX/40);
//	}

//	if (prevX > nextX) {
////		reward = 10/(nextX+1);
//		reward = 10 * exp (-nextX/40);
//	} else {
////		reward = -10/(prevX+1);
//		reward = -10 * exp (-prevX/40);
//	}


//	sumReward += reward;
}

void QuadrocopterCtrl::readState (std::vector<float>& state) {

	for (auto& stateItem : state) {
		stateItem = 0;
	}

	float x = simulationModel.getPosition();
	float v = simulationModel.getVelocity();

	if (x>100) {
		state [0] = (x - 100) / 100.0;
	} else if (x>50) {
		state [1] = (x-50) / 50.0;
	} else if (x>20) {
		state [2] = (x-20)/30.0;
	} else if (x>10) {
		state [3] = (x-10)/10.0;
	} else if (x>5) {
		state [4] = (x-5)/5.0;
	} else if (x>2) {
		state [5] = (x-2)/3.0;
	} else if (x>1) {
		state [6] = x-1;
	} else if (x>0) {
		state [7] = x;
	}

	if (x<-100) {
		state [8] = -(x + 100) / 100.0;
	} else if (x<-50) {
		state [9] = -(x+50) / 50.0;
	} else if (x<-20) {
		state [10] = -(x+20)/30.0;
	} else if (x<-10) {
		state [11] = -(x+10)/10.0;
	} else if (x<-5) {
		state [12] = -(x+5)/5.0;
	} else if (x<-2) {
		state [13] = -(x+2)/3.0;
	} else if (x<-1) {
		state [14] = -x+1;
	} else if (x<0) {
		state [15] = -x;
	}

	int s = 16;
	if (v>10) {
		state [s+0] = (v - 10) / 10.0;
	} else if (v>5) {
		state [s+1] = (v-5) / 5.0;
	} else if (v>2) {
		state [s+2] = (v-2)/3.0;
	} else if (v>1) {
		state [s+3] = (v-1);
	} else if (v>0.5) {
		state [s+4] = (v-0.5)/0.5;
	} else if (v>0.2) {
		state [s+5] = (v-0.2)/0.2;
	} else if (v>0.1) {
		state [s+6] = (v-0.1)/0.1;
	} else if (v>0) {
		state [s+7] = v/0.1;
	}

	if (v<-10) {
		state [s+8] = -(v + 10) / 10.0;
	} else if (v<-5) {
		state [s+9] = -(v+5) / 5.0;
	} else if (v<-2) {
		state [s+10] = -(v+2)/3.0;
	} else if (v<-1) {
		state [s+11] = -(v+1);
	} else if (v<-0.5) {
		state [s+12] = -(v+0.5)/0.5;
	} else if (v<-0.2) {
		state [s+13] = -(v+0.2)/0.3;
	} else if (v<-0.1) {
		state [s+14] = -(v+0.1)/0.1;
	} else if (v<0) {
		state [s+15] = -v/0.1;
	}
}

void QuadrocopterCtrl::act () {

	//getting prev state
	prevState [0] = simulationModel.getPosition();
	prevState [1] = simulationModel.getVelocity();
//	prevX = simulationModel.getPosition();
//	readState(prevState);
	
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
//	nextX = simulationModel.getPosition();
//	readState(nextState);
	
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
