//
//  QuadrocopterDiscreteCtrl.hpp
//  Quadrocopter-Visualizer
//
//  Created by anton on 22/05/16.
//
//

#ifndef QuadrocopterDiscreteCtrl_hpp
#define QuadrocopterDiscreteCtrl_hpp

#include "Simulation.hpp"

class QuadrocopterDiscrete {

public:

	QuadrocopterDiscrete ();
	virtual ~QuadrocopterDiscrete ();
	
	void createIn (World& w);

	float getPosition ();
	void setPosition (float pos);
	void setVelocity (float v);
	float getVelocity ();
	void setMotorPower (float p);
	
	void step ();
	
private:
	float position;
};

class QuadrocopterDiscreteCtrl {
public:
	
	QuadrocopterDiscreteCtrl (int id, QuadrocopterDiscrete& simulationModel);
	
	void act ();
	void storeExperience ();
	void calcReward ();
	void reset ();
	double getReward ();
	int getPosition ();
	QuadrocopterDiscrete& getSimulationModel ();

private:

	int id;
	long action = -1;
	int timeReward = 0;
	double reward;
	int position = 0;
	int prevPosition = 0;
	std::vector<float> prevState;
	std::vector<float> nextState;
	
	QuadrocopterDiscrete& simulationModel;

};

#endif /* QuadrocopterDiscreteCtrl_hpp */
