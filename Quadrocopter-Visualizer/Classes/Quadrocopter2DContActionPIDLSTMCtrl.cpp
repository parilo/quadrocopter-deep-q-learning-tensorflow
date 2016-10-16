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
const int lstmStepsCount = 20;

Quadrocopter2DContActionPIDLSTMCtrl::Quadrocopter2DContActionPIDLSTMCtrl (
	int id,
	Quadrocopter2D& simulationModel
) :
	Quadrocopter2DCtrl (id, simulationModel),
	targetAngle(0),
	throttle(6.5),
	anglePID(8, 0, 1.6),
	actionCont(actionSize, 0)
{
}

void Quadrocopter2DContActionPIDLSTMCtrl::actPID (double& motor1power, double& motor2power) {

	double angle = simulationModel.getRotation();
//	angle = angle>0?fmod(angle, 2*M_PI):2*M_PI-fmod(-angle, 2*M_PI);
//	angle -= M_PI;
//if (id == 14) CCLOG ("angles: %lf %lf %lf", targetAngle, angle, simulationModel.getRotation());
	double dAngle = - targetAngle + angle;

//	dAngle -= Math.Ceiling (Math.Floor (dPitch / 180.0) / 2.0) * 360.0;

	motor1power = throttle;
	motor2power = throttle;

	double angleForce = anglePID.calc (0, dAngle / M_PI);
//	angleForce = angleForce>forceLimit?forceLimit:angleForce;
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

//	float p1 = fabs(actionCont [0]);
//	float p2 = fabs(actionCont [1]);
////	p1 = p1>0?p1:0;
////	p2 = p2>0?p2:0;

//	actionCont [0] = 11;
//	actionCont [1] = 0;

//	throttle = fabs (actionCont [0]) * 10.0 / 15.0;
//	targetAngle = actionCont [1] * 2 * M_PI / 15.0; //normalization

	throttle = fabs (actionCont [0]);

//if (id == 14) CCLOG ("--- action: %f %f %lf %lf", actionCont [0], actionCont [1], throttle, targetAngle);
	
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
//	Quadrocopter2DBrain::storeQuadrocopterExperienceContLSTM(id, reward, actionCont, prevState, lstmStateC, lstmStateH, nextState, outLstmStateC, outLstmStateH);
}

void Quadrocopter2DContActionPIDLSTMCtrl::reset () {
	Quadrocopter2DCtrl::reset();
	//!! need to reinit lstm ObservationSeqs
}
