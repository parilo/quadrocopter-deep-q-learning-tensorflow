//
//  QuadrocopterCtrl.hpp
//  Quadrocopter-Visualizer
//
//  Created by anton on 29/04/16.
//
//

#ifndef QuadrocopterCtrl_hpp
#define QuadrocopterCtrl_hpp

#include "Simulation.hpp"

class QuadrocopterCtrl {
public:
	
	QuadrocopterCtrl (int id, Quadrocopter1D& simulationModel);
	
	void act ();
	void storeExperience ();
	void calcReward ();
	void reset ();
	double getReward ();
	Quadrocopter1D& getSimulationModel ();

private:

	int id;
	Quadrocopter1D& simulationModel;
	long action = -1;
	int timeReward = 0;
	double reward;
//	double sumReward = 0;
	std::vector<float> prevState;
	std::vector<float> nextState;

};

#endif /* QuadrocopterCtrl_hpp */
