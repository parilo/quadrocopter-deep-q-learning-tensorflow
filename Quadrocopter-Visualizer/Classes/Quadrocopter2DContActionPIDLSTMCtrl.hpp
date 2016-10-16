//
//  Quadrocopter2DContActionPIDLSTMCtrl.hpp
//  Quadrocopter-Visualizer
//
//  Created by anton on 05/10/16.
//
//

#ifndef Quadrocopter2DContActionPIDLSTMCtrl_hpp
#define Quadrocopter2DContActionPIDLSTMCtrl_hpp

#include "Quadrocopter2DCtrl.hpp"
#include "PID.hpp"
#include <list>

class Quadrocopter2DContActionPIDLSTMCtrl : public Quadrocopter2DCtrl {
public:
	
	Quadrocopter2DContActionPIDLSTMCtrl (int id, Quadrocopter2D& simulationModel);
	
	void act () override;
	void calcReward () override;
	void storeExperience () override;
	void reset () override;
	
protected:

	std::vector<float> actionCont;
	
	double throttle;
	double targetAngle;
	PID anglePID;
	
	void actPID (double& motor1power, double& motor2power);
	
	
};

#endif /* Quadrocopter2DContActionPIDLSTMCtrl_hpp */
