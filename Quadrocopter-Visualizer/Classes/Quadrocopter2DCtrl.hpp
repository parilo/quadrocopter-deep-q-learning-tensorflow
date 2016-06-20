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
	
	void act ();
	void storeExperience ();
	void calcReward ();
	void reset ();
	double getReward ();
	QuadrocopterModel2DIFace& getModel ();

private:

	int id;
	Quadrocopter2D& simulationModel;
	long action = -1;
//	int timeReward = 0;
	double reward;
//	double sumReward = 0;
	std::vector<float> prevState;
	std::vector<float> nextState;

	void readState (std::vector<float>& state);

};

#endif /* Quadrocopter2DCtrl_hpp */
