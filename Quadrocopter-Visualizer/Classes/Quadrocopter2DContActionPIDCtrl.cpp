//
//  Quadrocopter2DContActionPIDCtrl.cpp
//  Quadrocopter-Visualizer
//
//  Created by anton on 04/09/16.
//
//

#include "Quadrocopter2DContActionPIDCtrl.hpp"
#include "Quadrocopter2DBrain.hpp"
#include "Lib.h"

const int actionSize = 2;

Quadrocopter2DContActionPIDCtrl::Quadrocopter2DContActionPIDCtrl (
	int id,
	Quadrocopter2D& simulationModel
) :
	Quadrocopter2DCtrl (id, simulationModel),
	targetAngle(0),
	throttle(6.5),
	anglePID(8, 0, 1.6)
{
	actionCont.resize(actionSize);
}

void Quadrocopter2DContActionPIDCtrl::actPID (double& motor1power, double& motor2power) {

	double angle = simulationModel.getRotation();
//	angle = angle>0?fmod(angle, 2*M_PI):2*M_PI-fmod(-angle, 2*M_PI);
//	angle -= M_PI;
//if (id == 14) CCLOG ("angles: %lf %lf %lf", targetAngle, angle, fabs(simulationModel.getAngularVelocity()));
	double dAngle = - targetAngle + angle;

//	dAngle -= Math.Ceiling (Math.Floor (dPitch / 180.0) / 2.0) * 360.0;

	motor1power = throttle;
	motor2power = throttle;

	double angleForce = anglePID.calc (0, dAngle / M_PI);
//	angleForce = angleForce>forceLimit?forceLimit:angleForce;
	motor1power -=   angleForce;
	motor2power +=   angleForce;
}

void Quadrocopter2DContActionPIDCtrl::calcReward () {

	//награда за приближение к точке
	float prevR = prevState [2];
	float nextR = nextState [2];

	reward = 0.1 * (prevR - nextR);
	if (reward > 0) reward *= 0.5;

	reward -= 0.005 * fabs(simulationModel.getAngularVelocity());

	if (simulationModel.isCollided()) {
//std::cout << "--- collided " << std::endl;
		reward = - 0.5;
	}

//	if (fabs(reward)>0.5) {
//		double prevX = prevState [0] * prevState [2] / 10;
//		double prevY = prevState [1] * prevState [2] / 10;
//		double nextX = nextState [0] * nextState [2] / 10;
//		double nextY = nextState [1] * nextState [2] / 10;
//		std::cout
//			<< id
//			<< " reward: " << reward
//			<< " prev r: " << prevR << " next r: " << nextR
//			<< " (" << prevX << ", " << prevY << ") -> (" << nextX << ", " << nextY << ")" << " "
//			<< actionCont [0] << ", " << actionCont [1]
//			<< " v (" << prevState [5] << ", " << prevState [6] << ") -> (" << nextState[5] << ", " << nextState[6] << ")"
//			<< std::endl;
//	}

//if (id == 14) std::cout << "reward: " << reward << std::endl;

}

void Quadrocopter2DContActionPIDCtrl::act () {

	//getting prev state
	readState(prevState);

	Quadrocopter2DBrain::quadrocopterBrainActCont(id, prevState, actionCont);

////if (id == 14) CCLOG ("--- action: %f %f %f %f", prevState [8], prevState [9], actionCont [0], actionCont [1]);
//	float p1 = fabs(actionCont [0]);
//	float p2 = fabs(actionCont [1]);
////	p1 = p1>0?p1:0;
////	p2 = p2>0?p2:0;

	throttle = fmin(15, fabs (actionCont [0]));
	targetAngle = fmod(actionCont [1] * 0.2, 2*M_PI);

//	throttle = 7;
//	if (targetAngle < 0) {
//		targetAngle = 3.2;
//	} else {
//		targetAngle = -3.2;
//	}

//	if (id == 14)
//	std::cout
//		<< "--- action: "
//		<< throttle << " " << targetAngle
//		<< std::endl;
	
//if (id == 14) CCLOG ("--- action: %f %f %lf %lf", actionCont [0], actionCont [1], throttle, targetAngle);
	
	double p1;
	double p2;
	
	actPID(p1, p2);

//if (id == 0) CCLOG ("--- motor power: %lf %lf %lf %lf", throttle, targetAngle, p1, p2);

	simulationModel.setMotorPower(p1, p2);

	reseted = false;
}

void Quadrocopter2DContActionPIDCtrl::storeExperience () {

	if (reseted) {
		return;
	}
	
	readState(nextState);
	
	calcReward();
	
	Quadrocopter2DBrain::storeQuadrocopterExperienceCont(id, reward, actionCont, prevState, nextState);
}
