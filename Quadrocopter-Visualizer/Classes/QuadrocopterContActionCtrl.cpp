//
//  QuadrocopterContActionCtrl.cpp
//  Quadrocopter-Visualizer
//
//  Created by anton on 13/08/16.
//
//

#include "QuadrocopterContActionCtrl.hpp"
#include "Quadrocopter2DBrain.hpp"
#include "Lib.h"

const int actionSize = 1;

QuadrocopterContActionCtrl::QuadrocopterContActionCtrl (int id, Quadrocopter1D& simulationModel) :
	QuadrocopterCtrl(id, simulationModel)
{
	actionCont.resize(actionSize);
}

void QuadrocopterContActionCtrl::calcReward () {

	//награда за приближение к точке
	float prevX = fabsf(prevState [0]);
	float nextX = fabsf(nextState [0]);

	reward = 2 * (prevX - nextX);
	if (reward > 0) reward *= 0.5;

}

void QuadrocopterContActionCtrl::act () {

	//getting prev state
	readState(prevState);
	
	Quadrocopter2DBrain::quadrocopterBrainActCont(id, prevState, actionCont);
//	actionCont [0] *= 100;
//if (id == 14) CCLOG ("--- action: %f %f", prevState [0], actionCont [0]);
	simulationModel.setMotorPower(actionCont [0]);

	reseted = false;
}

void QuadrocopterContActionCtrl::storeExperience () {

	if (reseted) {
		return;
	}
	
	readState(nextState);
	
	calcReward();
	
	Quadrocopter2DBrain::storeQuadrocopterExperienceCont(id, reward, actionCont, prevState, nextState);
}

void QuadrocopterContActionCtrl::onSimulationStep (int stepIndex) {

	float x = simulationModel.getPosition();
	if (fabsf(x) > 200) {
		reset();
		return;
	}
	
}
