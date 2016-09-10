//
//  Quadrocopter2DContActionPIDCtrl.hpp
//  Quadrocopter-Visualizer
//
//  Created by anton on 04/09/16.
//
//

#ifndef Quadrocopter2DContActionPIDCtrl_hpp
#define Quadrocopter2DContActionPIDCtrl_hpp

#include "Quadrocopter2DCtrl.hpp"
#include "PID.hpp"

class Quadrocopter2DContActionPIDCtrl : public Quadrocopter2DCtrl {
public:
	
	Quadrocopter2DContActionPIDCtrl (int id, Quadrocopter2D& simulationModel);
	
	void act () override;
	void calcReward () override;
	void storeExperience () override;
	
protected:

	std::vector<float> actionCont;
	
	double throttle;
	double targetAngle;
	PID anglePID;
	
	void actPID (double& motor1power, double& motor2power);
	
	
};

#endif /* Quadrocopter2DContActionPIDCtrl_hpp */
