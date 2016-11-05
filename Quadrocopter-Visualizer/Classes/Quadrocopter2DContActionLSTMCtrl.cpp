//
//  Quadrocopter2DContActionLSTMCtrl.cpp
//  Quadrocopter-Visualizer
//
//  Created by anton on 05/11/2016.
//
//

#include "Quadrocopter2DContActionLSTMCtrl.hpp"
#include "Quadrocopter2DBrain.hpp"
#include "Lib.h"

const int actionSize = 2;
const int lstmStepsCount = 20;

Quadrocopter2DContActionLSTMCtrl::Quadrocopter2DContActionLSTMCtrl (
	int id,
	Quadrocopter2D& simulationModel
) :
	Quadrocopter2DCtrl (id, simulationModel),
	actionCont(actionSize, 0)
{
}

void Quadrocopter2DContActionLSTMCtrl::calcReward () {

	//награда за приближение к точке
	float prevR = prevState [2];
	float nextR = nextState [2];

	reward = 0.1 * (prevR - nextR);
	if (reward > 0) reward *= 0.9;

	if (simulationModel.isCollided()) {
//std::cout << "--- collided " << std::endl;
		reward -= 1.0;
	}

}

void Quadrocopter2DContActionLSTMCtrl::act () {

	//getting prev state
	readState(prevState);

	Quadrocopter2DBrain::quadrocopterBrainActContLSTM(id, prevState, actionCont);

//	if (id == 0) {
//		std::cout << "lstm c" << std::endl;
//		printVector(lstmStateC);
//		std::cout << "lstm h" << std::endl;
//		printVector(lstmStateH);
//		std::cout << "out lstm c" << std::endl;
//		printVector(outLstmStateC);
//		std::cout << "out lstm h" << std::endl;
//		printVector(outLstmStateH);
//	}

	float p1 = fmin(15, fabs(actionCont [0]));
	float p2 = fmin(15, fabs(actionCont [1]));

	simulationModel.setMotorPower(p1, p2);

	reseted = false;
}

void Quadrocopter2DContActionLSTMCtrl::storeExperience () {

	if (reseted) {
		return;
	}
	
	readState(nextState);
	
	calcReward();
	
//	Quadrocopter2DBrain::storeQuadrocopterExperienceCont(id, reward, actionCont, prevState, nextState);
	Quadrocopter2DBrain::storeQuadrocopterExperienceContLSTM(id, reward, actionCont, prevState, nextState);
}

void Quadrocopter2DContActionLSTMCtrl::reset () {
	Quadrocopter2DCtrl::reset();
	Quadrocopter2DBrain::resetQuadrocopterLSTM (id);
}
