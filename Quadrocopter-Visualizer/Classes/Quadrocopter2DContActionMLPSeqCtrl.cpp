//
//  Quadrocopter2DContActionMLPSeqCtrl.cpp
//  Quadrocopter-Visualizer
//
//  Created by anton on 26/10/16.
//
//

#include "Quadrocopter2DContActionMLPSeqCtrl.hpp"
#include "Quadrocopter2DBrain.hpp"
#include "Lib.h"

const int actionSize = 2;

Quadrocopter2DContActionMLPSeqCtrl::Quadrocopter2DContActionMLPSeqCtrl (
	int id,
	Quadrocopter2D& simulationModel
) : Quadrocopter2DCtrl (id, simulationModel)
{
	actionCont.resize(actionSize);
}

void Quadrocopter2DContActionMLPSeqCtrl::calcReward () {

	//награда за приближение к точке
	float prevR = prevState [2];
	float nextR = nextState [2];

	reward = 0.1 * (prevR - nextR);
	if (reward > 0) reward *= 0.9;

	if (simulationModel.isCollided()) {
//std::cout << "--- collided " << std::endl;
		reward = - 1;
	}

}

void Quadrocopter2DContActionMLPSeqCtrl::act () {

	//getting prev state
	readState(prevState);
	
	Quadrocopter2DBrain::quadrocopterBrainActContMLPSeq(id, prevState, actionCont);

//if (id == 14) CCLOG ("--- action: %f %f %f %f", prevState [8], prevState [9], actionCont [0], actionCont [1]);
	float p1 = fmin(15, fabs(actionCont [0]));
	float p2 = fmin(15, fabs(actionCont [1]));
//	p1 = p1>0?p1:0;
//	p2 = p2>0?p2:0;
	simulationModel.setMotorPower(p1, p2);

	reseted = false;
}

void Quadrocopter2DContActionMLPSeqCtrl::storeExperience () {

	if (reseted) {
		return;
	}
	
	readState(nextState);
	
	calcReward();
	
	Quadrocopter2DBrain::storeQuadrocopterExperienceContMLPSeq(id, reward, actionCont, prevState, nextState);
}

void Quadrocopter2DContActionMLPSeqCtrl::reset () {
	Quadrocopter2DCtrl::reset();
	readState(prevState);
	actionCont.assign(2, 0);
	Quadrocopter2DBrain::resetQuadrocopterMLPSeq (id, prevState);
}

void Quadrocopter2DContActionMLPSeqCtrl::resetAction () {
	Quadrocopter2DCtrl::resetAction();
	readState(prevState);
	actionCont.assign(2, 0);
	Quadrocopter2DBrain::resetQuadrocopterMLPSeq (id, prevState);
}
