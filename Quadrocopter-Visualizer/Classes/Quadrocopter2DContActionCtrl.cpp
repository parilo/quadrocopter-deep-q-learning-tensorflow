//
//  Quadrocopter2DContActionCtrl.cpp
//  Quadrocopter-Visualizer
//
//  Created by anton on 14/08/16.
//
//

#include "Quadrocopter2DContActionCtrl.hpp"
#include "Quadrocopter2DBrain.hpp"
#include "Lib.h"

const int actionSize = 2;

Quadrocopter2DContActionCtrl::Quadrocopter2DContActionCtrl (
	int id,
	Quadrocopter2D& simulationModel
) : Quadrocopter2DCtrl (id, simulationModel)
{
	actionCont.resize(actionSize);
}

void Quadrocopter2DContActionCtrl::calcReward () {

	//награда за приближение к точке
	float prevR = prevState [2];
	float nextR = nextState [2];

  reward = 0.01 * (prevR - nextR);
  if (reward > 0) reward *= 0.9;

	if (simulationModel.isCollided()) {
//std::cout << "--- collided " << std::endl;
    reward -= 0.2;
	}

}

void Quadrocopter2DContActionCtrl::act () {

	//getting prev state
	readState(prevState);
	
	Quadrocopter2DBrain::quadrocopterBrainActCont(id, prevState, actionCont);

//if (id == 14) CCLOG ("--- action: %f %f %f %f", prevState [8], prevState [9], actionCont [0], actionCont [1]);
	float p1 = fmin(15, fabs(actionCont [0]));
	float p2 = fmin(15, fabs(actionCont [1]));
//	p1 = p1>0?p1:0;
//	p2 = p2>0?p2:0;
	simulationModel.setMotorPower(p1, p2);

	reseted = false;
}

void Quadrocopter2DContActionCtrl::storeExperience () {

	if (reseted) {
		return;
	}
	
	readState(nextState);
	
	calcReward();
	
	Quadrocopter2DBrain::storeQuadrocopterExperienceCont(id, reward, actionCont, prevState, nextState);
}
