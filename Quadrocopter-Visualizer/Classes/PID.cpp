//
//  PID.cpp
//  Quadrocopter-Visualizer
//
//  Created by anton on 04/09/16.
//
//

#include "PID.hpp"

PID::PID (double P, double I, double D) :
sumErr(0),
prevErr(0)
{
	this->P = P;
	this->I = I;
	this->D = D;
}

const double dt = 0.03;

double PID::calc (double current, double target) {
	
	double err = target - current;
	this->sumErr += err;
	
	double force = P * err + I * sumErr * dt + D * (err - prevErr) / dt;
	
	this->prevErr = err;
	return force;
}
