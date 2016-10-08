//
//  Quadrocopter2DContActionPIDLSTMCtrl.cpp
//  Quadrocopter-Visualizer
//
//  Created by anton on 05/10/16.
//
//

#include "Quadrocopter2DContActionPIDLSTMCtrl.hpp"
#include "Quadrocopter2DBrain.hpp"
#include "Lib.h"

const int actionSize = 2;
const int lstmStateSize = 128;

Quadrocopter2DContActionPIDLSTMCtrl::Quadrocopter2DContActionPIDLSTMCtrl (
	int id,
	Quadrocopter2D& simulationModel
) :
	Quadrocopter2DCtrl (id, simulationModel),
	targetAngle(0),
	throttle(6.5),
	anglePID(0.4, 0, 0.08),
	actionCont(actionSize, 0),
	lstmStateC(lstmStateSize, 0),
	lstmStateH(lstmStateSize, 0),
	outLstmStateC(lstmStateSize, 0),
	outLstmStateH(lstmStateSize, 0)
{
//	CCLOG("--- size: %ld %ld %ld %ld", lstmStateC.size(), lstmStateH.size(), outLstmStateC.size(), outLstmStateH.size());
}

void Quadrocopter2DContActionPIDLSTMCtrl::actPID (double& motor1power, double& motor2power) {

	double angle = simulationModel.getRotation();
	angle = angle>0?fmod(angle, 2*M_PI):2*M_PI-fmod(-angle, 2*M_PI);
	angle -= M_PI;
//CCLOG ("angles: %lf %lf", targetAngle, angle);
	double dAngle = targetAngle - angle;

//	dAngle -= Math.Ceiling (Math.Floor (dPitch / 180.0) / 2.0) * 360.0;

	motor1power = throttle;
	motor2power = throttle;

	double angleForce = anglePID.calc (0, dAngle / M_PI);
	angleForce = angleForce>5?5:angleForce;
	motor1power -=   angleForce;
	motor2power +=   angleForce;
}

void Quadrocopter2DContActionPIDLSTMCtrl::calcReward () {

	//награда за приближение к точке
	float prevR = prevState [2];
	float nextR = nextState [2];

	reward = 0.1 * (prevR - nextR);
	if (reward > 0) reward *= 0.5;

	if (simulationModel.isCollided()) {
//std::cout << "--- collided " << std::endl;
		reward -= 1.0;
	}

}

void Quadrocopter2DContActionPIDLSTMCtrl::act () {

	//getting prev state
	readState(prevState);

	lstmStateC.swap(outLstmStateC);
	lstmStateH.swap(outLstmStateH);
	Quadrocopter2DBrain::quadrocopterBrainActContLSTM(id, prevState, lstmStateC, lstmStateH, actionCont, outLstmStateC, outLstmStateH);

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

////if (id == 14) CCLOG ("--- action: %f %f %f %f", prevState [8], prevState [9], actionCont [0], actionCont [1]);
//	float p1 = fabs(actionCont [0]);
//	float p2 = fabs(actionCont [1]);
////	p1 = p1>0?p1:0;
////	p2 = p2>0?p2:0;

	throttle = fabs (actionCont [0]) * 10.0 / 15.0;
	targetAngle = actionCont [1] * 2 * M_PI / 15.0; //normalization
	
	double p1;
	double p2;
	
	actPID(p1, p2);

//if (id == 0) CCLOG ("--- motor power: %lf %lf %lf %lf", throttle, targetAngle, p1, p2);

	simulationModel.setMotorPower(p1, p2);

	reseted = false;
}

void Quadrocopter2DContActionPIDLSTMCtrl::storeExperience () {

	if (reseted) {
		return;
	}
	
	readState(nextState);
	
	calcReward();
	
//	Quadrocopter2DBrain::storeQuadrocopterExperienceCont(id, reward, actionCont, prevState, nextState);
	Quadrocopter2DBrain::storeQuadrocopterExperienceContLSTM(id, reward, actionCont, prevState, lstmStateC, lstmStateH, nextState, outLstmStateC, outLstmStateH);
}

void Quadrocopter2DContActionPIDLSTMCtrl::reset () {
	Quadrocopter2DCtrl::reset();
	std::fill(lstmStateC.begin(), lstmStateC.end(), 0);
	std::fill(lstmStateH.begin(), lstmStateH.end(), 0);
	std::fill(outLstmStateC.begin(), outLstmStateC.end(), 0);
	std::fill(outLstmStateH.begin(), outLstmStateH.end(), 0);
}
