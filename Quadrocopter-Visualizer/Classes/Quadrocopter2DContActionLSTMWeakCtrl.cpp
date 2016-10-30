//
//  Quadrocopter2DContActionLSTMWeakCtrl.cpp
//  Quadrocopter-Visualizer
//
//  Created by anton on 23/10/16.
//
//

#include "Quadrocopter2DContActionLSTMWeakCtrl.hpp"
#include "Quadrocopter2DBrain.hpp"
#include "Lib.h"

const int actionSize = 2;

Quadrocopter2DContActionLSTMWeakCtrl::Quadrocopter2DContActionLSTMWeakCtrl (
	int id,
	Quadrocopter2D& simulationModel
) : Quadrocopter2DCtrl (id, simulationModel)
{
	actionCont.resize(actionSize);
}

void Quadrocopter2DContActionLSTMWeakCtrl::calcReward () {

	//награда за приближение к точке
	float prevR = prevState [2];
	float nextR = nextState [2];

	reward = 0.1 * (prevR - nextR);
	if (reward > 0) reward *= 0.5;

	if (simulationModel.isCollided()) {
//std::cout << "--- collided " << std::endl;
		reward = - 1;
	}
//std::cout << "--- reward " << reward << std::endl;

}

void Quadrocopter2DContActionLSTMWeakCtrl::act () {
//if (id == 0)
//std::cout << "--- act: " << id << std::endl;
	//getting prev state
	readState(prevState);
	
	Quadrocopter2DBrain::quadrocopterBrainActContLSTMWeak (id, prevState, actionCont);

//if (id == 14) CCLOG ("--- action: %f %f %f %f", prevState [8], prevState [9], actionCont [0], actionCont [1]);
	float p1 = fmin(15, fabs(actionCont [0]));
	float p2 = fmin(15, fabs(actionCont [1]));
	simulationModel.setMotorPower(p1, p2);

	reseted = false;
//if (id == 0)
//std::cout << "--- act2: " << id << " / " << (reseted?1:0) << std::endl;
}

void Quadrocopter2DContActionLSTMWeakCtrl::storeExperience () {
//if (id == 0)
//std::cout << "--- store: " << id << " / " << (reseted?1:0) << std::endl;

	if (reseted) {
		return;
	}

//if (id == 0)
//std::cout << "--- store2: " << id << std::endl;
	
	readState(nextState);
	
	calcReward();
	
	Quadrocopter2DBrain::storeQuadrocopterExperienceContLSTMWeak(id, reward, actionCont, prevState, nextState);
}

void Quadrocopter2DContActionLSTMWeakCtrl::reset () {
	Quadrocopter2DCtrl::reset();
	readState(prevState);
	actionCont.assign(2, 0);
	Quadrocopter2DBrain::resetQuadrocopterLSTMWeak (id, prevState);
}

void Quadrocopter2DContActionLSTMWeakCtrl::resetAction () {
	Quadrocopter2DCtrl::resetAction();
	readState(prevState);
	actionCont.assign(2, 0);
	Quadrocopter2DBrain::resetQuadrocopterLSTMWeak (id, prevState);
}
