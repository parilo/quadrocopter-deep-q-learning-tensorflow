//
//  Quadrocopter2DCtrl.hpp
//  Quadrocopter-Visualizer
//
//  Created by anton on 24/05/16.
//
//

#ifndef Quadrocopter2DCtrl_hpp
#define Quadrocopter2DCtrl_hpp

#include "Simulation.hpp"

class Quadrocopter2DCtrl {
public:
	
	Quadrocopter2DCtrl (int id, Quadrocopter2D& simulationModel);
	
	virtual void act ();
	virtual void storeExperience ();
	virtual void calcReward ();
	virtual void reset ();
	virtual void resetAction ();
	double getReward ();
	void setState (const std::vector<float> state) {}
	QuadrocopterModel2DIFace& getModel ();
	void onTrainStep (int trainStepIndex);
	virtual void onSimulationStep (int stepIndex);

protected:

	int id;
	bool reseted = false;
	Quadrocopter2D& simulationModel;
	long action = -1;
	double time;
	double reward;
//	double sumReward = 0;
	std::vector<float> prevState;
	std::vector<float> nextState;
	
	double rewardWeight = 0.2;
	double linearDamping = 0.0;
	double angularDamping = 0.1;
	float gravity = 0;

	void readState (std::vector<float>& state);

};

#endif /* Quadrocopter2DCtrl_hpp */
